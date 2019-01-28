/*
	$Id: displaycard_be.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/Display/cliprect.h>
#include <API/Core/Display/palette.h>
#include <API/Core/Display/vidmode.h>
#include <Core/Display/Generic/colormap.h>
#include <Core/Display/Be/displaycard_be.h>
#include <Core/Display/Generic/displaycard_generic.h>
#include <Core/System/Be/app_beos.h>

#include "clanwindowscreen.h"

CL_DisplayCard_Be::CL_DisplayCard_Be(int card_no) : CL_DisplayCard_Generic(card_no)
{
	m_palette = new CL_Palette();
	m_target = new CL_Target_Be();
	
	clws = ((LibApplication*)be_app)->clanscreen;
	cl_assert(clws);
}

CL_DisplayCard_Be::~CL_DisplayCard_Be()
{
//	CL_MouseCursor::hide();

	delete m_target;
	delete m_palette;
}

void CL_DisplayCard_Be::flip_display(bool sync)
{
	signal_preflip();
	clws->flip_display();
	signal_postflip();
}

void CL_DisplayCard_Be::put_display(const class CL_Rect &rect)
{
	signal_preflip();
	clws->put_display(rect);
	signal_postflip();
}

void CL_DisplayCard_Be::set_palette(CL_Palette *pal)
{
	m_palette = new CL_Palette(pal->palette);
}

CL_Palette *CL_DisplayCard_Be::get_palette()
{
	return m_palette;
}

void CL_DisplayCard_Be::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	set_gfxmode(width, height, bpp, fullscreen, allow_resize);
	clws->set_videomode(get_width(), get_height(), bpp, fullscreen, allow_resize, video_memory);
}

bool CL_DisplayCard_Be::is_initialized()
{
	return true;
}

const std::list<CL_VidMode*> &CL_DisplayCard_Be::get_videomodes()
{
	cl_assert(false); // not implemented yet!
	static std::list<CL_VidMode*> modes;
	return modes;
}

void CL_DisplayCard_Be::fill_rect(
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
	if (a <= 0.99 || !clws->fill_rect_accelerated() || (!clws->is_connected()))
	{
		CL_DisplayCard_Generic::fill_rect(x1, y1, x2, y2, r, g, b, a);
		return;
	}

	CL_ClipRect rect(x1, y1, x2, y2);

	CL_ClipRect cur_clip = get_clip_rect();
	if (cur_clip.test_all_clipped(rect))
	{
		return;
	}

	CL_ClipRect crect = cur_clip.clip(rect);
	CL_ColorMap cmap(get_target());

	switch (m_target->get_depth())
	{
		case 8:
			uint8 pixelcolor8;
			pixelcolor8 = cmap.calc_color(r, g, b, a);
			clws->fill_rect8( crect.m_x1, crect.m_y1+get_height()*clws->write_frame, crect.m_x2, crect.m_y2+get_height()*clws->write_frame, pixelcolor8 );
			break;
		case 16:
			uint16 pixelcolor16;
			pixelcolor16 = cmap.calc_color(r, g, b, a);
			clws->fill_rect16( crect.m_x1, crect.m_y1+get_height()*clws->write_frame, crect.m_x2, crect.m_y2+get_height()*clws->write_frame, pixelcolor16 );
			break;
		case 32:
			uint32 pixelcolor32;
			pixelcolor32 = cmap.calc_color(r, g, b, a);
			clws->fill_rect32( crect.m_x1, crect.m_y1+get_height()*clws->write_frame, crect.m_x2, crect.m_y2+get_height()*clws->write_frame, pixelcolor32 );
			break;
	}
}

void CL_DisplayCard_Be::clear_display(
	float red,
	float green,
	float blue,
	float alpha)
{
/*	if (!clws->sync)
	{
		CL_DisplayCard_Generic::fill_rect(
			0,
			0,
			get_width(),
			get_height(),
			red,
			green,
			blue,
			alpha);
	} else
	{*/
		fill_rect(
			0,
			0,
			get_width(),
			get_height(),
			red,
			green,
			blue,
			alpha);
/*		clws->sync();
	}*/
}
