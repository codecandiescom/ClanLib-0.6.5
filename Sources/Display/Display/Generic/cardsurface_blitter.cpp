/*
	$Id: cardsurface_blitter.cpp,v 1.4 2001/12/14 02:39:58 plasmoid Exp $

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

#include <Display/Display/Generic/cardsurface_blitter.h>
#include <API/Display/Display/pixeldata.h>
#include <Display/Display/Generic/blit_transparent.h>
#include <API/Display/Display/surfaceprovider.h>
#include <Display/Display/Generic/displaycard_generic.h>
#include <Display/Display/Generic/blit_opaque.h>

CL_CardSurface_Blitter::CL_CardSurface_Blitter(
	CL_DisplayCard_Generic *target, 
	CL_SurfaceProvider *provider,
	bool dynamic)
: CL_CardSurface(provider)
{
	m_target = target;
	m_dynamic = dynamic;

//	blt_dyn = NULL;
	
	if (m_dynamic) m_provider->lock();

	reload();
}

CL_CardSurface_Blitter::~CL_CardSurface_Blitter()
{
	m_blitters.delete_all();
	
/*	if(blt_dyn != NULL)
	{
		delete blt_dyn;
		blt_dyn = NULL;
	}
*/
	
	if (m_dynamic) m_provider->unlock();
}

bool CL_CardSurface_Blitter::reload()
{
	m_blitters.delete_all();

	m_provider->lock();

	m_width = m_provider->get_width();
	m_height = m_provider->get_height();
	m_no_sprs = m_provider->get_num_frames();

	if (m_dynamic)
	{
		m_blitters = m_target->create_hw_dynamic_blitters(m_provider);
		create_sw_blitters();
	}
	else
	{
		m_blitters = m_target->create_hw_blitters(m_provider);
		create_sw_blitters();
	}

	m_provider->unlock();

	return true;
}

bool CL_CardSurface_Blitter::can_convert_video()
{
	return false;
}

bool CL_CardSurface_Blitter::can_convert_system()
{
	return false;
}

bool CL_CardSurface_Blitter::convert_video()
{
	return false;
}

bool CL_CardSurface_Blitter::convert_system()
{
	return false;
}

CL_Target *CL_CardSurface_Blitter::get_target()
{
	return m_target->get_target();
}

CL_ClipRect CL_CardSurface_Blitter::get_clip_rect()
{
	return m_target->get_clip_rect();
}

void CL_CardSurface_Blitter::put_screen(
	int x,
	int y,
	int spr_no,
	CL_Target *target)
{
	if (target == NULL) target = get_target();

	if (target != NULL)
	{
		x += target->get_translate_offset_x();
		y += target->get_translate_offset_y();
	}

	CL_ClipRect c = get_clip_rect();
	CL_ClipRect d(x, y, x+get_width(), y+get_height());
	if (c.test_clipped(d))
	{
		m_blitters.get_clip()->blt_clip(
			target,
			x,
			y,
			spr_no,
			c);
	}
	else
	{
		m_blitters.get_noclip()->blt_noclip(
			target,
			x,
			y,
			spr_no);
	}
}
		
void CL_CardSurface_Blitter::put_screen(
	int x,
	int y,
	float scale_x,
	float scale_y,
	int spr_no,
	CL_Target *target)
{
	put_screen(
		x,
		y,
		(int) (scale_x * get_width()),
		(int) (scale_y * get_height()),
		spr_no,
		target);
}

void CL_CardSurface_Blitter::put_screen(
	int x,
	int y,
	int size_x,
	int size_y,
	int spr_no,
	CL_Target *target)
{
	if (target == NULL) target = get_target();

	if (target != NULL)
	{
		x += target->get_translate_offset_x();
		y += target->get_translate_offset_y();
	}

	CL_ClipRect c = get_clip_rect();
	CL_ClipRect d(x, y, x+size_x, y+size_y);
	if (c.test_clipped(d))
	{
		m_blitters.get_scale_clip()->blt_scale_clip(
			target,
			x,
			y,
			size_x,
			size_y,
			spr_no,
			c);
	}
	else
	{
		m_blitters.get_scale_noclip()->blt_scale_noclip(
			target,
			x,
			y,
			size_x,
			size_y,
			spr_no);
	}

}

bool CL_CardSurface_Blitter::is_video()
{
	return true;
}

void CL_CardSurface_Blitter::create_sw_blitters()
{
	if (m_blitters.test_complete()) return;

	int bytes_per_pixel = (get_target()->get_depth()+7)/8;

	CL_Blit_Transparent *blt_trans = new CL_Blit_Transparent(m_target, m_provider);
	bool res = blt_trans->init_surface(&m_blitters);

	if (!res)
	{
		delete blt_trans;
	/*	if(blt_dyn)
		{
			delete blt_dyn;
			blt_dyn = NULL;
		}
	*/
		// If blit-transparent returns false we're dealing with a fully opaque surface
		CL_Blit_Opaque *blt_dyn = new CL_Blit_Opaque(
			m_provider,
			bytes_per_pixel,
			get_target());

		if (!m_blitters.test_noclip())		m_blitters.set_noclip(blt_dyn);
		if (!m_blitters.test_clip())		m_blitters.set_clip(blt_dyn);
		if (!m_blitters.test_scale_noclip())	m_blitters.set_scale_noclip(blt_dyn);
		if (!m_blitters.test_scale_clip())	m_blitters.set_scale_clip(blt_dyn);
	}
}
