/*
	$Id: blit_transparent.cpp,v 1.4 2001/09/02 02:24:43 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	Alpha-only blitter.
	This file by Martin Starch (starch@dark.x.dtu.dk) 3/99
	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <Display/Display/Generic/blit_alphamask_rle.h>
#include <Display/Display/Generic/blit_transparent.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <API/Display/Display/pixeldata.h>

#ifndef WIN32
#include "Core/System/Generic/string_asm.h"
#else
#define fast_memmove memcpy
#endif

#undef USE_I386_ASSEMBLER

#if !defined(WIN32) || defined(USE_TASM)

	/* assembler acceleration -- mbn 03/99 */

	extern "C"
	{
		void blit_transparent_noclip_asm_16bpp(
			unsigned char *src,
			unsigned short *dest,
			unsigned int width);

		void blit_transparent_noclip_asm_32bpp(
			unsigned char *src,
			unsigned int *dest,
			unsigned int width);

		void blit_transparent_clip_asm_16bpp(
			unsigned char *src,
			unsigned short *dest,
			unsigned int width);

		void blit_transparent_clip_asm_32bpp(
			unsigned char *src,
			unsigned int *dest,
			unsigned int width);
	}

#endif


CL_Blit_Transparent::CL_Blit_Transparent(
	CL_DisplayCard_Generic *card,
	CL_SurfaceProvider *provider)
{
	m_card = card;
	m_provider = provider;

	m_contained_alpha_blitter = NULL;
	m_lines = NULL;
	m_num_lines = 0;
}

CL_Blit_Transparent::~CL_Blit_Transparent()
{
	if (m_lines != NULL)
	{
		for (unsigned int i=0;i<m_num_lines;i++) 
		{
			if (m_lines[i]) delete[] m_lines[i];
		}
	}
	delete[] m_lines;
	delete m_contained_alpha_blitter;
}

bool CL_Blit_Transparent::init_surface(CL_Blitters *blitters)
{
	CL_PixelData alpha(
		0,
		0,
		0,
		255,
		m_provider,
		1);

	m_num_lines = m_provider->get_height()*m_provider->get_num_frames();

	int bytes_per_line = alpha.get_bytes_per_line();
	unsigned char *alpha_data = new unsigned char[m_num_lines*bytes_per_line];

	bool has_alpha = false;
	bool transparent = false;

	int alpha_width = alpha.get_width();

	unsigned int y;
	for (y=0;y<m_num_lines;y++)
	{
		unsigned char *l = &alpha_data[y*bytes_per_line];
		alpha.get_line_pixel_to_dest(y, l);
		
		for (int x=0;x<alpha_width;x++)
		{
			if (l[x] == 0) transparent = true;
			if (l[x] != 0 && l[x] != 255) 
			{
//				std::cout << "how can this happen" << std::endl;
				transparent = true;
				has_alpha = true;
				break;
			}
		}
	}

	if (!transparent)
	{
		// This blitter will handle surfaces containing transparency/alpha only
		delete[] alpha_data;
		return false;
	}

	CL_PixelData input(
		m_card->get_target()->get_red_mask(),
		m_card->get_target()->get_green_mask(),
		m_card->get_target()->get_blue_mask(),
		m_card->get_target()->get_alpha_mask(),
		m_provider,
		(m_card->get_target()->get_depth()+7)/8);

	m_width = m_provider->get_width();
	m_height = m_provider->get_height();

	m_lines = new unsigned char*[m_num_lines];
	int input_bytes_per_line = input.get_bytes_per_line();
	unsigned char *image_data = new unsigned char[m_num_lines*input_bytes_per_line];

	for (y=0;y<m_num_lines;y++) 
	{
		m_lines[y] = NULL;

		unsigned char *line_dest = &image_data[y*input_bytes_per_line];
//		memset(line_dest, 0, input_bytes_per_line); // <- debug - is memory valid?
		input.get_line_pixel_to_dest(y, line_dest);
		
		make_line(
			m_card->get_target(),
			m_provider, 
			y,
			line_dest,
			&alpha_data[y*bytes_per_line]);
	}

	if (has_alpha)
	{
		m_contained_alpha_blitter = new CL_Blit_AlphaMask_RLE(
			m_card,
			m_provider,
			image_data,
			input_bytes_per_line,
			alpha_data,
			bytes_per_line);
	}

	delete[] alpha_data;
	delete[] image_data;

	if (!blitters->test_clip()) blitters->set_clip(this);
	if (!blitters->test_noclip()) blitters->set_noclip(this);
	if (!blitters->test_scale_noclip()) blitters->set_scale_noclip(this);
	if (!blitters->test_scale_clip()) blitters->set_scale_clip(this);

	return true;
}

void CL_Blit_Transparent::make_line(
	CL_Target *target,
	CL_SurfaceProvider *provider,
	int line_num,
	unsigned char *input_data,
	unsigned char *alpha_data)
{
	int dest_bytes_pr_pixel = target->get_depth()/8;
	cl_assert((dest_bytes_pr_pixel == 2) || (dest_bytes_pr_pixel == 4));

	unsigned char *data = new unsigned char[dest_bytes_pr_pixel*m_width*4];

	int cur_state = CL_Blit_Transparent::cmd_skip;
	int cmd_pos = 0;
	int cmd_start = 0;
	unsigned short *cmd_set_size = NULL;

	bool line_has_data = true;

	for (unsigned int x=0;x<m_width;x++)
	{
		if (alpha_data[x] == 255)
		{
			if (cur_state == CL_Blit_Transparent::cmd_skip)
			{
				unsigned short *data_ptr = (unsigned short *) (&data[cmd_pos]);
				*data_ptr = x-cmd_start;
				cmd_pos += 2;

				cmd_start = x;

				cmd_set_size = (unsigned short *) (&data[cmd_pos]);
				cmd_pos += 2;

				cur_state = CL_Blit_Transparent::cmd_copy;
			}

			if (dest_bytes_pr_pixel == 2)
			{
				*((unsigned short *) &data[cmd_pos]) = ((unsigned short *) input_data)[x];
				cmd_pos += sizeof(unsigned short);
			}
			else
			{
				*((unsigned int *) &data[cmd_pos]) = ((int *) input_data)[x];
				cmd_pos += sizeof(unsigned int);
			}
		}
		else
		{
			if (cur_state == CL_Blit_Transparent::cmd_copy)
			{
				*cmd_set_size = x - cmd_start;
				cmd_start = x;

				cur_state = CL_Blit_Transparent::cmd_skip;
			}
		}
	}
	if (cur_state == CL_Blit_Transparent::cmd_skip)
	{
		if (cmd_start == 0) line_has_data = false;

		unsigned short *data_ptr = (unsigned short *) &data[cmd_pos];
		*data_ptr = m_width-cmd_start;
		cmd_pos += 2;
	}
	else
	{
		*cmd_set_size = m_width - cmd_start;
	}

	if (line_has_data)
	{
		m_lines[line_num] = new unsigned char[cmd_pos];
		fast_memmove((char *) (m_lines[line_num]), (char *) data, cmd_pos);
	}
	else
	{
		m_lines[line_num] = NULL;
	}

	delete[] data;
}

template <class T>
void SubBlitter_Transparent<T>::blt_noclip(
	CL_Blit_Transparent *parent,
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char *) target->get_data();
	dest += x*sizeof(T) + (y-1)*dest_pitch;

//	T *dd;
	
	int cur_line = parent->m_height*spr_no;
	for (unsigned int yy=0; yy<parent->m_height; yy++)
	{
		dest += dest_pitch;
		
		unsigned char *line_data = parent->m_lines[cur_line+yy];
		if (line_data == NULL) continue;

		T *dd = (T *) dest;
		int cur_state = CL_Blit_Transparent::cmd_copy;
		
		unsigned int x = *((unsigned short *) (&line_data[0]));
		int lpos = 2;

		/* assembler acceleration -- mbn 03/99 */
		
#if !defined(WIN32) || defined(USE_TASM)
#ifdef USE_I386_ASSEMBLER
		switch (sizeof(T))
		{
		case 2:
			blit_transparent_noclip_asm_16bpp(
				line_data+lpos,
				(unsigned short *) (&dd[x]),
				parent->m_width-x);

			break;

		case 4:
			blit_transparent_noclip_asm_32bpp(
				line_data+lpos,
				(unsigned int *) (&dd[x]),
				parent->m_width-x);

			break;

		default:
#endif
#endif
			while (x<parent->m_width)
			{
				if (cur_state == CL_Blit_Transparent::cmd_copy)
				{
					unsigned short rep = *((unsigned short *) &line_data[lpos]);
					lpos += 2;

					BltRLERow_Transparent<T>::show_row(
						(T *) &(line_data[lpos]),
						&dd[x],
						rep);

					x += rep;
					lpos += sizeof(T)*rep;

					cur_state = CL_Blit_Transparent::cmd_skip;
				}
				else
				{
					unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
					unsigned short skip = *skip_pos;
					x += skip;
					lpos += 2;
					cur_state = CL_Blit_Transparent::cmd_copy;
				}
			}
#if !defined(WIN32) || defined(USE_TASM)
#ifdef USE_I386_ASSEMBLER
		}
#endif
#endif
	}
}

void CL_Blit_Transparent::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	target->lock();
	
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	switch (dest_bytes_per_pixel)
	{
	case 2:
		SubBlitter_Transparent<unsigned short>::blt_noclip(this, target, x, y, spr_no);
		break;
	case 4:
		SubBlitter_Transparent<unsigned int>::blt_noclip(this, target, x, y, spr_no);
		break;
	default:
		cl_assert(false);
		break;
	}

	target->unlock();

	if (m_contained_alpha_blitter != NULL)
	{
		m_contained_alpha_blitter->blt_noclip(target, x, y, spr_no);
	}
}

template <class T>
void SubBlitter_Transparent<T>::blt_clip(
	CL_Blit_Transparent *parent,
	CL_Target *target,
	int x,
	int y,
	int spr_no, 
	const CL_ClipRect &clip)
{
	CL_ClipRect dest_clip(x, y, x+parent->m_width, y+parent->m_height);
	CL_ClipRect clipped = clip.clip(dest_clip);
	
	if (clipped.m_x1 >= clipped.m_x2 ||
	    clipped.m_y1 >= clipped.m_y2) return;

	target->lock();

	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char *) target->get_data();
	dest += clipped.m_x1*sizeof(T) + clipped.m_y1*dest_pitch;

	int src_delta_x = clipped.m_x1-dest_clip.m_x1;
	int src_delta_y = clipped.m_y1-dest_clip.m_y1;

	unsigned int size_x = clipped.m_x2-clipped.m_x1;
	unsigned int size_y = clipped.m_y2-clipped.m_y1;

	int offset_line = parent->m_height*spr_no + src_delta_y;

	for (unsigned int yy=0; yy<size_y; yy++)
	{
		unsigned char *line_data = parent->m_lines[offset_line+yy];
		if (line_data == NULL) continue;

		T	*dd = (T *) (dest+yy*dest_pitch);

		unsigned short src_pos = *((unsigned short *) (&line_data[0]));
		int cur_state = CL_Blit_Transparent::cmd_copy;
		int lpos = 2;

		while (src_pos < src_delta_x)
		{
			if (cur_state == CL_Blit_Transparent::cmd_copy)
			{
				unsigned short rep = *((unsigned short *) &line_data[lpos]);
				lpos += 2;

				if (src_pos+rep >= src_delta_x)
				{
					unsigned short skip = src_delta_x-src_pos;
					unsigned short clip_rep = rep - skip;

					if (int(src_pos+rep) >= int(size_x+src_delta_x))
					{
						clip_rep -= (src_pos+rep)-(size_x+src_delta_x);
					}

					if (clip_rep > 0)
					{
						BltRLERow_Transparent<T>::show_row(
							(T *) &(line_data[lpos+sizeof(T)*skip]),
							dd,
							clip_rep);
					}
				}

				src_pos += rep;
				lpos += sizeof(T)*rep;

				cur_state = CL_Blit_Transparent::cmd_skip;
			}
			else
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				src_pos += skip;
				lpos += 2;
				cur_state = CL_Blit_Transparent::cmd_copy;
			}
		}

		unsigned int x = src_pos - src_delta_x;

#if !defined(WIN32) || defined(USE_TASM)
#ifdef USE_I386_ASSEMBLER
		switch (sizeof(T))
		{
		case 2:
			if (cur_state == CL_Blit_Transparent::cmd_skip)
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				x += skip;
				lpos += 2;
			}
			if (x<parent->m_width)
				blit_transparent_clip_asm_16bpp(
					line_data+lpos,
					(unsigned short *) (&dd[x]),
					size_x-x);

			break;

		case 4:
			if (cur_state == CL_Blit_Transparent::cmd_skip)
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				x += skip;
				lpos += 2;
			}
			if (x<parent->m_width)
				blit_transparent_clip_asm_32bpp(
					line_data+lpos,
					(unsigned int *) (&dd[x]),
					size_x-x);

			break;

		default:
#endif
#endif
			while (x<size_x)
			{
				if (cur_state == CL_Blit_Transparent::cmd_copy)
				{
					unsigned short rep = *((unsigned short *) &line_data[lpos]);
					lpos += 2;

					unsigned short clip_rep = rep;

					if (x+rep >= size_x)
					{
						clip_rep = size_x - x;
					}

					if (clip_rep > 0)
					{
						BltRLERow_Transparent<T>::show_row(
							(T *) &(line_data[lpos]),
							&dd[x],
							clip_rep);
					}

					x += rep;
					lpos += sizeof(T)*rep;

					cur_state = CL_Blit_Transparent::cmd_skip;
				}
				else
				{
					unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
					unsigned short skip = *skip_pos;
					x += skip;
					lpos += 2;
					cur_state = CL_Blit_Transparent::cmd_copy;
				}
			}
#if !defined(WIN32) || defined(USE_TASM)
#ifdef USE_I386_ASSEMBLER
		}
#endif
#endif
	}

	target->unlock();
}

void CL_Blit_Transparent::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect &clip)
{
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	switch (dest_bytes_per_pixel)
	{
	case 2:
		SubBlitter_Transparent<unsigned short>::blt_clip(this, target, x, y, spr_no, clip);
		break;
	case 4:
		SubBlitter_Transparent<unsigned int>::blt_clip(this, target, x, y, spr_no, clip);
		break;
	}
	if (m_contained_alpha_blitter != NULL)
	{
		m_contained_alpha_blitter->blt_clip(target, x, y, spr_no, clip);
	}
}

template <class T>
void SubBlitter_Transparent<T>::unpack_line(
	T *dest,
	unsigned char *transparent_dest,
	unsigned char *source_data,
	int num_pixels)
{
	int cur_state = CL_Blit_Transparent::cmd_skip;
	int x = 0;
	int lpos = 0;
	while (x < num_pixels)
	{
		if (cur_state == CL_Blit_Transparent::cmd_copy)
		{
			unsigned short rep = *((unsigned short *) &source_data[lpos]);
			lpos += 2;

			memset(&transparent_dest[x], 0, rep);

			for (;rep>0;rep--)
			{
				dest[x++] = *((T *) &source_data[lpos]);
				lpos += sizeof(T);
			}

			cur_state = CL_Blit_Transparent::cmd_skip;
		}
		else
		{
			unsigned short *skip_pos = (unsigned short *) (&source_data[lpos]);
			unsigned short skip = *skip_pos;
			memset(&transparent_dest[x], 1, skip);
			x += skip;
			lpos += 2;
			cur_state = CL_Blit_Transparent::cmd_copy;
		}
	}
}

template <class T>
void SubBlitter_Transparent<T>::blt_scale_noclip(
	CL_Blit_Transparent *parent,
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	if (dest_width <= 0 || dest_height <= 0) return;
	target->lock();

	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char*) target->get_data();
	dest += x*sizeof(T) + y*dest_pitch;

	unsigned int stepX = (parent->m_width<<16) / dest_width;
	unsigned int stepY = (parent->m_height<<16) / dest_height;
	unsigned int posX = 0;
	unsigned int posY = (spr_no*parent->m_height)<<16;

	T *unpacked_line = new T[parent->m_width];
	unsigned char *is_transparent = new unsigned char[parent->m_width];

	for (int yy=0; yy<dest_height; yy++)
	{
		unsigned char *line_data = parent->m_lines[posY>>16];
		if (line_data != NULL) 
		{
			unpack_line(unpacked_line, is_transparent, line_data, parent->m_width);

			T *dd = (T *) (dest+yy*dest_pitch);

			posX = 0;
			for (int xx=0;xx<dest_width;xx++)
			{
				int ix = (posX>>16);
				if (!is_transparent[ix]) 
				{
					BltRLERow_Transparent<T>::show_row(
						&unpacked_line[ix],
						&dd[xx],
						1);
				}
				posX += stepX;
			}
		}
		posY+=stepY;
	}

	delete[] unpacked_line;
	delete[] is_transparent;

	target->unlock();
}

void CL_Blit_Transparent::blt_scale_noclip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	switch (dest_bytes_per_pixel)
	{
	case 2:
			SubBlitter_Transparent<unsigned short>::blt_scale_noclip(
				this,
				target,
				x,
				y,
				dest_width,
				dest_height,
				spr_no);
		break;
	case 4:
			SubBlitter_Transparent<unsigned int>::blt_scale_noclip(
				this,
				target,
				x,
				y,
				dest_width,
				dest_height,
				spr_no);
		break;
	}
	if (m_contained_alpha_blitter != NULL)
	{
		m_contained_alpha_blitter->blt_scale_noclip(target, x, y, dest_width, dest_height, spr_no);
	}
}

template <class T>
void SubBlitter_Transparent<T>::blt_scale_clip(
	CL_Blit_Transparent *parent,
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect &clip)
{
	if (dest_width <= 0 || dest_height <= 0) return;

	CL_ClipRect dest_clip(x, y, x+dest_width, y+dest_height);
	CL_ClipRect clipped = clip.clip(dest_clip);
	
	if (clipped.m_x1 >= clipped.m_x2 ||
	    clipped.m_y1 >= clipped.m_y2) return;
	target->lock();

	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char*) target->get_data();
	dest += clipped.m_x1*dest_bytes_per_pixel + clipped.m_y1*dest_pitch;

	unsigned int stepX = (parent->m_width<<16) / dest_width;
	unsigned int stepY = (parent->m_height<<16) / dest_height;

	unsigned int clipX = stepX*(clipped.m_x1-x);
	unsigned int clipY = stepY*(clipped.m_y1-y);

	unsigned int posX = clipX;
	unsigned int posY = clipY + ((spr_no*parent->m_height) << 16);

	dest_width = clipped.m_x2-clipped.m_x1;
	dest_height = clipped.m_y2-clipped.m_y1;

	T *unpacked_line = new T[parent->m_width];
	unsigned char *is_transparent = new unsigned char[parent->m_width];

	for (int yy=0; yy<dest_height; yy++)
	{
		unsigned char *line_data = parent->m_lines[posY>>16];
		if (line_data != NULL) 
		{
			unpack_line(unpacked_line, is_transparent, line_data, parent->m_width);

			T *dd = (T *) (dest+yy*dest_pitch);

			posX = clipX;
			for (int xx=0;xx<dest_width;xx++)
			{
				int ix = (posX>>16);
				if (!is_transparent[ix]) 
				{
					BltRLERow_Transparent<T>::show_row(
						&unpacked_line[ix],
						&dd[xx],
						1);
				}
				posX += stepX;
			}
		}
		posY+=stepY;
	}

	delete[] unpacked_line;
	delete[] is_transparent;

	target->unlock();
}

void CL_Blit_Transparent::blt_scale_clip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect &clip)
{
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	switch (dest_bytes_per_pixel)
	{
	case 2:
			SubBlitter_Transparent<unsigned short>::blt_scale_clip(
				this,
				target,
				x,
				y,
				dest_width,
				dest_height,
				spr_no,
				clip);
		break;
	case 4:
			SubBlitter_Transparent<unsigned int>::blt_scale_clip(
				this,
				target,
				x,
				y,
				dest_width,
				dest_height,
				spr_no,
				clip);
		break;
	}
	if (m_contained_alpha_blitter != NULL)
	{
		m_contained_alpha_blitter->blt_scale_clip(target, x, y, dest_width, dest_height, spr_no, clip);
	}
}
