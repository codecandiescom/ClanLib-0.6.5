
/*
	$Id: display_fbdev.cpp,v 1.4 2001/10/14 19:59:11 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	TODO:
		-Only mmap mmio if there is a Matrox
*/

#include "Core/precomp.h"
#include <Display/Display/FBDev/blit_mga.h>
#include <Display/Display/Generic/blitters.h>
#include <Display/Display/Generic/colormap.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <API/Core/System/error.h>
#include <Display/Display/FBDev/display_fbdev.h>
#include <API/Display/Display/palette.h>
#include <API/Display/Display/surfaceprovider.h>
#include <Display/Display/FBDev/target_fbdev.h>
#include <API/Display/Display/vidmode.h>
#include <API/Display/Display/mousecursor.h>
#include "Core/System/Unix/appconf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <linux/fb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <math.h>


#include "regs.h"
#include "mmio.h"

CL_FBDev_DisplayCard::CL_FBDev_DisplayCard(int card_no) : CL_DisplayCard_Generic(card_no)
{
	FileConfig config("clanlib");
	fb_try_accel = config.readEntry("fb_try_accel", "yes");

	m_initialized = false;
	m_mmioaddr = NULL;

	m_palette = new CL_Palette();

	m_target = NULL;
	fb_mem = NULL;
	
	fd = open("/dev/fb0", O_RDWR);
	if (fd<0)
	{
		throw CL_Error("FB dev: Couldn't open /dev/fb0, you must have read-write access!");
	}
}

CL_FBDev_DisplayCard::~CL_FBDev_DisplayCard()
{
	CL_MouseCursor::hide();
	if (m_initialized)
	{
		// clear out the framebuffer
		memset((void*)fb_mem,0,fb_fix.smem_len);

		delete m_target;
		ioctl(fd,FBIOPUT_VSCREENINFO,&fb_var_orig);
	}
	
	close(fd);

	delete m_palette;
}

CL_Blitters CL_FBDev_DisplayCard::create_hw_blitters(CL_SurfaceProvider *provider)
{
	CL_Blitters ret;
        
	if (m_mmioaddr)
	{
		CL_Blit_MGA *blitter = new CL_Blit_MGA(this, provider, m_mmioaddr);
		if (! blitter->init_surface(&ret) )
		{
			delete blitter;
		}
	}

	return ret;
}
                                                                        
void CL_FBDev_DisplayCard::flip_display(bool sync)
{
	cl_assert(m_initialized); // card not initialized!

	signal_preflip();

	m_target->put_image();

	if (doublebuffer)
	{
		fb_var.yoffset = yoffset;
		yoffset = (yoffset == get_height()) ? 0 : get_height();

		if (m_mmioaddr)
		{
			mga_waitidle(m_mmioaddr);
		}

		ioctl( fd, FBIOPAN_DISPLAY, &fb_var);

		set_card_clip();
	}

	signal_postflip();
}

void CL_FBDev_DisplayCard::put_display(const class CL_Rect &rect)
{
	cl_assert(m_initialized);

	signal_preflip();

	// not implemented for this target yet. Someone with fbdev knowledge, please do so. :)
	// -- mbn 21. nov 2000
	cl_assert(false);

	signal_postflip();
}

void CL_FBDev_DisplayCard::set_palette(CL_Palette *pal)
{
	m_palette = new CL_Palette(pal->palette);
}

CL_Palette *CL_FBDev_DisplayCard::get_palette()
{
	return m_palette;
}

void CL_FBDev_DisplayCard::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	if (m_initialized) // clean up
	{
		delete m_target; m_target = NULL;
	}
	
	set_gfxmode(width, height, bpp, fullscreen, allow_resize);

	// fetch initial settings
	ioctl( fd, FBIOGET_VSCREENINFO, &fb_var_orig );
	ioctl( fd, FBIOGET_FSCREENINFO, &fb_fix );
	fb_var = fb_var_orig;


	// try to set resolution
	fb_var.activate = FB_ACTIVATE_NOW;
	fb_var.accel_flags = 0;
	fb_var.xres = get_width();
	fb_var.yres = get_height();
	fb_var.xres_virtual = get_width();
	fb_var.yres_virtual = get_height()*2;
	fb_var.bits_per_pixel = bpp;
	if (-1 == ioctl(fd,FBIOPUT_VSCREENINFO,&fb_var))
	{
		doublebuffer = false;
		fb_var.yres_virtual = get_height();
		if (-1 == ioctl(fd,FBIOPUT_VSCREENINFO,&fb_var))
		{
			// Damn: Could not set proper video mode.
			fb_var = fb_var_orig;
			
			// Test if we can use the current one (is it big enough?)
			if ((int)fb_var.xres < get_width() || (int)fb_var.yres < get_height() || (int)fb_var.bits_per_pixel != bpp)
			{
				char s[80];	// Don't diss me, tell me why it's so hard to fill a STL string; wanted to use streams like with CL_String!
				printf( s, "FBDev: Couldn't set proper video mode, need %dx%dx%d!", get_width(), get_height(), bpp );
				throw CL_Error( s );
			}
		}
	} else
	{
		doublebuffer = true;
		std::cout << "Using doublebuffer mode (panning)." << std::endl;
		yoffset = get_height();
	}

	/* move viewport to upper left corner */
	fb_var.xoffset = 0;
	fb_var.yoffset = 0;
	ioctl(fd,FBIOPAN_DISPLAY,&fb_var);
	
	// map framebuffer and get the address
	if ( MAP_FAILED == (fb_mem = (unsigned char *) mmap(	NULL, 
						fb_fix.smem_len, 
						PROT_READ | PROT_WRITE, 
						MAP_SHARED,
						fd, 0 )) )
	{
		throw CL_Error("FB dev: Couldn't mmap framebuffer");
	}
	

	if ( fb_try_accel == "yes"  &&  fb_var.accel_flags == 0 )
	{
		m_mmioaddr = (unsigned char *) mmap(NULL, fb_fix.mmio_len,
						PROT_READ | PROT_WRITE, MAP_SHARED,
						fd, fb_fix.smem_len);

		if (m_mmioaddr == MAP_FAILED  ||  m_mmioaddr == NULL)
		{
			std::cout << "No acceleration available." << std::endl;
			m_mmioaddr = NULL;
		}
		else
		{
			std::cout << "Yeah, using acceleration!" << std::endl;
			switch( fb_fix.accel )
			{
#ifdef FB_ACCEL_MATROX_MGAG400
                                case FB_ACCEL_MATROX_MGAG400:
#endif
				case FB_ACCEL_MATROX_MGAG200:
					m_accel_alpha_rect = true;
					m_accel_scale_blit = true;
					std::cout << "Using accelerated alpha rectangles." << std::endl;
					std::cout << "Using accelerated scaled blits." << std::endl;
					break;
				default:
					m_accel_alpha_rect = false;
					m_accel_scale_blit = false;
					break;
			}
		}
	} else
	{
		std::cout << "Not trying to use acceleration." << std::endl;
	}

	// clear out the framebuffer
	memset((void*)fb_mem,0,fb_fix.smem_len);

	m_target = new CL_Target_FBDev( doublebuffer, (char*)fb_mem, get_width(), get_height(), &fb_var, m_mmioaddr );

	m_initialized = true;
}

unsigned int CL_FBDev_DisplayCard::get_virt_width()
{
	return fb_var.xres_virtual;
}

unsigned char* CL_FBDev_DisplayCard::get_offscreen_mem()
{
	if (!m_mmioaddr)
	{
		return NULL;
	}

	return fb_mem + m_target->get_pitch()*get_height()*2;
}

bool CL_FBDev_DisplayCard::is_initialized()
{
	return m_initialized;
}

const std::list<CL_VidMode*> &CL_FBDev_DisplayCard::get_videomodes()
{
	cl_assert(false); // not implemented yet!
	static std::list<CL_VidMode*> ret;
	return ret;
}

void CL_FBDev_DisplayCard::draw_rect(
        int x1,
        int y1,
        int x2,
        int y2,
        float r,
        float g,
        float b,
        float a)
{
	throw CL_Error("draw_rect not implemented in fbdev, please tell us to do that");
}

void CL_FBDev_DisplayCard::fill_rect(
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
	if (!m_mmioaddr)
	{
		CL_DisplayCard_Generic::fill_rect(x1, y1, x2, y2, r, g, b, a);
		return;
	}
	if (a <= 0.99 && !m_accel_alpha_rect)
	{
		CL_DisplayCard_Generic::fill_rect(x1, y1, x2, y2, r, g, b, a);
		return;
	}
	
	// Translate coords into position:
	int trans_x = get_translate_offset_x();
	int trans_y = get_translate_offset_y();
	x1 += trans_x;
	y1 += trans_y;
	x2 += trans_x;
	y2 += trans_y;

	int y = y1 + yoffset;
	int h = y2 - y1;

	if (a <= 0.99)
	{
		/* This one is for alpha rectangles */

		mga_waitfifo( m_mmioaddr, 3 );
		mga_out32( m_mmioaddr, SRC_ALPHA | DST_ONE_MINUS_SRC_ALPHA | ALPHACHANNEL, ALPHACTRL );
		mga_out32( m_mmioaddr, 0, ALPHAXINC );
		mga_out32( m_mmioaddr, 0, ALPHAYINC );

		mga_waitfifo( m_mmioaddr, 3 );		// red start and increments
		mga_out32( m_mmioaddr, FNORM_TO_F0915(r), DR4 );
		mga_out32( m_mmioaddr, 0, DR6 );
		mga_out32( m_mmioaddr, 0, DR7 );

		mga_waitfifo( m_mmioaddr, 3 );		// green start and increments
		mga_out32( m_mmioaddr, FNORM_TO_F0915(g), DR8 );
		mga_out32( m_mmioaddr, 0, DR10 );
		mga_out32( m_mmioaddr, 0, DR11 );

		mga_waitfifo( m_mmioaddr, 3 );		// blue start and increments
		mga_out32( m_mmioaddr, FNORM_TO_F0915(b), DR12 );
		mga_out32( m_mmioaddr, 0, DR14 );
		mga_out32( m_mmioaddr, 0, DR15 );

		mga_waitfifo( m_mmioaddr, 5 );
		mga_out32( m_mmioaddr, BLTMOD_BFCOL | BOP_COPY | SHFTZERO | SGNZERO | ARZERO | OP_TRAP | ATYPE_I, DWGCTL );
		mga_out32( m_mmioaddr, FNORM_TO_F0915(a), ALPHASTART );
		mga_out32( m_mmioaddr, (x2 << 16) | x1, FXBNDRY );
		mga_out32( m_mmioaddr, (y << 16) | h, YDSTLEN | EXECUTE );

		mga_out32( m_mmioaddr, 1, ALPHACTRL );	// turn off alpha blending
	} else
	{
		/* This one is for solid rectangles */

		CL_ColorMap cmap(get_target());
		unsigned int fill_color = cmap.calc_color(r, g, b, a);

		switch( fb_var.bits_per_pixel )
		{
			case 8:
				fill_color &= 0xFF;
				fill_color |= (fill_color<<8) | (fill_color<<16) | (fill_color<<24);
			break;
			case 16:
				fill_color &= 0xFFFF;
				fill_color |= (fill_color<<16);
			break;
		}

		mga_waitfifo( m_mmioaddr, 4 );
		mga_out32( m_mmioaddr, fill_color, FCOL );
		mga_out32( m_mmioaddr, (x2 << 16) | x1, FXBNDRY );
		mga_out32( m_mmioaddr, (y << 16) | h, YDSTLEN );
		mga_out32( m_mmioaddr, BOP_COPY | SHFTZERO | SGNZERO | ARZERO | SOLID | OP_TRAP | ATYPE_BLK, DWGCTL | EXECUTE );
	}
}

void CL_FBDev_DisplayCard::set_card_clip()
{
	if (!m_mmioaddr)
		return;

	int topy = m_cur_clip.m_y1 + yoffset;
	int boty = (m_cur_clip.m_y2-1) + yoffset;

	mga_waitfifo(m_mmioaddr, 3);
	mga_out32(m_mmioaddr, (m_cur_clip.m_x1 & 0x07FF) | (((m_cur_clip.m_x2-1) & 0x07FF) << 16), CXBNDRY);
	mga_out32(m_mmioaddr, (fb_var.xres_virtual * topy) & 0x00FFFFFF, YTOP);
	mga_out32(m_mmioaddr, (fb_var.xres_virtual * boty) & 0x00FFFFFF, YBOT);
}
