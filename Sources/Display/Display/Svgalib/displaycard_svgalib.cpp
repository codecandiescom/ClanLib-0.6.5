/*
	$Id: displaycard_svgalib.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_SVGALIB

#include <vga.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/Svgalib/displaycard_svgalib.h>
#include <API/Core/System/error.h>
#include <API/Display/Display/palette.h>
#include <Display/Display/Generic/target_system.h>
#include <API/Display/Display/vidmode.h>
#include "API/Core/System/cl_assert.h"

CL_DisplayCard_Svgalib::CL_DisplayCard_Svgalib(int _card_no) :
	CL_DisplayCard_Generic(_card_no)
{
	m_initialized = true;
	m_target = NULL;
}

CL_DisplayCard_Svgalib::~CL_DisplayCard_Svgalib()
{
	if (m_initialized) vga_setmode(TEXT);
	delete m_target;
}

void CL_DisplayCard_Svgalib::put_display(const class CL_Rect &rect)
{
	cl_assert(false); // not implemented.
}

void CL_DisplayCard_Svgalib::flip_display(bool)
{
	m_target->lock();

	unsigned char *backbuffer = (unsigned char *) m_target->get_data();
	unsigned int pitch = m_target->get_pitch();
	int height = m_target->get_height();

	for (int line=0; line<height; line++)
		vga_drawscanline(
			line,
			backbuffer+line*pitch);

	m_target->unlock();
}

void CL_DisplayCard_Svgalib::set_palette(CL_Palette *palette)
{
	m_palette = new CL_Palette(palette->palette, palette->num_colors);

	for (int i=0; i<256; i++)
	{
		vga_setpalette(
			i,
			palette->palette[i*3+0]/4,
			palette->palette[i*3+1]/4,
			palette->palette[i*3+2]/4);
	}
}

CL_Palette *CL_DisplayCard_Svgalib::get_palette()
{
/*	for (int i=0; i<256; i++)
	{
		int r,g,b;
		vga_getpalette(i, &r, &g, &b);

		system_palette.palette[i*3+0] = r;
		system_palette.palette[i*3+1] = g;
		system_palette.palette[i*3+2] = b;
	}
*/
	return m_palette;
}

void CL_DisplayCard_Svgalib::set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory)
{
	int num_modes = vga_lastmodenumber();

	for (int i=0; i<num_modes; i++)
	{
		vga_modeinfo *cur_mode = vga_getmodeinfo(i);

		if (cur_mode->width == width &&
			cur_mode->height == height &&
			cur_mode->bytesperpixel == (bpp+7)/8)
		{
			delete m_target;
			m_target = NULL;

			set_gfxmode(width, height, bpp, fullscreen, allow_resize);

			vga_setmode(i);
			m_initialized = true;
			
			// this may look like the strangest way to find the correct masks,
			// but this is how the vgatest application does!
			int red_mask=0, green_mask=0, blue_mask=0;
			switch (cur_mode->colors)
			{
			case 256:
				// 8 bpp not supported in this version of ClanLib.
				cl_assert(false);
				break;
			
			case 1 << 15: // 555 RGB, blue at LSB
				red_mask = 31 << (5+5);
				green_mask = 31 << 5;
				blue_mask = 31;
				break;
			
			case 1 << 16: // 565 RGB, blue at LSB
				red_mask =  31 << (6+5);
				green_mask = 63 << 5;
				blue_mask = 31;
				break;
			
			case 1 << 24: // 888 RGB or BGR
				if (cur_mode->bytesperpixel == 3)
				{
					if (cur_mode->flags & RGB_MISORDERED) // BGR
					{
						blue_mask = 255 << (8+8);
						green_mask = 255 << 8;
						red_mask = 255;
					}
					else
					{
						red_mask = 255 << (8+8);
						green_mask = 255 << 8;
						blue_mask = 255;
					}
				}
				else
				{
					if (cur_mode->flags & RGB_MISORDERED) // RGBX
					{
						red_mask = 255 << (8+8+8);
						green_mask = 255 << (8+8);
						blue_mask = 255 << 8;
					}
					else // XRGB
					{
						red_mask = 255 << (8+8);
						green_mask = 255 << 8;
						blue_mask = 255;
					}
				}
				break;
			
			default:
				// unknown bit depth!
				cl_assert(false);
			}

			m_target = new CL_Target_System(
				width,
				height,
				(bpp+7)/8,
				red_mask,
				green_mask,
				blue_mask,
				0); // alpha mask

			return;
		}
	}

	throw CL_Error("Video mode not available");
}

bool CL_DisplayCard_Svgalib::is_initialized()
{
	return m_initialized;
}

const std::list<CL_VidMode*> &CL_DisplayCard_Svgalib::get_videomodes()
{
//	std::cout << "TODO: Make CL_DisplayCard_Svgalib::get_videmodes()" << std::endl;
	static std::list<CL_VidMode*> dummy;
	return dummy;
}

#endif
