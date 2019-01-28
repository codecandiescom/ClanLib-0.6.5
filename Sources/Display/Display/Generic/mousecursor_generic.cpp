/*
	$Id: mousecursor_generic.cpp,v 1.4 2001/12/14 02:39:58 plasmoid Exp $

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

using namespace std;

#include "mousecursor_generic.h"
#include "API/Display/Display/display.h"
#include "API/Core/System/system.h"
#include "API/Display/Input/mouse.h"
#include "surface_generic.h"

/**************************************
      class CL_MouseCursor_Generic
**************************************/

void CL_MouseCursor::set_cursor(
	CL_MouseCursorProvider *provider,
	int frame)
{
	CL_MouseCursor_Generic::m_instance.set_cursor(provider, frame);
}

void CL_MouseCursor::show(bool show_async)
{
	CL_MouseCursor_Generic::m_instance.show(show_async);
}

void CL_MouseCursor::hide()
{
	CL_MouseCursor_Generic::m_instance.hide();
}

/**************************************
      class CL_MouseCursor_Generic
**************************************/
CL_MouseCursor_Generic CL_MouseCursor_Generic::m_instance;

CL_MouseCursor_Generic::CL_MouseCursor_Generic() : m_rect(0,0,0,0)
{
	m_buffer = NULL;
	m_provider = NULL;
	m_cursor = NULL;
	m_cur_frame = 0;
	m_frame_timer = 0;
	m_mux = CL_Mutex::create();
	m_thread = NULL;
	m_callback_added = false;
	m_last_x = m_last_y = -1;
}

CL_MouseCursor_Generic::~CL_MouseCursor_Generic()
{
	if(m_provider)
		delete m_provider;
	if(m_cursor)
		delete m_cursor;
	delete[] m_buffer;
	delete m_mux;
	delete m_thread;
}

void CL_MouseCursor_Generic::run()
{
	while (m_async)
	{
		m_mux->enter();

		if (m_async && m_provider != NULL)
		{
			CL_Target *target = ((CL_DisplayCard_Generic *) CL_Display::get_current_card())->get_frontbuffer();
			show_cursor(target, true);
		}
		m_mux->leave();

		CL_System::sleep(20);
	}
}

void CL_MouseCursor_Generic::pre_flip()
{
	m_mux->enter();

	if (m_visible && m_provider != NULL)
	{
		show_cursor(((CL_DisplayCard_Generic *) CL_Display::get_current_card())->get_target(), false);
	}
	else
	{
		invalidate_region();
	}

	// Do not leave mutex, as this will happen in post_flip
}

void CL_MouseCursor_Generic::post_flip()
{
	m_mux->leave();
}

void CL_MouseCursor_Generic::show_cursor(CL_Target *dest, bool load_saved)
{
	if (!load_saved)
	{
		invalidate_region();
	}
	int mouse_x = CL_Mouse::get_x();
	int mouse_y = CL_Mouse::get_y();

	int x1 = mouse_x+m_provider->get_surface_provider()->get_translate_x();
	int y1 = mouse_y+m_provider->get_surface_provider()->get_translate_y();

	bool mouse_moved = (x1 != m_last_x || y1 != m_last_y);
	if (load_saved && mouse_moved)
	{
		load_region(dest);
	}
	m_last_x = x1;
	m_last_y = y1;

	if (m_provider->is_animated() && CL_System::get_time() >= m_frame_timer)
	{
		m_cur_frame = (m_cur_frame+1) % m_provider->get_surface_provider()->get_num_frames();
		int next_frame = m_provider->get_frame_delay(m_cur_frame);
		if (next_frame == -1) next_frame = 100;
		m_frame_timer = CL_System::get_time() + next_frame;
	}

	CL_ClipRect r(
		x1,
		y1,
		x1+m_provider->get_surface_provider()->get_width(),
		y1+m_provider->get_surface_provider()->get_height());
	CL_ClipRect scr_r = r.clip(CL_Display::get_current_card()->get_clip_rect());
	if (scr_r.m_x1 < scr_r.m_x2 && scr_r.m_y1 < scr_r.m_y2)
	{
		if (mouse_moved)
		{
			save_region(dest, scr_r);
		}
		m_cursor->impl->get_cardsurface(
			CL_Display::get_current_card())->put_screen(
				x1, y1, m_cur_frame, dest);
	}
	else
	{
		invalidate_region();
	}
}

void CL_MouseCursor_Generic::set_cursor(
	CL_MouseCursorProvider *provider,
	int frame)
{
	m_mux->enter();
	if (m_provider != NULL) 
	{
		m_provider->get_surface_provider()->unlock();
		delete m_cursor;
	}
	if (m_cursor != NULL)
	{
		//delete m_cursor;
		m_cursor = NULL;
	}

	m_provider = provider;
	if (m_provider != NULL)
	{
		m_provider->get_surface_provider()->lock();
		m_cursor = CL_Surface::create(m_provider->get_surface_provider(), false);
		m_cur_frame = frame;
	       //	if (m_cur_frame < 0) m_cur_frame = 0; m_cur_frame is unsigned
		if (m_cur_frame >= provider->get_surface_provider()->get_num_frames())
		{
			m_cur_frame = provider->get_surface_provider()->get_num_frames()-1;
		}
		if (!m_callback_added)
		{
			((CL_DisplayCard_Generic *) CL_Display::get_current_card())->add_callback(this);
			m_callback_added = true;
		}
		show(true);
	}
	m_mux->leave();
	
	if (m_provider == NULL && m_visible) // make system cursor visible if no custom cursor was set.
	{
		CL_DisplayCard_Generic *card = (CL_DisplayCard_Generic *) CL_Display::get_current_card();
		card->show_system_cursor();
	}
	else if (m_provider != NULL && m_visible)
	{
		CL_DisplayCard_Generic *card = (CL_DisplayCard_Generic *) CL_Display::get_current_card();
		card->hide_system_cursor();
	}
}

void CL_MouseCursor_Generic::show(bool show_async)
{
	m_async = show_async;
	m_visible = true;

	if (m_cursor == NULL) // if we have no custom cursor, use system cursor.
	{
		CL_DisplayCard_Generic *card = (CL_DisplayCard_Generic *) CL_Display::get_current_card();
		card->show_system_cursor();
		return;
	}

	if (m_async && m_thread == NULL)
	{
		m_thread = CL_Thread::create(this);
		m_thread->start();
	}
	if (m_async == false)
	{
		delete m_thread;
		m_thread = NULL;
	}
}

void CL_MouseCursor_Generic::hide()
{
	CL_DisplayCard_Generic *card = (CL_DisplayCard_Generic *) CL_Display::get_current_card();
	card->hide_system_cursor();

	m_mux->enter();
	m_visible = false;
	delete m_thread;
	m_thread = NULL;
	invalidate_region();
	m_mux->leave();
}

void CL_MouseCursor_Generic::invalidate_region()
{
	delete[] m_buffer;
	m_buffer = NULL;
	m_rect = CL_ClipRect(0,0,0,0);
}

void CL_MouseCursor_Generic::save_region(CL_Target *source, const CL_ClipRect &r)
{
	if (source == NULL) return;

	source->lock();

	if ((m_rect.m_x2-m_rect.m_x1) != (r.m_x2-r.m_x1) ||
		(m_rect.m_y2-m_rect.m_y1) != (r.m_y2-r.m_y1))
	{
		delete[] m_buffer;
		m_buffer = new unsigned char[
			(r.m_x2-r.m_x1)*
			(r.m_y2-r.m_y1)*
			source->get_bytes_per_pixel()];
	}

	int dest_pitch = (r.m_x2-r.m_x1)*source->get_bytes_per_pixel();
	unsigned char *dest_ptr = m_buffer;
	unsigned char *src_ptr = &((unsigned char *) source->get_data())[
		r.m_y1*source->get_pitch()+
		r.m_x1*source->get_bytes_per_pixel()];
	for (int y=r.m_y1;y<r.m_y2;y++)
	{
		memcpy(
			dest_ptr,
			src_ptr,
			dest_pitch);
		dest_ptr += dest_pitch;
		src_ptr += source->get_pitch();
	}

	m_rect = r;

	source->unlock();
}

void CL_MouseCursor_Generic::load_region(CL_Target *dest)
{
	if (dest == NULL || m_buffer == NULL) return;

	dest->lock();

	int src_pitch = (m_rect.m_x2-m_rect.m_x1)*dest->get_bytes_per_pixel();
	unsigned char *src_ptr = m_buffer;
	unsigned char *dest_ptr = &((unsigned char *) dest->get_data())[
		m_rect.m_y1*dest->get_pitch()+
		m_rect.m_x1*dest->get_bytes_per_pixel()];
	for (int y=m_rect.m_y1;y<m_rect.m_y2;y++)
	{
		memcpy(
			dest_ptr,
			src_ptr,
			src_pitch);
		src_ptr += src_pitch;
		dest_ptr += dest->get_pitch();
	}

	dest->unlock();
}
