/*
	$Id: target_generic.cpp,v 1.9 2002/02/18 22:47:37 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	See http://www.clanlib.org
*/

#include "Core/precomp.h"

#include <API/Display/Display/cliprect.h>
#include <API/Display/Display/target.h>
#include <API/Display/Display/pixelformat.h>
#include <API/Display/Display/palette.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/IOData/cl_endian.h>
#include <Display/Display/Generic/colormap.h>

using namespace std;

#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0 :(-1)))
#define ABS(x) ((x)>0 ? (x) : (-x))

inline void fast_draw_pixel_32bpp(
	unsigned char *data,
	int x,
	int y,
	int color,
	unsigned int pitch);

inline void fast_draw_pixel_24bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned char *color,
	unsigned int pitch);

inline void fast_draw_pixel_16bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned short color,
	unsigned int pitch);

inline void fast_draw_pixel_8bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned char color,
	unsigned int pitch);

int clip_line(int &x1,int &y1,int &x2, int &y2, int min_clip_x, int max_clip_x, int min_clip_y, int max_clip_y);

void cl_fill_rect_opaque(
	CL_Target *target, int _x1, int _y1, int _x2, int _y2, int color);

void CL_Target::push_clip_rect()
{
	if (clip_stack.empty())
		clip_stack.push(
			CL_ClipRect(0, 0, get_width(), get_height()));

	clip_stack.push(clip_stack.top());
}

void CL_Target::push_clip_rect(const CL_ClipRect &rect)
{
	push_clip_rect();
	clip_stack.top() = clip_stack.top().clip(rect);
}

CL_ClipRect CL_Target::get_clip_rect()
{
	if (clip_stack.empty())
		clip_stack.push(
			CL_ClipRect(0, 0, get_width(), get_height()));

	return clip_stack.top();
}

void CL_Target::set_clip_rect(const CL_ClipRect &rect)
{
	if (clip_stack.empty())
		clip_stack.push(rect);
	else
		clip_stack.top() = rect;
}

void CL_Target::pop_clip_rect()
{
	cl_assert_debug(clip_stack.empty() == false);
	clip_stack.pop();
}

void CL_Target::push_translate_offset() 
{ 
	m_translation_stack.push(m_translation_stack.top()); 
}

void CL_Target::push_translate_offset(int x, int y) 
{ 
	TranslationOffset cur = m_translation_stack.top(); 
	m_translation_stack.push(TranslationOffset(cur.first+x, cur.second+y)); 
}

int  CL_Target::get_translate_offset_x() const
{ 
	return m_translation_stack.top().first; 
}

int  CL_Target::get_translate_offset_y() const
{ 
	return m_translation_stack.top().second; 
}

void CL_Target::set_translate_offset(int x, int y) 
{ 
	m_translation_stack.top().first = x; m_translation_stack.top().second = y; 
}

void CL_Target::pop_translate_offset() 
{ 
	m_translation_stack.pop(); if (m_translation_stack.empty()) m_translation_stack.push(TranslationOffset(0,0)); 
}

void CL_Target::draw_pixel(int x, int y, int color)
{
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x += trans_x;
	y += trans_y;


	CL_ClipRect clip = get_clip_rect();
	if ((x < clip.m_x1) ||
	    (x >= clip.m_x2) ||
	    (y < clip.m_y1) ||
	    (y >= clip.m_y2)) return;

	lock();

	unsigned char* data = (unsigned char*) get_data();
	
	switch (get_bytes_per_pixel())
	{
		case	1 :	
			{
				unsigned char *d = data + y * get_pitch() + x;
				*d = (unsigned char) color;
				break;
			}
		case	2 :
			{
				unsigned short *d = (unsigned short*) (data + y * get_pitch() + x*2);
				*d = (unsigned short) color;
				break;
			}
		case	3 :
			{
				// that should do the trick - untested !!!
				unsigned char *d = data + y * get_pitch() + x*3;
				*(d++) = (unsigned char) color;
				color >>= 8;
				*(d++) = (unsigned char) color;
				color >>= 8;
				*d = (unsigned char) color;
				break;
			}
		case	4 :
			{	
				unsigned int *d = (unsigned int*) (data + y * get_pitch() + x*4);
				*d = (unsigned int) color;
				break;
			}
		default:
			{
				cl_assert(false);
				break;
			}
	}
	unlock();
}

void CL_Target::flip_vertical()
{
	lock();


  unsigned char* data = (unsigned char*) get_data();

	int byte_pp = get_bytes_per_pixel();
	int h = get_height();
	int w = get_width();

  unsigned char *buf = new unsigned char[byte_pp];


  for(int j = 0; j < h; j++)
  {
  	for(int i = 0; i < w/2; i++)
  	{
  		memcpy(buf, (data + i*byte_pp), byte_pp);
    	memcpy((data + i*byte_pp), (data + (w-i-1)*byte_pp), byte_pp);
    	memcpy((data + (w-i-1)*byte_pp), buf, byte_pp);
  	}
    data += w*byte_pp;
  }

	unlock();

}

void CL_Target::flip_horizontal()
{
	lock();

	unsigned char* data = (unsigned char*) get_data();

	int byte_pp = get_bytes_per_pixel();
	int h = get_height();
	int w = get_width();
	
	unsigned char* temp = new unsigned char[w*byte_pp];
	
  for(int i = 0; i < h/2 ; i++)
  {
  	memcpy(temp, data + i*w*byte_pp, w* byte_pp);
    memcpy(data + i*w*byte_pp, data + (h-i-1)*w*byte_pp, w*byte_pp);
    memcpy(data + (h-i-1)*w*byte_pp, temp, w* byte_pp);
  }
  
	unlock();
}


void CL_Target::get_pixel(int x, int y, float *r, float *g, float *b, float *a)
{
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x += trans_x;
	y += trans_y;


	lock();
	unsigned char* data = (unsigned char*) get_data();
	int color = 0;
	
	switch (get_bytes_per_pixel())
	{
		case	1 :	
			{
				unsigned char *d = data + y * get_pitch() + x;
				color = *d;
				break;
			}
		case	2 :
			{
				unsigned short *d = (unsigned short*) (data + y * get_pitch() + x*2);
				color = *d;
				break;
			}
		case	3 :
			{			
				// that should do the trick - untested !!!
				unsigned char *d = data + y * get_pitch() + x*3;
#ifdef USE_BIG_ENDIAN
				color = (*d << 16) | (*(d+1) << 8) | (*(d+2));
#else
				color = (*d) | (*(d+1) << 8) | (*(d+2) << 16);
#endif
    				break;
			}
		case	4 :
			{	
				unsigned int *d = (unsigned int*) (data + y * get_pitch() + x*4);
				color = *d;
				break;
			}
		default:
			{
				cl_assert(false);
				break;
			}
	}
	
	if (is_indexed())
	{
		*r = get_palette()->palette[color*3 +0] / 255.0f;
		*g = get_palette()->palette[color*3 +1] / 255.0f;
		*b = get_palette()->palette[color*3 +2] / 255.0f;
		*a = 1.0f;
	}
	else
	{
		unsigned int red_mask = get_red_mask();
		unsigned int green_mask = get_green_mask();
		unsigned int blue_mask = get_blue_mask();
		unsigned int alpha_mask = get_alpha_mask();

		*r = (color & red_mask) / float (red_mask);
		*g = (color & green_mask) / float (green_mask);
		*b = (color & blue_mask) / float (blue_mask);
		
		// if we have an alphamask, use it
		if (alpha_mask)
		{
			*a = (color & alpha_mask) / float (alpha_mask);
		}
		else
		{
			//else we have an alpha of 1.0f (don't divide by 0 alpha!)
			*a = 1.0f;
		}
	}
	unlock();
}


int CL_Target::get_pixel(int x, int y)
{
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x += trans_x;
	y += trans_y;


	lock();
	unsigned char* data = (unsigned char*) get_data();
	int color = 0;
	
	switch (get_bytes_per_pixel())
	{
		case	1 :	
			{
				unsigned char* d = data + y * get_pitch() + x;
				color = *d;
				break;
			}
		case	2 :
			{
				unsigned short* d = (unsigned short*) (data + y * get_pitch() + x*2);
				color = *d;
				break;
			}
		case	3 :
			{			
				// that should do the trick - untested !!!
				unsigned char* d = data + y * get_pitch() + x*3;
#ifdef USE_BIG_ENDIAN
				color = (*d << 16) | (*(d+1) << 8) | (*(d+2));
#else
				color = (*d) | (*(d+1) << 8) | (*(d+2) << 16);
#endif
				break;
			}
		case	4 :
			{	
				unsigned int* d = (unsigned int*) (data + y * get_pitch() + x*4);
				color = *d;
				break;
			}
		default:
			{
				cl_assert(false);
				break;
			}
	}
	unlock();
	return color;
}

void CL_Target::draw_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	if (a <= 0.01) return;

	// Using fill_rect is faster than draw_line because draw_line calls
	// fill_rect anyways if it is a straight line, which it always is.
	fill_rect(x1, y1, x2, y1 + 1, r, g, b, a); // Top
	fill_rect(x1, y1 + 1, x1 + 1, y2 - 1, r, g, b, a); // Left
	fill_rect(x2 - 1, y1 + 1, x2, y2 - 1, r, g, b, a); // Right
	fill_rect(x1, y2 - 1, x2, y2, r, g, b, a); // Bottom
}

void CL_Target::fill_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x1 += trans_x;
	y1 += trans_y;
	x2 += trans_x;
	y2 += trans_y;

	int color = CL_Color::get_color(this, r,g,b,a);

	// this could/ should get optimised, but the main focus now is to get it working again ;-)

	if (a == 1)
	{
		cl_fill_rect_opaque(this, x1, y1, x2, y2, color);
		return;
	}

	CL_ClipRect rect(x1, y1, x2, y2);
	if (get_clip_rect().test_all_clipped(rect)) return;

	CL_ClipRect crect = get_clip_rect().clip(rect);

	lock();

	CL_ColorMap cmap(this);

	unsigned int fill_color = cmap.calc_color(r, g, b, a);

	unsigned int red_mask = get_red_mask();
	unsigned int green_mask = get_green_mask();
	unsigned int blue_mask = get_blue_mask();
	unsigned int alpha_mask = get_alpha_mask();
	
	{
		unsigned int rr = fill_color & red_mask;
		unsigned int gg = fill_color & green_mask;
		unsigned int bb = fill_color & blue_mask;

		rr >>= cmap.m_red_start;
		gg >>= cmap.m_green_start;
		bb >>= cmap.m_blue_start;
	
		rr = (int) (rr*a);
		gg = (int) (gg*a);
		bb = (int) (bb*a);

		fill_color =
			(rr<<cmap.m_red_start) +
			(gg<<cmap.m_green_start) +
			(bb<<cmap.m_blue_start);
	}

	unsigned int dest_bytes_per_pixel = (get_depth()+7)/8;
	unsigned int dest_pitch = get_pitch();

	unsigned char *dest = (unsigned char*) get_data();
	dest += crect.m_x1*dest_bytes_per_pixel + crect.m_y1*dest_pitch;

	unsigned int delta_x = crect.m_x2-crect.m_x1;
	unsigned int delta_y = crect.m_y2-crect.m_y1;

	int rr_start = cmap.m_red_start;
	int gg_start = cmap.m_green_start;
	int bb_start = cmap.m_blue_start;

	switch (dest_bytes_per_pixel)
	{
	case 2:
		{
			int i;
			int rl = 1 << cmap.m_red_length;
			int gl = 1 << cmap.m_green_length;
			int bl = 1 << cmap.m_blue_length;
	
			unsigned short *rr_lookup = new unsigned short[rl];
			unsigned short *gg_lookup = new unsigned short[gl];
			unsigned short *bb_lookup = new unsigned short[bl];
						
			for (i=0;i<rl;i++)
			{
				rr_lookup[i] = (unsigned short)(i*(1-a)) << rr_start;
			}
			for (i=0;i<gl;i++)
			{
				gg_lookup[i] = (unsigned short)(i*(1-a)) << gg_start;
			}
			for (i=0;i<bl;i++)
			{
				bb_lookup[i] = (unsigned short)(i*(1-a)) << bb_start;
			}

			int skip = dest_pitch - delta_x*2;
			unsigned short clear_value = (unsigned short) fill_color;

			if (is_video())
			{
				unsigned short *data = new unsigned short[delta_x];
				for (unsigned int yy=0; yy<delta_y; yy++)
				{
					memcpy(data, dest, delta_x*2);
					for (unsigned int xx=0;xx<delta_x;xx++)
					{
						unsigned short dval = data[xx];
						unsigned int rr = dval & red_mask;
						unsigned int gg = dval & green_mask;
						unsigned int bb = dval & blue_mask;
						rr >>= rr_start;
						gg >>= gg_start;
						bb >>= bb_start;
						dval =
							rr_lookup[rr] +
							gg_lookup[gg] +
							bb_lookup[bb] +
							clear_value;

						data[xx] = dval;
					}
					memcpy(dest, data, delta_x*2);
					dest += delta_x*2;
					dest += skip;
				}
				delete [] data;
			}
			else
			{
				for (unsigned int yy=0; yy<delta_y; yy++)
				{
					for (unsigned int xx=0;xx<delta_x;xx++)
					{
						unsigned short dval = *((unsigned short *)dest+xx);
						register unsigned int rr = dval & red_mask;
						unsigned int gg = dval & green_mask;
						unsigned int bb = dval & blue_mask;
						rr >>= rr_start;
						gg >>= gg_start;
						bb >>= bb_start;

						dval =
							rr_lookup[rr] +
							gg_lookup[gg] +
							bb_lookup[bb] +
							clear_value;

						*((unsigned short *)dest+xx) = dval;
					}
					dest += delta_x*2;
					dest += skip;
				}
			}
			delete [] rr_lookup;
			delete [] gg_lookup;
			delete [] bb_lookup;
		}
		break;

	case 4:
		{
			int i;
			int rl = 1 << cmap.m_red_length;
			int gl = 1 << cmap.m_green_length;
			int bl = 1 << cmap.m_blue_length;
	
			unsigned int *rr_lookup = new unsigned int[rl];
			unsigned int *gg_lookup = new unsigned int[gl];
			unsigned int *bb_lookup = new unsigned int[bl];

			for (i=0;i<rl;i++)
			{
				rr_lookup[i] = (unsigned int)(i*(1-a)) << rr_start;
			}
			for (i=0;i<gl;i++)
			{
				gg_lookup[i] = (unsigned int)(i*(1-a)) << gg_start;
			}
			for (i=0;i<bl;i++)
			{
				bb_lookup[i] = (unsigned int)(i*(1-a)) << bb_start;
			}

			int skip = dest_pitch - delta_x*4;

			if (is_video())
			{
				unsigned int* data = new unsigned int[delta_x];
				for (unsigned int yy=0; yy<delta_y; yy++)
				{
					memcpy(data, dest, delta_x*4);
					for (unsigned int xx=0;xx<delta_x;xx++)
					{
						unsigned int dval = data[xx];
						unsigned int rr = dval & red_mask;
						unsigned int gg = dval & green_mask;
						unsigned int bb = dval & blue_mask;
						unsigned int aa = dval & alpha_mask;
						rr >>= rr_start;
						gg >>= gg_start;
						bb >>= bb_start;

						dval =
							rr_lookup[rr] +
							gg_lookup[gg] +
							bb_lookup[bb] +
							fill_color;

						data[xx] = dval | aa;
					}
					memcpy(dest, data, delta_x*4);
					dest += delta_x*4;
					dest += skip;
				}
				delete [] data;
			}
			else
			{
				for (unsigned int yy=0; yy<delta_y; yy++)
				{
					for (unsigned int xx=0;xx<delta_x;xx++)
					{
						unsigned int dval = *((unsigned int *) dest);
						unsigned int rr = dval & red_mask;
						unsigned int gg = dval & green_mask;
						unsigned int bb = dval & blue_mask;
						unsigned int aa = dval & alpha_mask;
						rr >>= rr_start;
						gg >>= gg_start;
						bb >>= bb_start;

						dval =
							rr_lookup[rr] +
							gg_lookup[gg] +
							bb_lookup[bb] +
							fill_color;

						*((unsigned int *) dest) = dval | aa;

						dest += 4;
					}
					dest += skip;
				}
			}
			delete [] rr_lookup;
			delete [] gg_lookup;
			delete [] bb_lookup;
		}
		break;

	default:
		cl_assert(false); // unsupported pixel depth!
		break;
	}

	unlock();
}

void cl_fill_rect_opaque(
	CL_Target *target,
	int _x1,
	int _y1,
	int _x2,
	int _y2,
	int color)
{
	CL_ClipRect rect(_x1, _y1, _x2, _y2);

	if (target->get_clip_rect().test_all_clipped(rect)) return;

	CL_ClipRect crect = target->get_clip_rect().clip(rect);

	int x1 = crect.m_x1;
	int x2 = crect.m_x2;
	int y1 = crect.m_y1;
	int y2 = crect.m_y2;

	target->lock();

	unsigned int dest_pitch = target->get_pitch();
	unsigned int dest_bytes_per_pixel = target->get_bytes_per_pixel();

	register unsigned char *dest = (unsigned char*) target->get_data(); //This variable is hit hard later in the function
	dest += x1*dest_bytes_per_pixel + y1*dest_pitch;

	unsigned int delta_x = x2-x1;
	unsigned int delta_y = y2-y1;

	switch (dest_bytes_per_pixel)
	{
	case 1:
		{
			int skip = dest_pitch - delta_x;
			if (skip == 0)
			{
    				memset(dest, (char) color, delta_x*delta_y);
			
			} else
			{
				for (unsigned int y = 0; y < delta_y; y++)
				{
					memset(dest, (char) color, delta_x);
					dest += skip + delta_x;
				}
			}
		}
	
	case 2:
		{
			register unsigned short _color = (unsigned short) color;
			int skip = dest_pitch - delta_x * 2;
			// the blitter probes if it can perform a memset (when hi and lo byte of _color are equal)
			if ((_color & 0x00FF) != (_color & 0xFF00))
			{
				for (unsigned int y = 0; y < delta_y; y++)
				{
					for (unsigned int x=0; x < delta_x; x++)
					{
						*((unsigned short *) dest) = _color;
						dest += 2;
					}
					dest += skip;
				}
			}
			else
			{
				char color8 = (char)_color;
				if (skip != 0)
				{
					for (unsigned int y = 0; y < delta_y; y++)
					{
						memset(dest, color8, 2 * delta_x);
						dest += skip + 2 * delta_x;
					}
				}
				else // everything can be done with one single memset :-)
				{
					memset(dest, color8, 2 * delta_x * delta_y);
				}
			}
		}
		break;

	case 3:
		{
			int skip = dest_pitch - delta_x * 3;
			for (unsigned int y = 0; y < delta_y; y++)
			{
				for (unsigned int x = 0; x < delta_x; x++)
				{
					*(dest++) = color & 0x0000FF;
					*(dest++) = color & 0x00FF00;
					*(dest++) = color & 0xFF0000;
				}
				dest += skip;
			}
		}

	case 4:
		{
			int skip = dest_pitch - delta_x * 4;
			for (unsigned int y = 0; y < delta_y; y++)
			{
				for (unsigned int x = 0; x < delta_x; x++)
				{
					*((unsigned int *) dest) = color;
					dest += 4;
				}
				dest += skip;
			}
		}
		break;

	default:
		cl_assert(false); // unsupported pixel depth!
		break;
	}

	target->unlock();
}

// Used for draw_line function
inline void fast_draw_pixel_32bpp(
	unsigned char *data,
	int x,
	int y,
	int color,
	unsigned int pitch)
{
	unsigned int *d = (unsigned int*) (data + y * pitch + (x<<2));
	*d = (unsigned int) color;
	
	return;
}

// Used for draw_line function
inline void fast_draw_pixel_24bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned char *color,
	unsigned int pitch)
{
	unsigned char *d = data + y * pitch + x*3;
	memcpy(d,color,3);
	return;
}

// Used for draw_line function
inline void fast_draw_pixel_16bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned short color,
	unsigned int pitch)
{
	unsigned short *d ;
  d = (unsigned short*) (data + y * pitch + (x<<1));
	*d = color;

	return;
}

// Used for draw_line function
inline void fast_draw_pixel_8bpp(
	unsigned char *data,
	int x,
	int y,
	unsigned char color,
	unsigned int pitch)
{

	unsigned char *d = data + y * pitch + x;
  *d = color;
	return;
}

void CL_Target::draw_line(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	if (y1 == y2 || x1 == x2)
	{
		CL_Target::fill_rect(x1, y1, x2+1, y2+1, r, g, b, a);
		return;
	}

	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x1 += trans_x;
	y1 += trans_y;
	x2 += trans_x;
	y2 += trans_y;


	int         color_32bpp  = CL_Color::get_color(this, r,g,b,a);
	CL_ClipRect clip         = get_clip_rect();
	int         pitch        = get_pitch();

	if (!clip_line(x1,y1,x2,y2,clip.m_x1,clip.m_x2-1,clip.m_y1,clip.m_y2-1))
	{
		// line is totally out of clip area
		return;
	}
	
 	lock();
	
	unsigned char* data = (unsigned char*) get_data();

	int lg_delta, sh_delta, cycle, lg_step, sh_step;

	lg_delta = x2 - x1;
	sh_delta = y2 - y1;
	lg_step = SGN(lg_delta);
	lg_delta = ABS(lg_delta);
	sh_step = SGN(sh_delta);
	sh_delta = ABS(sh_delta);

	switch (get_bytes_per_pixel())
	{
		//*****************************************************************
		// Draw line in 8bpp mode
		case	1 :	
			{
				unsigned char	color_8bpp = (unsigned char) color_32bpp;
				if (sh_delta < lg_delta)
				{
					cycle = lg_delta >> 1;
					while (x1 != x2)
					{
						fast_draw_pixel_8bpp(data,x1,y1,color_8bpp,pitch);
						cycle += sh_delta;
						if (cycle > lg_delta)
						{
							cycle -= lg_delta;
							y1 += sh_step;
						}
						x1 += lg_step;
					}
					fast_draw_pixel_8bpp(data,x1,y1,color_8bpp,pitch);
				}
				cycle = sh_delta >> 1;
				while (y1 != y2)
				{
					fast_draw_pixel_8bpp(data,x1,y1,color_8bpp,pitch);
					cycle += lg_delta;
					if (cycle > sh_delta)
					{
						cycle -= sh_delta;
						x1 += lg_step;
					}
					y1 += sh_step;
				}
				fast_draw_pixel_8bpp(data,x1,y1,color_8bpp,pitch);
				break;

			}

		//*****************************************************************
		// Draw line in 16bpp mode
		case	2 :
			{
				unsigned short color_16bpp = (unsigned short) color_32bpp;
				if (sh_delta < lg_delta)
				{
					cycle = lg_delta >> 1;
					while (x1 != x2)
					{
						fast_draw_pixel_16bpp(data,x1,y1,color_16bpp,pitch);
						cycle += sh_delta;
						if (cycle > lg_delta)
						{
							cycle -= lg_delta;
							y1 += sh_step;
						}
						x1 += lg_step;
					}
					fast_draw_pixel_16bpp(data,x1,y1,color_16bpp,pitch);
				}
				cycle = sh_delta >> 1;
				while (y1 != y2)
				{
					fast_draw_pixel_16bpp(data,x1,y1,color_16bpp,pitch);
					cycle += lg_delta;
					if (cycle > sh_delta)
					{
						cycle -= sh_delta;
						x1 += lg_step;
					}
					y1 += sh_step;
				}
				fast_draw_pixel_16bpp(data,x1,y1,color_16bpp,pitch);
				break;
			}

		//*****************************************************************
		// Draw line in 24bpp mode
		case	3 :
			{
				unsigned char	color_24bpp[3] = {0,0,0};
				if (sh_delta < lg_delta)
				{
					cycle = lg_delta >> 1;
					while (x1 != x2)
					{
						fast_draw_pixel_24bpp(data,x1,y1,color_24bpp,pitch);
						cycle += sh_delta;
						if (cycle > lg_delta)
						{
							cycle -= lg_delta;
							y1 += sh_step;
						}
						x1 += lg_step;
					}
					fast_draw_pixel_24bpp(data,x1,y1,color_24bpp,pitch);
				}
				cycle = sh_delta >> 1;
				while (y1 != y2)
				{
					fast_draw_pixel_24bpp(data,x1,y1,color_24bpp,pitch);
					cycle += lg_delta;
					if (cycle > sh_delta)
					{
						cycle -= sh_delta;
						x1 += lg_step;
					}
					y1 += sh_step;
				}
				fast_draw_pixel_24bpp(data,x1,y1,color_24bpp,pitch);
				break;
			}
			
		//*****************************************************************
		// Draw line in 32bpp mode
		case	4 :
			{	
				if (sh_delta < lg_delta)
				{
					cycle = lg_delta >> 1;
					while (x1 != x2)
					{
						fast_draw_pixel_32bpp(data,x1,y1,color_32bpp,pitch);
						cycle += sh_delta;
						if (cycle > lg_delta)
						{
							cycle -= lg_delta;
							y1 += sh_step;
						}
						x1 += lg_step;
					}
					fast_draw_pixel_32bpp(data,x1,y1,color_32bpp,pitch);
				}
				cycle = sh_delta >> 1;
				while (y1 != y2)
				{
					fast_draw_pixel_32bpp(data,x1,y1,color_32bpp,pitch);
					cycle += lg_delta;
					if (cycle > sh_delta)
					{
						cycle -= sh_delta;
						x1 += lg_step;
					}
					y1 += sh_step;
				}
				fast_draw_pixel_32bpp(data,x1,y1,color_32bpp,pitch);
				break;
			}
		default:
			{
				cl_assert(false);
				break;
			}
	}

	unlock();
}


// This function was ripped from "Tricks of the Windows game programming gurus" 
// written by Andre Lamothe ISBN:0-672-31361-8
// The function was slightly adapted to work with clanlib
// it returns 0 if the line is totally outside the clipping area and 1 if the
// line was successfully clipped
int clip_line(int &x1,int &y1,int &x2, int &y2, int min_clip_x, int max_clip_x, int min_clip_y, int max_clip_y)
{

// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009
#define CLIP_CODE_SW 0x0005

	int xc1=x1;
	int yc1=y1;
	int xc2=x2;
	int yc2=y2;

	int p1_code=0;
	int p2_code=0;

	// determine codes for p1 and p2
	if (y1 < min_clip_y)
	{
		p1_code|=CLIP_CODE_N;
	}
	else if (y1 > max_clip_y)
	{
		p1_code|=CLIP_CODE_S;
	}

	if (x1 < min_clip_x)
	{
		p1_code|=CLIP_CODE_W;
	}
	else if (x1 > max_clip_x)
	{
		p1_code|=CLIP_CODE_E;
	}

	if (y2 < min_clip_y)
	{
		p2_code|=CLIP_CODE_N;
	}
	else if (y2 > max_clip_y)
	{
		p2_code|=CLIP_CODE_S;
	}

	if (x2 < min_clip_x)
	{
		p2_code|=CLIP_CODE_W;
	}
	else if (x2 > max_clip_x)
	{
		p2_code|=CLIP_CODE_E;
	}

	// try and trivially reject
	if ((p1_code & p2_code))
	{
		return(0);
	}

	// test for totally visible, if so leave points untouched
	if (p1_code==0 && p2_code==0)
	{
		return(1);
	}

	// determine end clip point for p1
	switch(p1_code)
	{
	case CLIP_CODE_C: 
		break;

	case CLIP_CODE_N:
		{
			yc1 = (int) min_clip_y;
			xc1 = (int) (x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1));
		} 
		break;

	case CLIP_CODE_S:
		{
			yc1 = (int) max_clip_y;
			xc1 = (int) (x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));
		} 
		break;

	case CLIP_CODE_W:
		{
			xc1 = (int) min_clip_x;
			yc1 = (int) (y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));
		} 
		break;
		
	case CLIP_CODE_E:
		{
			xc1 = (int) max_clip_x;
			yc1 = (int) (y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
		} 
		break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		{
			// north hline intersection
			yc1 = (int) min_clip_y;
			xc1 = (int) (x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1));

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				// east vline intersection
				xc1 = (int) max_clip_x;
				yc1 = (int) (y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
			} // end if

		} 
		break;
	
	case CLIP_CODE_SE:
		{
			// south hline intersection
			yc1 = (int) max_clip_y;
			xc1 = (int) (x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));	

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				// east vline intersection
				xc1 = (int) max_clip_x;
				yc1 = (int) (y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1));
			} // end if

		} 
		break;
	
	case CLIP_CODE_NW:
		{
			// north hline intersection
			yc1 = (int) min_clip_y;
			xc1 = (int) (x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1));
			
			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				xc1 = (int) min_clip_x;
				yc1 = (int) (y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));	
			} // end if

		} 
		break;
	  	
	case CLIP_CODE_SW:
		{
			// south hline intersection
			yc1 = (int) max_clip_y;
			xc1 = (int) (x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1));	

			// test if intersection is valid, of so then done, else compute next
			if (xc1 < min_clip_x || xc1 > max_clip_x)
			{
				xc1 = (int) min_clip_x;
				yc1 = (int) (y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1));	
			} // end if
		}
		break;

	default:
		break;

	} // end switch

	// determine clip point for p2
	switch(p2_code)
	{
	case CLIP_CODE_C: 
		break;

	case CLIP_CODE_N:
		{
			yc2 = (int) min_clip_y;
			xc2 = (int) (x2 + (min_clip_y-y2)*(x1-x2)/(y1-y2));
		} 
		break;

	case CLIP_CODE_S:
		{
			yc2 = (int) max_clip_y;
			xc2 = (int) (x2 + (max_clip_y-y2)*(x1-x2)/(y1-y2));
		} 
		break;

	case CLIP_CODE_W:
		{
			xc2 = (int) min_clip_x;
			yc2 = (int) (y2 + (min_clip_x-x2)*(y1-y2)/(x1-x2));
		} 
		break;

	case CLIP_CODE_E:
		{
			xc2 = (int) max_clip_x;
			yc2 = (int) (y2 + (max_clip_x-x2)*(y1-y2)/(x1-x2));
		} 
		break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		{
		// north hline intersection
		yc2 = (int) min_clip_y;
		xc2 = (int) (x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2));

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x)
		{
			// east vline intersection
			xc2 = (int) max_clip_x;
			yc2 = (int) (y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2));
		} // end if

		} 
		break;

	case CLIP_CODE_SE:
		{
			// south hline intersection
			yc2 = (int) max_clip_y;
			xc2 = (int) (x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2));	

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				// east vline intersection
				xc2 = (int) max_clip_x;
				yc2 = (int) (y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2));
			} // end if
		} 
		break;

	case CLIP_CODE_NW:
		{
			// north hline intersection
			yc2 = (int) min_clip_y;
			xc2 = (int) (x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2));

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				xc2 = (int) min_clip_x;
				yc2 = (int) (y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2));	
			} // end if
		} 
		break;

	case CLIP_CODE_SW:
		{
			// south hline intersection
			yc2 = (int) max_clip_y;
			xc2 = (int) (x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2));	

			// test if intersection is valid, of so then done, else compute next
			if (xc2 < min_clip_x || xc2 > max_clip_x)
			{
				xc2 = (int) min_clip_x;
				yc2 = (int) (y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2));	
			} // end if

		} 
		break;

	default:
		break;

	} // end switch

	// do bounds check
	if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
		(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
		(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
		(yc2 < min_clip_y) || (yc2 > max_clip_y) )
	{
			return(0);
	} // end if

	// store vars back
	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return(1);
}
