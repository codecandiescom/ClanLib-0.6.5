/*
	$Id: display_xwindow.h,v 1.5 2002/08/14 13:48:40 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_xwindow
#define header_display_xwindow

#ifdef USE_X11

#include "Display/Display/Generic/displaycard_generic.h"
#include "API/Core/System/keep_alive.h"

#include "API/Core/System/mutex.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "target_ximage_std.h"
#include "target_ximage_shm.h"
#include "target_ximage_dga.h"

#include "Core/System/Unix/init_linux.h"
#include "x11_resolution.h"

class CL_XWindow_CompatibleCard : public CL_DisplayCard_Generic
// used by GLX implementation.
{
public:
	CL_XWindow_CompatibleCard(int card_no)
	: CL_DisplayCard_Generic(card_no)
	{
		fullscreen = false;
		cursor = None;
	}
	
	static CL_Signal_v1<XEvent&> sig_xevent;
	static Atom wm_delete_window;
	
	virtual Window get_window()=0;
	virtual Display *get_display()=0;
	virtual Window get_root()=0;

	virtual int get_total_memory() { return -1; }
	virtual const std::list<CL_VidMode*> &get_videomodes() 
	{
		if (modelist.empty())
			fill_modelist();
		return modelist; 
	}
	virtual bool is_fullscreen() { return fullscreen; }

	// used by mouse code
	int get_win_width() { return win_width; }
	int get_win_height() { return win_height; }

	virtual void show_system_cursor()
	{
		if (cursor != None)
		{
			XFreeCursor(get_display(), cursor);
		}

		XUndefineCursor(get_display(), get_window());
	}
	
	virtual void hide_system_cursor()
	{
		if (cursor == None)
		{
			char bm_no_data[] = { 0,0,0,0, 0,0,0,0 };
			Pixmap pixmap_no_data = XCreateBitmapFromData(
				get_display(),
				get_window(),
				bm_no_data,
				8, 8);
		
			XColor black;
			memset(&black, 0, sizeof(XColor));
			black.flags = DoRed | DoGreen | DoBlue;

			cursor = XCreatePixmapCursor(
				get_display(),
				pixmap_no_data,
				pixmap_no_data,
				&black,
				&black,
				0, 0);

			XFreePixmap(get_display(), pixmap_no_data);

//			cursor = XCreateFontCursor(get_display(), XC_tcross);
		}
	
		XDefineCursor(get_display(), get_window(), cursor);
	}
	
protected:
	virtual void get_real_resolution(int* width, int* height);
	void fill_modelist();

	bool fullscreen;
	int win_width, win_height;

private:
	std::list<CL_VidMode*> modelist;
	Cursor cursor;
};

class CL_XWindow_DisplayCard
: public CL_XWindow_CompatibleCard, public CL_KeepAlive
{
public:
	CL_XWindow_DisplayCard(Display *dpy, Window root, int card_no);
	virtual ~CL_XWindow_DisplayCard();
	
	// CL_XWindow_CompatibleCard:
	// --------------------------

	virtual Window get_window() { return m_win; }
	virtual Display *get_display() { return m_dpy; }
	virtual Window get_root() { return m_root; }

	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_palette(CL_Palette *palette);
	virtual CL_Palette *get_palette();
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	virtual bool is_initialized();

	virtual std::string get_name() { return "X11 Display"; }

	// CL_DisplayCard_Generic functions:
	// ---------------------------------

	virtual CL_Target *get_target() { CL_MutexSection m(m_mux); return m_target; }
	virtual CL_Target *get_frontbuffer() { CL_MutexSection m(m_mux); return m_target_front; }
	
	// CL_System_KeepAlive functions:
	// ------------------------------

	virtual void keep_alive();
	
protected: // inherited by mesa impl.
	virtual CL_Target_XImage *create_target();
	virtual XVisualInfo *get_visual_info() { return &m_visual_info; }

private:
	void on_resize(int width, int height);

	bool m_initialized;
	CL_Palette *m_palette;
	CL_Target_XImage *m_target;
	CL_Target_XImage *m_target_front;

	XVisualInfo m_visual_info;
	Display *m_dpy;
	Window m_win;
	Window m_root;
	GC m_gc;
	int m_scr;
	Colormap m_color_map;
	
	CL_X11Resolution resolution;
	CL_Slot slot_resize;

	CL_Mutex* m_mux;
};

#endif /* USE_X11 */

#endif
