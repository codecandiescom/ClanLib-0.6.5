/*
	$Id: displaycard_generic.h,v 1.7 2002/02/03 20:56:06 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_generic
#define header_displaycard_generic

#include "API/Display/Display/target.h"
#include "API/Display/Display/displaycard.h"
#include "API/Core/System/cl_assert.h"

class CL_SurfaceProvider;
class CL_CardSurface;
class CL_Target;
class CL_CardSurface_Blitter;
class CL_Blitters;

#include <stack>

class CL_DisplayCard_Generic : public CL_DisplayCard
{
public:
	CL_DisplayCard_Generic(int card_no)
	{
		m_card_no = card_no;
		m_width = 0;
		m_height = 0;
		m_bpp = 0;
		m_cur_clip = CL_ClipRect(0,0,0,0);
		m_fullscreen = 0;
	}

	virtual ~CL_DisplayCard_Generic() {;}
	
	virtual CL_CardSurface *create_cardsurface_video(
		CL_SurfaceProvider *provider);

	virtual CL_CardSurface *create_cardsurface_system(
		CL_SurfaceProvider *provider);

	virtual int get_card_no() { return m_card_no; }

	virtual int get_width() { return m_width; }
	virtual int get_height() { return m_height; }
	virtual int get_bpp() { return m_bpp; }
	virtual bool is_fullscreen() { return m_fullscreen; }
	virtual bool allow_resize() { return m_allow_resize; }
	
	virtual void push_clip_rect()
	{
		m_clip_stack.push_front(m_cur_clip);
	}

	virtual void push_clip_rect(const CL_ClipRect &clip)
	{
		push_clip_rect();
		m_cur_clip = m_cur_clip.clip(clip);
		if (get_target() != NULL) get_target()->set_clip_rect(m_cur_clip);
	}
	virtual CL_ClipRect get_clip_rect()
	{
		return m_cur_clip;
	}

	virtual void set_clip_rect(const CL_ClipRect &clip)
	{
		m_cur_clip = clip;
		if (get_target() != NULL) get_target()->set_clip_rect(m_cur_clip);
	}

	virtual void pop_clip_rect()
	{
		m_cur_clip = m_clip_stack.front();
		m_clip_stack.pop_front();
		if (get_target() != NULL) get_target()->set_clip_rect(m_cur_clip);
	}

	virtual void clear_display(float red=0, float green=0, float blue=0, float alpha=1);
	virtual void fill_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void draw_rect(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	virtual void draw_line(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
	
	virtual void sync_buffers();

	virtual CL_Target *get_frontbuffer()=0;
	void frontbuffer_lock();
	void frontbuffer_unlock();

	class CL_FlipDisplayCallback_Generic
	{
	public:
		virtual void pre_flip()=0;
		virtual void post_flip()=0;
	};

	void add_callback(CL_FlipDisplayCallback_Generic *callback)
	{
		callbacks.push_back(callback);
	}
	
	virtual void show_system_cursor()
	{
		static bool warning = true;
		if (warning)
		{
			cl_info(1, "System cursor show() not implemented for this display target.");
			warning = false;
		}
	}

	virtual void hide_system_cursor()
	{
		static bool warning = true;
		if (warning)
		{
			cl_info(1, "System cursor hide() not implemented for this display target.");
			warning = false;
		}
	}

protected:
	std::list<CL_FlipDisplayCallback_Generic*> callbacks;

	void signal_preflip()
	{
		for (std::list<CL_FlipDisplayCallback_Generic*>::iterator it = callbacks.begin();
			it!=callbacks.end(); 
			it++)
		{
			(*it)->pre_flip(); 
		}
	}

	void signal_postflip()
	{
		for (std::list<CL_FlipDisplayCallback_Generic*>::iterator it = callbacks.begin();
			it!=callbacks.end(); 
			it++)
		{
			(*it)->post_flip(); 
		}
	}

	friend class CL_CardSurface_Blitter;

	void set_gfxmode(int width, int height, int bpp, bool fullscreen, bool allow_resize)
	{
		m_width = width;
		m_height = height;
		m_bpp = bpp;
		m_fullscreen = fullscreen;
		m_allow_resize = allow_resize;
		
		m_cur_clip = CL_ClipRect(0, 0, width, height);
		while (m_clip_stack.empty() == false) m_clip_stack.pop_front();

		if (m_clip_stack.empty())
		{
			set_clip_rect(CL_ClipRect(0, 0, width, height));
		}
		else
		{
			m_clip_stack.back() = CL_ClipRect(0, 0, width, height);
		}
	}

	void set_resize(int width, int height)
	{
		m_width = width;
		m_height = height;

		if (m_clip_stack.empty())
		{
			set_clip_rect(CL_ClipRect(0, 0, width, height));
		}
		else
		{
			m_clip_stack.back() = CL_ClipRect(0, 0, width, height);
		}

		m_sig_resized(width, height);
	}

	// Implement those to get hardware support:
	virtual CL_Blitters create_hw_blitters(CL_SurfaceProvider *provider);
	virtual CL_Blitters create_hw_dynamic_blitters(CL_SurfaceProvider *provider);

	CL_ClipRect m_cur_clip;
	std::deque<CL_ClipRect> m_clip_stack;

public:
	virtual void push_translate_offset() 
	{
		if (get_target() == get_frontbuffer())
		{
			get_target()->push_translate_offset();
		}
		else
		{
			get_target()->push_translate_offset();
			get_frontbuffer()->push_translate_offset();
		}
	}
	virtual void push_translate_offset(int x, int y) 
	{ 
		if (get_target() == get_frontbuffer())
		{
			get_target()->push_translate_offset(x, y);
		}
		else
		{
			get_target()->push_translate_offset(x, y);
			get_frontbuffer()->push_translate_offset(x, y);
		}
	}
	virtual int  get_translate_offset_x() { return get_target()->get_translate_offset_x(); }
	virtual int  get_translate_offset_y() { return get_target()->get_translate_offset_y(); }
	virtual void set_translate_offset(int x, int y) 
	{ 
		if (get_target() == get_frontbuffer())
		{
			get_target()->set_translate_offset(x, y);
		}
		else
		{
			get_target()->set_translate_offset(x, y);
			get_frontbuffer()->set_translate_offset(x, y);
		}
	}
	virtual void pop_translate_offset() 
	{ 
		if (get_target() == get_frontbuffer())
		{
			get_target()->pop_translate_offset();
		}
		else
		{
			get_target()->pop_translate_offset();
			get_frontbuffer()->pop_translate_offset();
		}
	}

	virtual CL_Signal_v2<int, int> &sig_resized() { return m_sig_resized; }
	virtual CL_Signal_v1<const CL_Rect &> &sig_paint() { return m_sig_paint; }

private:
	int m_card_no;

	int m_width;
	int m_height;
	int m_bpp;

	bool m_allow_resize;
	bool m_fullscreen;

	CL_Signal_v2<int, int> m_sig_resized;
	CL_Signal_v1<const CL_Rect &> m_sig_paint;

	void fill_rect_opaque(int x1, int y1, int x2, int y2, float r, float g, float b, float a);
};

#endif
