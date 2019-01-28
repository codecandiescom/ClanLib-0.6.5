/*
	$Id: displaycard_generic.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <Display/Display/Generic/displaycard_generic.h>
#include <API/Display/Display/vidmode.h>
#include <Display/Display/Generic/blitters.h>
#include "blit_dynamic.h"
#include <Display/Display/Generic/cardsurface_blitter.h>

void CL_DisplayCard::set_videomode(class CL_VidMode *vidmode)
{
	set_videomode(
		vidmode->width,
		vidmode->height,
		vidmode->bpp);
}

CL_CardSurface *CL_DisplayCard_Generic::create_cardsurface_video(
	CL_SurfaceProvider *provider)
{
	return new CL_CardSurface_Blitter(this, provider);
}

CL_CardSurface *CL_DisplayCard_Generic::create_cardsurface_system(
	CL_SurfaceProvider *provider)
{
	return new CL_CardSurface_Blitter(this, provider);
}

CL_Blitters CL_DisplayCard_Generic::create_hw_blitters(CL_SurfaceProvider *provider)
{
	CL_Blitters ret;
	return ret;
}

CL_Blitters CL_DisplayCard_Generic::create_hw_dynamic_blitters(CL_SurfaceProvider *provider)
{
	CL_Blitters ret;
	CL_Blit_Dynamic *blt_dyn = new CL_Blit_Dynamic(provider, get_target());

	ret.set_noclip(blt_dyn);
	ret.set_clip(blt_dyn);
	ret.set_scale_noclip(blt_dyn);
	ret.set_scale_clip(blt_dyn);
	
	return ret;
}

void CL_DisplayCard_Generic::clear_display(
	float red,
	float green,
	float blue,
	float alpha)
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
}

void CL_DisplayCard_Generic::fill_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	get_target()->fill_rect(x1, y1, x2, y2, r, g, b, a);
}

void CL_DisplayCard_Generic::draw_rect(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	get_target()->draw_rect(x1, y1, x2, y2, r, g, b, a);
}

void CL_DisplayCard_Generic::sync_buffers()
{
	CL_Target *front = get_frontbuffer();
	CL_Target *back = get_target();
	
	if (front == back) return; // single buffered system.
	
	back->lock();
	front->lock();

	memcpy(
		back->get_data(),
		front->get_data(),
		back->get_height()*back->get_pitch());

	front->unlock();
	back->unlock();
}

void CL_DisplayCard_Generic::draw_line(
	int x1,
	int y1,
	int x2,
	int y2,
	float r,
	float g,
	float b,
	float a)
{
	get_target()->draw_line(
		x1, y1,
		x2, y2,
		r, g, b, a);
}
