/*
	$Id: display_ggi.cpp,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI


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

#include <API/Display/Display/mousecursor.h>
#include <API/Display/Display/cliprect.h>
#include <API/Display/Display/palette.h>
#include <API/Display/Display/vidmode.h>
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <Display/Display/Generic/colormap.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/GGI/display_ggi.h>
#include <Display/Display/GGI/target_ggi.h>
#include <Display/Display/GGI/target_ggi_db.h>
#include <Display/Display/GGI/target_ggi_put.h>

CL_GGI_DisplayCard::CL_GGI_DisplayCard(int card_no) : CL_DisplayCard_Generic(card_no)
{
	m_initialized = false;

	m_palette = new CL_Palette();

	m_target = m_target2 = NULL;

	if (ggiInit())
	{
		throw CL_Error( "CL_GGI_DisplayCard: Cannot initialize libGGI" );
	}

	vis = ggiOpen(NULL);
	if (!vis)
	{
		ggiExit();
		throw CL_Error( "CL_GGI_DisplayCard: Cannot open default visual, check GGI_DISPLAY env." );
	}
}

CL_GGI_DisplayCard::~CL_GGI_DisplayCard()
{
	CL_MouseCursor::hide();
	if (m_initialized)
	{
		if (m_target)		
			delete m_target;
		if (m_target2)
			delete m_target2;
		ggiClose(vis);
		ggiExit();
	}
		
	delete m_palette;
}

void CL_GGI_DisplayCard::flip_display(bool sync)
{
	cl_assert(m_initialized); // card not initialized!
	
	signal_preflip();

//	ggiFlush(vis);

	if (m_target2) // two frames (DirectBuffer)
	{
		// swap them
		CL_Target_GGI* temp;
		temp      = m_target;
		m_target  = m_target2;
		m_target2 = temp;

		m_target->to_back();   // does a ggiSetWriteFrame
		m_target2->to_front(); // does a ggiSetDisplayFrame and a ggiFlush
	} else
	{
		// only one frame (system memory)
		m_target->to_back();  // does nothing
		m_target->to_front(); // does a ggiFlush and a ggiPutBox, or first putbox?
	}

	signal_postflip();
}

void CL_GGI_DisplayCard::put_display(const class CL_Rect &rect)
{
	cl_assert(m_initialized);

	signal_preflip();

	// not implemented. Someone with ggi knowledge, please do so.
	// -- mbn 21. nov 2000
	cl_assert(false);

	signal_postflip();
}

void CL_GGI_DisplayCard::set_palette(CL_Palette *pal)
{
	m_palette = new CL_Palette(pal->palette);
}

CL_Palette *CL_GGI_DisplayCard::get_palette()
{
	return m_palette;
}

void CL_GGI_DisplayCard::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	if (m_initialized) // clean up
	{
		if (m_target)		
			delete m_target;
		if (m_target2)
			delete m_target2;
		m_target = m_target2 = NULL;
	}
	
	set_gfxmode( width, height, bpp, fullscreen, allow_resize );

	ggiSetFlags( vis, GGIFLAG_ASYNC );
	
	switch (bpp)
	{
		case	 8: mode.graphtype = GT_8BIT ; break;
		case	15: mode.graphtype = GT_15BIT; break;
		case	16: mode.graphtype = GT_16BIT; break;
		case	24: mode.graphtype = GT_24BIT; break;
		case	32: mode.graphtype = GT_32BIT; break;
	}
	
	if (ggiSetSimpleMode(vis, get_width(), get_height(), 2, mode.graphtype))
	{
		if (ggiSetSimpleMode(vis, get_width(), get_height(), 1, mode.graphtype))
		{
			ggiClose( vis );
			ggiExit();
			throw CL_Error ( "cannot set mode!" );
		} else
		{
			// ok, we got one frame
			m_target = new CL_Target_GGI_Put( vis );
			std::cout << "Using non DirectBuffer capable target (one frame)" << std::endl;
		}
	} else
	{
		// cool, got two frames
		if (ggiDBGetNumBuffers(vis)) // DirectBuffer capable?
		{
			m_target  = new CL_Target_GGI_DB( vis, 0 );
			m_target2 = new CL_Target_GGI_DB( vis, 1 );
			std::cout << "Using DirectBuffer capable target (two frames)" << std::endl;
		} else
		{
			m_target = new CL_Target_GGI_Put( vis );
			std::cout << "Using non DirectBuffer capable target (two frames, one used)" << std::endl;
		}
	}

	m_initialized = true;
}

bool CL_GGI_DisplayCard::is_initialized()
{
	return m_initialized;
}

const std::list<CL_VidMode*> &CL_GGI_DisplayCard::get_videomodes()
{
	cl_assert(false); // not implemented yet!
	static std::list<CL_VidMode*> ret;
	return ret;
}

void CL_GGI_DisplayCard::draw_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	throw CL_Error("draw_rect not implemented in GGI, please tell us to do that");
}

void CL_GGI_DisplayCard::fill_rect(
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
	if (a <= 0.99
		|| m_target2==NULL)
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


	CL_ClipRect rect(x1, y1, x2, y2);

	CL_ClipRect cur_clip = get_clip_rect();
	if (cur_clip.test_all_clipped(rect))
	{
		return;
	}

	CL_ClipRect crect = cur_clip.clip(rect);
	CL_ColorMap cmap(get_target());
	ggi_pixel pixelcolor = cmap.calc_color(r, g, b, a);
	
	ggiSetGCForeground( vis, pixelcolor );
	ggiDrawBox( vis, crect.m_x1, crect.m_y1, crect.m_x2 - crect.m_x1, crect.m_y2 - crect.m_y1 );
}

void CL_GGI_DisplayCard::draw_line(
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

	CL_ClipRect rect(x1, y1, x2, y2);

	CL_ClipRect cur_clip = get_clip_rect();
	if (cur_clip.test_all_clipped(rect))
	{
		return;
	}

	CL_ClipRect crect = cur_clip.clip(rect);
	CL_ColorMap cmap(get_target());
	ggi_pixel pixelcolor = cmap.calc_color(r, g, b, a);
	
	ggiSetGCForeground( vis, pixelcolor );
	ggiDrawLine( vis, crect.m_x1, crect.m_y1, crect.m_x2 - crect.m_x1, crect.m_y2 - crect.m_y1 );
}


void CL_GGI_DisplayCard::clear_display(
	float red,
	float green,
	float blue,
	float alpha)
{
//	if (alpha <= 0.99)
	{
		fill_rect(
			0,
			0,
			get_width(),
			get_height(),
			red,
			green,
			blue,
			alpha);
		return;
	}
	
/*	CL_ColorMap cmap(get_target());
	ggi_pixel pixelcolor = cmap.calc_color(red, green, blue, alpha);
	
	ggiSetGCForeground( vis, pixelcolor );
	ggiFillscreen( vis );*/
}

#endif
