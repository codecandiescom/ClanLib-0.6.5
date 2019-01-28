/*
	$Id: blit_alphamask_rle.cpp,v 1.8 2001/11/07 02:59:18 plasmoid Exp $

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

#include "blit_alphamask_rle.h"
#include "displaycard_generic.h"
#include <API/Display/Display/surfaceprovider.h>

using namespace std;

BltRLERow_AlphaMask<unsigned short> *CL_Blit_AlphaMask_RLE::bltrow_16bit = NULL;
BltRLERow_AlphaMask<unsigned int> *CL_Blit_AlphaMask_RLE::bltrow_32bit = NULL;

CL_Blit_AlphaMask_RLE::CL_Blit_AlphaMask_RLE(
	CL_DisplayCard_Generic *card,
	CL_SurfaceProvider *provider,
	unsigned char *image_data,
	int image_bytes_pr_line,
	unsigned char *alpha_data,
	int alpha_bytes_pr_line)
{
	m_num_lines = provider->get_height()*provider->get_num_frames();
	m_width = provider->get_width();
	m_height = provider->get_height();

	m_lines = new unsigned char*[m_num_lines];
	m_alpha_lines = new unsigned char*[m_num_lines];

	for (unsigned int y=0;y<m_num_lines;y++) 
	{
		m_lines[y] = NULL;
		m_alpha_lines[y] = NULL;

		make_line(
			card->get_target(),
			provider, 
			y,
			&image_data[y*image_bytes_pr_line],
			&alpha_data[y*alpha_bytes_pr_line]);
	}
}

CL_Blit_AlphaMask_RLE::~CL_Blit_AlphaMask_RLE()
{
	for (unsigned int i=0;i<m_num_lines;i++) 
	{
		delete[] m_lines[i];
		delete[] m_alpha_lines[i];
	}
	delete[] m_lines;
	delete[] m_alpha_lines;
  if(bltrow_32bit != NULL)
  {
  	delete bltrow_32bit;
    bltrow_32bit = NULL;
  }
  if(bltrow_16bit != NULL)
  {
  	delete bltrow_16bit;
    bltrow_16bit = NULL;
  }
}

void CL_Blit_AlphaMask_RLE::make_line(
	CL_Target *target,
	CL_SurfaceProvider *provider,
	int line_num,
	unsigned char *input_data,
	unsigned char *alpha_data)
{
	int dest_bytes_pr_pixel = target->get_depth()/8;
	cl_assert((dest_bytes_pr_pixel == 2) || (dest_bytes_pr_pixel == 4));

	unsigned char *data = new unsigned char[dest_bytes_pr_pixel*m_width*4];

	int cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
	int cmd_pos = 0;
	int cmd_start = 0;
	unsigned short *cmd_set_size = NULL;

	bool line_has_data = true;

	CL_ColorMap colormap(target);

	for (unsigned int x=0;x<m_width;x++)
	{
		if (alpha_data[x] != 0 && 
			alpha_data[x] != 255)
		{
			if (cur_state == CL_Blit_AlphaMask_RLE::cmd_skip)
			{
				unsigned short *data_ptr = (unsigned short *) (&data[cmd_pos]);
				*data_ptr = x-cmd_start;
				cmd_pos += 2;

				cmd_start = x;

				cmd_set_size = (unsigned short *) (&data[cmd_pos]);
				cmd_pos += 2;

				cur_state = cmd_alpha;
			}

			if (dest_bytes_pr_pixel == 2)
			{
				PixelConstructor_AlphaMask<unsigned short>::save_pixel(
					target, 
					&colormap,
					input_data,
					&data[cmd_pos],
					alpha_data[x],
					x,
					&cmd_pos);
			}
			else
			{
				PixelConstructor_AlphaMask<unsigned int>::save_pixel(
					target, 
					&colormap,
					input_data,
					&data[cmd_pos],
					alpha_data[x],
					x,
					&cmd_pos);
			}
		}
		else
		{
			if (cur_state == cmd_alpha)
			{
				*cmd_set_size = x - cmd_start;
				cmd_start = x;

				cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
			}
		}
	}
	if (cur_state == CL_Blit_AlphaMask_RLE::cmd_skip)
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
		memcpy(m_lines[line_num], data, cmd_pos);

		int provider_width = provider->get_width();
		m_alpha_lines[line_num] = new unsigned char[provider_width];
		for (int i=0;i<provider_width;i++)
		{
			m_alpha_lines[line_num][i] = 255 - alpha_data[i];
		}
	}
	else
	{
		m_lines[line_num] = NULL;
	}

	delete[] data;
}

void CL_Blit_AlphaMask_RLE::check_16bit_rowblitter(CL_Target *target)
{
	if (bltrow_16bit == NULL)
	{
		CL_ColorMap colormap(target);

		unsigned short rmask = target->get_red_mask();
		unsigned short gmask = target->get_green_mask();
		unsigned short bmask = target->get_blue_mask();

		if (target->is_video())
		{
			bltrow_16bit = new BLTRLERow_AlphaMask_Buffered<unsigned short>(
							rmask, gmask, bmask,
							colormap.m_red_start,
							colormap.m_green_start,
							colormap.m_blue_start,
							colormap.m_red_length,
							colormap.m_green_length,
							colormap.m_blue_length);
		}
		else
		{
			bltrow_16bit = new BLTRLERow_AlphaMask_Direct<unsigned short>(
							rmask, gmask, bmask,
							colormap.m_red_start,
							colormap.m_green_start,
							colormap.m_blue_start,
							colormap.m_red_length,
							colormap.m_green_length,
							colormap.m_blue_length);
		}
	}
}

void CL_Blit_AlphaMask_RLE::check_32bit_rowblitter(CL_Target *target)
{
	if (bltrow_32bit == NULL)
	{
		CL_ColorMap colormap(target);

		unsigned int rmask = target->get_red_mask();
		unsigned int gmask = target->get_green_mask();
		unsigned int bmask = target->get_blue_mask();

		if (target->is_video())
		{
			bltrow_32bit = new BLTRLERow_AlphaMask_Buffered<unsigned int>(
							rmask, gmask, bmask,
							colormap.m_red_start,
							colormap.m_green_start,
							colormap.m_blue_start,
							colormap.m_red_length,
							colormap.m_blue_length,
							colormap.m_green_length);
		}
		else
		{
			bltrow_32bit = new BLTRLERow_AlphaMask_Direct<unsigned int>(
							rmask, gmask, bmask,
							colormap.m_red_start,
							colormap.m_green_start,
							colormap.m_blue_start,
							colormap.m_red_length,
							colormap.m_blue_length,
							colormap.m_green_length);
		}
	}
}

template <class T>
void SubBlitter_AlphaMask<T>::blt_noclip(
	CL_Blit_AlphaMask_RLE *parent,
	CL_Target *target,
	BltRLERow_AlphaMask<T> *row_blitter,
	int x,
	int y,
	int spr_no)
{
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char *) target->get_data();
	dest += x*sizeof(T) + y*dest_pitch;

	int cur_line = parent->m_height*spr_no;
	for (unsigned int yy=0; yy<parent->m_height; ++yy)
	{
		unsigned char *line_data = parent->m_lines[cur_line+yy];
		if (line_data == NULL) continue;

		unsigned char *alpha_data = parent->m_alpha_lines[cur_line+yy];

		T *dd = (T *) (dest+yy*dest_pitch);
		int cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
		
		unsigned int x = *((unsigned short *) (&line_data[0]));
		int lpos = 2;

		while (x<parent->m_width)
		{
			if (cur_state == CL_Blit_AlphaMask_RLE::cmd_alpha)
			{
				unsigned short rep = *((unsigned short *) &line_data[lpos]);
				lpos += 2;

				row_blitter->show_alpha_row(
					(T *) &(line_data[lpos]),
					&dd[x],
					&alpha_data[x],
					rep);

				x += rep;
				lpos += sizeof(T)*rep;

				cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
			}
			else
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				x += skip;
				lpos += 2;
				cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
			}
		}
	}
}

void CL_Blit_AlphaMask_RLE::blt_noclip(
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
		check_16bit_rowblitter(target);
		SubBlitter_AlphaMask<unsigned short>::blt_noclip(this, target, bltrow_16bit, x, y, spr_no);
		break;
	case 4:
		check_32bit_rowblitter(target);
		SubBlitter_AlphaMask<unsigned int>::blt_noclip(this, target, bltrow_32bit, x, y, spr_no);
		break;
	default:
		cl_assert(false);
	}

	target->unlock();
}

template <class T>
void SubBlitter_AlphaMask<T>::blt_clip(
	CL_Blit_AlphaMask_RLE *parent,
	CL_Target *target,
	BltRLERow_AlphaMask<T> *row_blitter,
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

		unsigned char	*alpha_data = parent->m_alpha_lines[offset_line+yy];
		T	*dd = (T *) (dest+yy*dest_pitch);

		unsigned short src_pos = *((unsigned short *) (&line_data[0]));
		int cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
		int lpos = 2;

		while (src_pos < src_delta_x)
		{
			if (cur_state == CL_Blit_AlphaMask_RLE::cmd_alpha)
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
						row_blitter->show_alpha_row(
							(T *) &(line_data[lpos+sizeof(T)*skip]),
							dd,
							&alpha_data[src_pos+skip],
							clip_rep);
					}
				}

				src_pos += rep;
				lpos += sizeof(T)*rep;

				cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
			}
			else
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				src_pos += skip;
				lpos += 2;
				cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
			}
		}

		unsigned int x = src_pos - src_delta_x;

		while (x<size_x)
		{
			if (cur_state == CL_Blit_AlphaMask_RLE::cmd_alpha)
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
					row_blitter->show_alpha_row(
						(T *) &(line_data[lpos]),
						&dd[x],
						&alpha_data[src_delta_x+x],
						clip_rep);
				}

				x += rep;
				lpos += sizeof(T)*rep;

				cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
			}
			else
			{
				unsigned short *skip_pos = (unsigned short *) (&line_data[lpos]);
				unsigned short skip = *skip_pos;
				x += skip;
				lpos += 2;
				cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
			}
		}
	}

	target->unlock();
}

void CL_Blit_AlphaMask_RLE::blt_clip(
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
		check_16bit_rowblitter(target);
		SubBlitter_AlphaMask<unsigned short>::blt_clip(this, target, bltrow_16bit, x, y, spr_no, clip);
		break;
	case 4:
		check_32bit_rowblitter(target);
		SubBlitter_AlphaMask<unsigned int>::blt_clip(this, target, bltrow_32bit, x, y, spr_no, clip);
		break;
	}
}

template <class T>
void SubBlitter_AlphaMask<T>::unpack_line(
	T *dest,
	unsigned char *transparent_dest,
	unsigned char *source_data,
	int num_pixels)
{
	int cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
	int x = 0;
	int lpos = 0;
	while (x < num_pixels)
	{
		if (cur_state == CL_Blit_AlphaMask_RLE::cmd_alpha)
		{
			unsigned short rep = *((unsigned short *) &source_data[lpos]);
			lpos += 2;

			memset(&transparent_dest[x], 0, rep);

			for (;rep>0;rep--)
			{
				dest[x++] = *((T *) &source_data[lpos]);
				lpos += sizeof(T);
			}

			cur_state = CL_Blit_AlphaMask_RLE::cmd_skip;
		}
		else
		{
			unsigned short *skip_pos = (unsigned short *) (&source_data[lpos]);
			unsigned short skip = *skip_pos;
			memset(&transparent_dest[x], 1, skip);
			x += skip;
			lpos += 2;
			cur_state = CL_Blit_AlphaMask_RLE::cmd_alpha;
		}
	}
}

template <class T>
void SubBlitter_AlphaMask<T>::blt_scale_noclip(
	CL_Blit_AlphaMask_RLE *parent,
	CL_Target *target,
	BltRLERow_AlphaMask<T> *row_blitter,
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
	unsigned int posY = 0;

	T *unpacked_line = new T[parent->m_width];
	unsigned char *is_transparent = new unsigned char[parent->m_width];

	for (int yy=0; yy<dest_height; yy++)
	{
		unsigned char *line_data = parent->m_lines[posY>>16];
		if (line_data != NULL) 
		{
			unsigned char *alpha_data = parent->m_alpha_lines[posY>>16];

			unpack_line(unpacked_line, is_transparent, line_data, parent->m_width);

			T *dd = (T *) (dest+yy*dest_pitch);

			posX = 0;
			for (int xx=0;xx<dest_width;xx++)
			{
				int ix = (posX>>16);
				if (!is_transparent[ix]) 
				{
					row_blitter->show_alpha_row(
						&unpacked_line[ix],
						&dd[xx],
						&alpha_data[ix],
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

void CL_Blit_AlphaMask_RLE::blt_scale_noclip(
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
			check_16bit_rowblitter(target);
			SubBlitter_AlphaMask<unsigned short>::blt_scale_noclip(
				this,
				target,
				bltrow_16bit,
				x,
				y,
				dest_width,
				dest_height,
				spr_no);
		break;
	case 4:
			check_32bit_rowblitter(target);
			SubBlitter_AlphaMask<unsigned int>::blt_scale_noclip(
				this,
				target,
				bltrow_32bit,
				x,
				y,
				dest_width,
				dest_height,
				spr_no);
		break;
	}
}

template <class T>
void SubBlitter_AlphaMask<T>::blt_scale_clip(
	CL_Blit_AlphaMask_RLE *parent,
	CL_Target *target,
	BltRLERow_AlphaMask<T> *row_blitter,
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

	unsigned int dest_pitch = target->get_pitch();

	unsigned char *dest = (unsigned char*) target->get_data();
	dest += clipped.m_x1*sizeof(T) + clipped.m_y1*dest_pitch;

	unsigned int stepX = (parent->m_width<<16) / dest_width;
	unsigned int stepY = (parent->m_height<<16) / dest_height;

	unsigned int clipX = stepX*(clipped.m_x1-x);
	unsigned int clipY = stepY*(clipped.m_y1-y);

	unsigned int posX = clipX;
	unsigned int posY = clipY;

	dest_width = clipped.m_x2-clipped.m_x1;
	dest_height = clipped.m_y2-clipped.m_y1;

	T *unpacked_line = new T[parent->m_width];
	unsigned char *is_transparent = new unsigned char[parent->m_width];

	for (int yy=0; yy<dest_height; yy++)
	{
		unsigned char *line_data = parent->m_lines[posY>>16];
		if (line_data != NULL) 
		{
			unsigned char *alpha_data = parent->m_alpha_lines[posY>>16];

			unpack_line(unpacked_line, is_transparent, line_data, parent->m_width);

			T *dd = (T *) (dest+yy*dest_pitch);

			posX = clipX;
			for (int xx=0;xx<dest_width;xx++)
			{
				int ix = (posX>>16);
				if (!is_transparent[ix]) 
				{
					row_blitter->show_alpha_row(
						&unpacked_line[ix],
						&dd[xx],
						&alpha_data[ix],
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

void CL_Blit_AlphaMask_RLE::blt_scale_clip(
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
			check_16bit_rowblitter(target);
			SubBlitter_AlphaMask<unsigned short>::blt_scale_clip(
				this,
				target,
				bltrow_16bit,
				x,
				y,
				dest_width,
				dest_height,
				spr_no,
				clip);
		break;
	case 4:
			check_32bit_rowblitter(target);
			SubBlitter_AlphaMask<unsigned int>::blt_scale_clip(
				this,
				target,
				bltrow_32bit,
				x,
				y,
				dest_width,
				dest_height,
				spr_no,
				clip);
		break;
	}
}
