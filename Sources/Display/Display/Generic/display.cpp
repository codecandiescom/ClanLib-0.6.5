/*
	$Id: display.cpp,v 1.5 2001/04/21 18:16:20 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef BORLAND
#pragma hdrstop
#endif

#include <API/Display/Display/display.h>
#include <API/Display/Display/displaycard.h>
#include <API/Display/Display/surface.h>

std::vector<CL_DisplayCard*> CL_Display::cards;

void CL_Display::flip_display(bool sync)
{
	CL_Display::get_current_card()->flip_display(sync);
}

void CL_Display::put_display(const class CL_Rect &rect)
{
	CL_Display::get_current_card()->put_display(rect);
}

void CL_Display::clear_display(float red, float green, float blue, float alpha)
{
	CL_Display::get_current_card()->clear_display(red, green, blue, alpha);
}

void CL_Display::set_palette(CL_Palette *palette)
{
	CL_Display::get_current_card()->set_palette(palette);
}

CL_Palette *CL_Display::get_palette()
{
	return CL_Display::get_current_card()->get_palette();
}

void CL_Display::set_videomode(CL_VidMode *mode)
{
	CL_Display::get_current_card()->set_videomode(mode);
}

void CL_Display::set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory)
{
	CL_Display::get_current_card()->set_videomode(width, height, bpp, fullscreen, allow_resize, video_memory);
}

CL_Target *CL_Display::get_target()
{
	return CL_Display::get_current_card()->get_target();
}

int CL_Display::get_width()
{
	return CL_Display::get_current_card()->get_width();
}

int CL_Display::get_height()
{
	return CL_Display::get_current_card()->get_height();
}

int CL_Display::get_bpp()
{
	return CL_Display::get_current_card()->get_bpp();
}

void CL_Display::push_clip_rect()
{
	CL_Display::get_current_card()->push_clip_rect();
}

void CL_Display::push_clip_rect(const CL_ClipRect &rect)
{
	CL_Display::get_current_card()->push_clip_rect(rect);
}

CL_ClipRect CL_Display::get_clip_rect()
{
	return CL_Display::get_current_card()->get_clip_rect();
}

void CL_Display::set_clip_rect(const CL_ClipRect &rect)
{
	CL_Display::get_current_card()->set_clip_rect(rect);
}

void CL_Display::pop_clip_rect()
{
	CL_Display::get_current_card()->pop_clip_rect();
}

void CL_Display::push_translate_offset()
{
	CL_Display::get_current_card()->push_translate_offset();
}

void CL_Display::push_translate_offset(int x, int y)
{
	CL_Display::get_current_card()->push_translate_offset(x, y);
}

int  CL_Display::get_translate_offset_x()
{
	return CL_Display::get_current_card()->get_translate_offset_x();
}

int  CL_Display::get_translate_offset_y()
{
	return CL_Display::get_current_card()->get_translate_offset_y();
}

void CL_Display::set_translate_offset(int x, int y)
{
	CL_Display::get_current_card()->set_translate_offset(x, y);
}

void CL_Display::pop_translate_offset()
{
	CL_Display::get_current_card()->pop_translate_offset();
}

void CL_Display::fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
	CL_Display::get_current_card()->fill_rect(x1, y1, x2, y2, r, g, b, a);
}

void CL_Display::draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
	CL_Display::get_current_card()->draw_rect(x1, y1, x2, y2, r, g, b, a);
}

void CL_Display::fill_rect(int x1, int y1, int x2, int y2, CL_Surface *fill_surface, int focus_x, int focus_y)
{
	push_clip_rect(CL_ClipRect(
		get_translate_offset_x()+x1,
		get_translate_offset_y()+y1,
		get_translate_offset_x()+x2,
		get_translate_offset_y()+y2));

	int width = fill_surface->get_width();
	int height = fill_surface->get_height();

	int x, y;
	if (focus_x < x1)
	{
		for (x=focus_x;x+width<x1;x+=width);
	}
	else
	{
		for (x=focus_x;x>x1;x-=width);
	}
	if (focus_y < y1)
	{
		for (y=focus_y;y+height<y1;y+=height);
	}
	else
	{
		for (y=focus_y;y>y1;y-=height);
	}

	for (;y<y2;)
	{
		for (int it_x=x;it_x<x2;it_x+=width)
		{
			fill_surface->put_screen(it_x, y);
		}
		y += height;
	}

	pop_clip_rect();
}

void CL_Display::draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
	CL_Display::get_current_card()->draw_line(x1, y1, x2, y2, r, g, b, a);
}

void CL_Display::sync_buffers()
{
	CL_Display::get_current_card()->sync_buffers();
}

CL_Signal_v2<int, int> &CL_Display::sig_resized()
{
	return CL_Display::get_current_card()->sig_resized();
}

CL_Signal_v1<const CL_Rect &> &CL_Display::sig_paint()
{
	return CL_Display::get_current_card()->sig_paint();
}

