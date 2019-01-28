/*
	$Id: blit_opaque.cpp,v 1.2 2001/04/12 19:58:17 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Display/Display/surfaceprovider.h"
#include <API/Display/Display/pixeldata.h>

#include <Display/Display/Generic/blit_opaque.h>
#ifdef __BEOS__
	#include "Core/System/Generic/string_asm.h"
#else
	#define fast_memmove memcpy
#endif

CL_Blit_Opaque::CL_Blit_Opaque(
	CL_SurfaceProvider *provider,
	int bytes_per_pixel,
	CL_Target *target)
{
	width = provider->get_width();
	height = provider->get_height();
	no_sprs = provider->get_num_frames();
	image_pitch = width*bytes_per_pixel;
	image_bytes_per_pixel = bytes_per_pixel;
	image = new unsigned char[width*height*no_sprs*bytes_per_pixel];

	CL_PixelData input(
		target->get_red_mask(),
		target->get_green_mask(),
		target->get_blue_mask(),
		target->get_alpha_mask(),
		provider,
		bytes_per_pixel);

	for (unsigned int y=0; y<height*no_sprs; y++)
		fast_memmove(
			(char *) &image[y*image_pitch],
			(char *) input.get_line_pixel(y),
			image_pitch);
}

CL_Blit_Opaque::~CL_Blit_Opaque()
{
	delete[] image;
}

void CL_Blit_Opaque::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	target->lock();
	
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *src = image;
	unsigned char *dest = (unsigned char *) target->get_data();

	src += image_pitch*height*spr_no;
	dest += x*dest_bytes_per_pixel + y*dest_pitch;

	for (unsigned int yy=0; yy<height; yy++)
	{
		fast_memmove((char *) dest, (char *) src, image_pitch);

		src += image_pitch;
		dest += dest_pitch;
	}

	target->unlock();
}

void CL_Blit_Opaque::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect & clip)
{
	CL_ClipRect dest_clip(x, y, x+width, y+height);
	CL_ClipRect clipped = clip.clip(dest_clip);
	
	if (clipped.m_x1 >= clipped.m_x2 ||
	    clipped.m_y1 >= clipped.m_y2) return;

	int t_width = target->get_width();
	int t_height = target->get_height();

	if( clipped.m_x2 < 0 || clipped.m_x1 > t_width ||
	    clipped.m_y2 < 0 || clipped.m_y1 > t_height ) return;

	target->lock();

	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *src = image;
	unsigned char *dest = (unsigned char *) target->get_data();

	src += image_pitch*height*spr_no;
	src += (clipped.m_x1-dest_clip.m_x1)*dest_bytes_per_pixel +
	       (clipped.m_y1-dest_clip.m_y1)*image_pitch;
	dest += clipped.m_x1*dest_bytes_per_pixel + clipped.m_y1*dest_pitch;

	if ( clipped.m_x1 < 0 ) dest = (unsigned char *) target->get_data()
  									+ clipped.m_y1*dest_pitch;

	if ( clipped.m_y1 < 0 && clipped.m_x1 >= 0 )
	{
		src = image;
		src += image_pitch*height*spr_no;
		src += (-y + clipped.m_y1) * image_pitch;
		src += ( -clipped.m_y1 * image_pitch ) + ( (clipped.m_x1-x) * image_bytes_per_pixel );
		dest = (unsigned char *) target->get_data() + clipped.m_x1*dest_bytes_per_pixel;
	}
	else if ( clipped.m_y1 < 0 && clipped.m_x1 < 0 )
	{
		src = image;
		src += image_pitch*height*spr_no;
		src += (-y + clipped.m_y1) * image_pitch;
		src += -clipped.m_y1 * image_pitch;
		src += (-x + clipped.m_x1) * image_bytes_per_pixel;
		dest = (unsigned char *) target->get_data();
	}

	int rows = clipped.m_y2 - clipped.m_y1;
	if( rows > clipped.m_y2 ) rows = clipped.m_y2;
	if( rows + clipped.m_y1 > t_height )
		rows = t_height - clipped.m_y1;

	for (int yy=0; yy<rows; yy++)
	{
		if ( clipped.m_x1 >= 0 && clipped.m_x2 <= t_width ) // line fits on target
		{
			fast_memmove((char *) dest, (char *) src, sizeof(unsigned char) * (clipped.m_x2 - clipped.m_x1) * dest_bytes_per_pixel );
		}
		else if ( clipped.m_x1 > 0 && clipped.m_x2 > t_width ) // Cut end.
		{
			short length = t_width - clipped.m_x1;
			fast_memmove((char *) dest, (char *) src, sizeof(unsigned char) * length * dest_bytes_per_pixel );
		}
		else if ( clipped.m_x1 < 0 && clipped.m_x2 > t_width ) // Cut both ends.
		{
			short length_begin = -clipped.m_x1;
			fast_memmove( (char *) dest, (char *)(src+ (length_begin * dest_bytes_per_pixel)),
						 (sizeof(unsigned char) * t_width * image_bytes_per_pixel) );
		}
 		else  // cut beginning of line.
		{
			short length_begin = -clipped.m_x1;
			fast_memmove( (char *) dest, (char *)(src + (length_begin * dest_bytes_per_pixel)),
						 (sizeof(unsigned char) * (clipped.m_x2) * dest_bytes_per_pixel) );
		}
		
		src += image_pitch;
		dest += dest_pitch;
	}
	target->unlock();
}

void CL_Blit_Opaque::blt_scale_noclip(
	CL_Target *target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no)
{
	if (dest_width <= 0 || dest_height <= 0) return;
	target->lock();

	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *_dest = (unsigned char*) target->get_data();
	_dest += x*dest_bytes_per_pixel + y*dest_pitch;

	unsigned int stepX = (width<<16) / dest_width;
	unsigned int stepY = (height<<16) / dest_height;
	unsigned int posX = 0;
	unsigned int posY = 0;

	switch(dest_bytes_per_pixel)
	{
	case 1:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned char *src = (unsigned char *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned char *dest = (unsigned char *) (_dest+yy*dest_pitch);

				posX=0;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;
	
	case 2:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned short *src = (unsigned short *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned short *dest = (unsigned short *) (_dest+yy*dest_pitch);

				posX=0;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;

	case 4:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned int *src = (unsigned int *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned int *dest = (unsigned int *) (_dest+yy*dest_pitch);

				posX=0;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;

	default:
		cl_assert(false); // unsupported pixel depth!
	}
	target->unlock();
}

void CL_Blit_Opaque::blt_scale_clip(
	CL_Target * target,
	int x,
	int y,
	int dest_width,
	int dest_height,
	int spr_no,
	const CL_ClipRect & clip)
{
	if (dest_width <= 0 || dest_height <= 0) return;

	CL_ClipRect dest_clip(x, y, x+dest_width, y+dest_height);
	CL_ClipRect clipped = clip.clip(dest_clip);
	
	if (clipped.m_x1 >= clipped.m_x2 ||
	    clipped.m_y1 >= clipped.m_y2) return;
	target->lock();

	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();

	unsigned char *_dest = (unsigned char*) target->get_data();
	_dest += clipped.m_x1*dest_bytes_per_pixel + clipped.m_y1*dest_pitch;

	unsigned int stepX = (width<<16) / dest_width;
	unsigned int stepY = (height<<16) / dest_height;

	unsigned int clipX = stepX*(clipped.m_x1-x);
	unsigned int clipY = stepY*(clipped.m_y1-y);

	unsigned int posX = clipX;
	unsigned int posY = clipY;

	dest_width = clipped.m_x2-clipped.m_x1;
	dest_height = clipped.m_y2-clipped.m_y1;

	switch(dest_bytes_per_pixel)
	{
	case 1:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned char *src = (unsigned char *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned char *dest = (unsigned char *) (_dest+yy*dest_pitch);

				posX=clipX;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;
	
	case 2:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned short *src = (unsigned short *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned short *dest = (unsigned short *) (_dest+yy*dest_pitch);

				posX=clipX;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;

	case 4:
		{
			for (int yy=0; yy<dest_height; yy++)
			{
				unsigned int *src = (unsigned int *) image;
				src += (spr_no*height+(posY>>16))*width;

				unsigned int *dest = (unsigned int *) (_dest+yy*dest_pitch);

				posX=clipX;
				for (int xx=0; xx<dest_width; xx++)
				{
					dest[xx] = src[(posX>>16)];
					posX+=stepX;
				}
				posY+=stepY;
			}
		}
		break;

	default:
		cl_assert(false); // unsupported pixel depth!
	}
	
	target->unlock();
}
