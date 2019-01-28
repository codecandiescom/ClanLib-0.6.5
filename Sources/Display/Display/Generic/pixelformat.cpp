/*
	$Id: pixelformat.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/


#include "Core/precomp.h"
#include "API/Display/Display/target.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/IOData/cl_endian.h"

unsigned int CL_Color::get_red_mask(EPixelFormat pixelformat)
{
	if (CL_Endian::is_system_big())
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 31 );
			case RGBA4444:	return ( 15 );
			case RGBA5551:	return ( 31 );
			case RGB888:	return ( 255 );
			case RGBA8888:	return ( 255 );
			case PAL8: break;
		}
	}
	else
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 31 << (6+5) );
			case RGBA4444:	return ( 15 << (4+4+4) );
			case RGBA5551:	return ( 31 << (5+5+1) );
			case RGB888:	return ( 255 << (8+8) );
			case RGBA8888:	return ( 255 << (8+8+8) );
			case PAL8: break;
		}
	}
	return 0;
}
			
unsigned int CL_Color::get_green_mask(EPixelFormat pixelformat)
{
	if (CL_Endian::is_system_big())
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 63 << (5) );
			case RGBA4444:	return ( 15 << (4) );
			case RGBA5551:	return ( 31 << (5) );
			case RGB888:	return ( 255 << (8) );
			case RGBA8888:	return ( 255 << (8) );
			case PAL8: break;
		}
	}
	else
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 63 << (5) );
			case RGBA4444:	return ( 15 << (4+4) );
			case RGBA5551:	return ( 31 << (5+1) );
			case RGB888:	return ( 255 << (8) );
			case RGBA8888:	return ( 255 << (8+8) );
			case PAL8: break;
		}
	}
	return 0;
}

unsigned int CL_Color::get_blue_mask(EPixelFormat pixelformat)
{
	if (CL_Endian::is_system_big())
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 31 << (5+6) );
			case RGBA4444:	return ( 15 << (4+4) );
			case RGBA5551:	return ( 31 << (5+5) );
			case RGB888:	return ( 255 << (8+8) );
			case RGBA8888:	return ( 255 << (8+8) );
			case PAL8: break;
		}
	}
	else
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 31 );
			case RGBA4444:	return ( 15 << (4) );
			case RGBA5551:	return ( 31 << (1) );
			case RGB888:	return ( 255 );
			case RGBA8888:	return ( 255 << (8) );
			case PAL8: break;
		}
	}
	return 0;
}

unsigned int CL_Color::get_alpha_mask(EPixelFormat pixelformat)
{
	if (CL_Endian::is_system_big())
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 0 );
			case RGBA4444:	return ( 15 << (4+4+4) );
			case RGBA5551:	return ( 1 << (5+5+5) );
			case RGB888:	return ( 0 );
			case RGBA8888:	return ( 255 << (8+8+8) );
			case PAL8: break;
		}
	}
	else
	{
		switch(pixelformat)
		{
			case RGB565:	return ( 0 );
			case RGBA4444:	return ( 15 );
			case RGBA5551:	return ( 1 );
			case RGB888:	return ( 0 );
			case RGBA8888:	return ( 255 );
			case PAL8: break;
		}
	}
	return 0;
}	

unsigned int CL_Color::get_color(EPixelFormat fmt, int r, int g, int b, int a)
{
	return CL_Color::get_color(CL_Color::get_red_mask(fmt),
		CL_Color::get_green_mask(fmt),
		CL_Color::get_blue_mask(fmt),
		CL_Color::get_alpha_mask(fmt),
		r,g,b,a);
}

unsigned int CL_Color::get_color(
	EPixelFormat fmt,
	float r,
	float g,
	float b,
	float a)
{
	return CL_Color::get_color(
		CL_Color::get_red_mask(fmt),
		CL_Color::get_green_mask(fmt),
		CL_Color::get_blue_mask(fmt),
		CL_Color::get_alpha_mask(fmt),
		(int) (255*r), 
		(int) (255*g),
		(int) (255*b),
		(int) (255*a));
}

unsigned int CL_Color::get_color(
	unsigned int rmask,
	unsigned int gmask,
	unsigned int bmask,
	unsigned int amask, 
	float r, float g, float b, float a)	     
{
	return CL_Color::get_color(
		rmask,
		gmask,
		bmask,
		amask,
		(int) (255*r), 
		(int) (255*g),
		(int) (255*b),
		(int) (255*a));
}

unsigned int CL_Color::get_color(
	unsigned int rmask,
	unsigned int gmask,
	unsigned int bmask,
	unsigned int amask, 
	int r, int g, int b, int a)	     
{
	// here comes the tricky part ... hmhmhm... Quicky Mart
	unsigned int rshift, gshift, bshift, ashift, tmp;
	unsigned int rtmp, gtmp, btmp, atmp;
	
	rshift = 32; tmp = rmask;
	while (tmp!=0)
	{
		tmp = (tmp << 1); rshift--;
	}
	
	gshift = 32; tmp = gmask;
	while (tmp!=0)
	{
		tmp = (tmp << 1); gshift--;
	}
	bshift = 32; tmp = bmask;
	while (tmp!=0)
	{
		tmp = (tmp << 1); bshift--;
	}
	
	ashift = 32; tmp = amask;
	while (tmp!=0)
	{
		tmp = (tmp << 1); ashift--;
	}

	rtmp = rmask >> rshift;
	gtmp = gmask >> gshift;
	btmp = bmask >> bshift;
	atmp = amask >> ashift;            
	
	tmp = (((unsigned int) (r/255.0 * rtmp)) << rshift) |
	      (((unsigned int) (g/255.0 * gtmp)) << gshift) |
          (((unsigned int) (b/255.0 * btmp)) << bshift) |
          (((unsigned int) (a/255.0 * atmp)) << ashift);

	return tmp; 
}

unsigned int CL_Color::get_color(
	CL_Target* target,
	int r, int g, int b, int a)
{
	return get_color(
		target->get_red_mask(), 
		target->get_green_mask(),
		target->get_blue_mask(),
		target->get_alpha_mask(),
		r,g,b,a);
}

unsigned int CL_Color::get_color(
	CL_Target* target,
	float r, float g, float b, float a)
{
	return get_color(
		target->get_red_mask(), 
		target->get_green_mask(),
		target->get_blue_mask(),
		target->get_alpha_mask(),
		(int) (255*r), 
		(int) (255*g),
		(int) (255*b),
		(int) (255*a));
}


float CL_Color::get_color(unsigned int mask, int color)
{
	unsigned int tmp = color & mask;	
	unsigned int shift = 32;

	while (tmp != 0) 
	{
		tmp = tmp << 1;
		shift --;
	}      

	return ((float) (color & mask)) / ((float) (mask >> shift));
}
