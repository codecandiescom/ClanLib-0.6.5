/*
	$Id: display_xwindow.cpp,v 1.13 2002/08/14 19:33:36 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_X11

#include <API/Application/clanapp.h>
#include <API/Core/System/system.h>
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <API/Display/Display/palette.h>
#include <Display/Display/X11/target_ximage.h>
#include <Display/Display/X11/target_ximage_shm.h>
#include <Display/Display/X11/target_ximage_std.h>
#include <API/Display/Display/vidmode.h>
#include <API/Core/Math/rect.h>
#include <Display/Display/X11/display_xwindow.h>
#include <Display/Display/X11/display_xwindow.h>

CL_Signal_v1<XEvent&> CL_XWindow_CompatibleCard::sig_xevent;
Atom CL_XWindow_CompatibleCard::wm_delete_window;

// CL_XWindow_DisplayCard

CL_XWindow_DisplayCard::CL_XWindow_DisplayCard(
	Display *dpy,
	Window root,
	int card_no)
: CL_XWindow_CompatibleCard(card_no), resolution(dpy, DefaultScreen(dpy))
{
	m_initialized = false;
	m_dpy = dpy;
	m_root = root;
	m_win = 0;
	m_gc = 0;

	m_mux = new CL_Mutex;

	m_scr = DefaultScreen(m_dpy);
//	m_visual_info = DefaultVisual(dpy, m_scr);
	cl_assert(XMatchVisualInfo(dpy, m_scr, DefaultDepth(dpy,m_scr), TrueColor, &m_visual_info));
	m_palette = new CL_Palette();
	m_target = NULL;
	m_target_front = NULL;
	
	slot_resize = sig_resized().connect(
		this, &CL_XWindow_DisplayCard::on_resize);
}

CL_XWindow_DisplayCard::~CL_XWindow_DisplayCard()
{
	if (fullscreen)
	{
		resolution.restore_mode();
	}
	if (m_initialized)
	{
		// BUGBUG: What about the visual, context and buffer - should they be deleted?
		delete m_target;
		delete m_target_front;
		XFreeGC(m_dpy, m_gc);
		XDestroyWindow(m_dpy, m_win);
		XCloseDisplay(m_dpy);
	}
//	XFreeColormap(m_dpy, m_color_map);
	delete m_palette;

	delete m_mux;
	m_mux = NULL;
}

void CL_XWindow_DisplayCard::keep_alive()
{
	XEvent event;
	
	// we try to minize the amounts of exposures to speed up repainting of the window.
	bool do_resize = false;
	int new_width = 0, new_height = 0;
	
	for (int i=XPending(m_dpy); i>0; i--)
	{
		XNextEvent(m_dpy, &event);

		switch (event.type)
		{
		case DestroyNotify:
			break;
			
		case ConfigureNotify:
			do_resize = true;
			new_width = event.xconfigure.width;
			new_height = event.xconfigure.height;
			break;

		case Expose:
			sig_paint()(
				CL_Rect(
					event.xexpose.x,
					event.xexpose.y,
					event.xexpose.x + event.xexpose.width,
					event.xexpose.y + event.xexpose.height));
			break;
		case ClientMessage:
			// Window close request
			if ((int) event.xclient.data.l[0] == (int) wm_delete_window) {
				CL_System::sig_quit().call();
			}
			break;
		}

		sig_xevent(event);
	}

	if (do_resize)
	{
		set_resize(new_width, new_height);
	}
}

void CL_XWindow_DisplayCard::on_resize(int width, int height)
{
	m_mux->enter();
	delete m_target; m_target = NULL;
	delete m_target_front; m_target_front = NULL;

	create_target();
	m_mux->leave();
}

void CL_XWindow_DisplayCard::flip_display(bool sync)
{
	cl_assert(m_initialized); // card not initialized!
	
	signal_preflip();

	// New tactic - sync before putscreen
	if (sync) XSync(m_dpy, false);
	
	int x = 0;
	int y = 0;
	
	if (is_fullscreen())
	{
		x = win_width/2 - get_width()/2;
		y = win_height/2 - get_height()/2;
	}

	m_target->put_image(x, y, m_win, m_gc);
	CL_Target_XImage *t = m_target;
	m_target = m_target_front;
	m_target_front = t;

	XFlush(m_dpy);
	
	signal_postflip();
}

void CL_XWindow_DisplayCard::put_display(const class CL_Rect &rect)
{
	cl_assert(m_initialized);

	signal_preflip();

	int x = rect.x1;
	int y = rect.y1;
	
	if (is_fullscreen())
	{
		x += win_width/2 - get_width()/2;
		y += win_height/2 - get_height()/2;
	}

	m_target->put_image(x, y, rect, m_win, m_gc);

	XFlush(m_dpy);

	signal_postflip();
}

void CL_XWindow_DisplayCard::set_palette(CL_Palette *pal)
{
	m_palette = new CL_Palette(pal->palette);
}

CL_Palette *CL_XWindow_DisplayCard::get_palette()
{
	return m_palette;
}

void CL_XWindow_DisplayCard::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	this->fullscreen = fullscreen;
	if (m_initialized) // clean up
	{
		delete m_target; m_target = NULL;
		delete m_target_front; m_target_front = NULL;
		XFreeGC(m_dpy, m_gc);
		XDestroyWindow(m_dpy, m_win);
		m_win = 0;
	}
	set_gfxmode(width, height, bpp, fullscreen, allow_resize);

	/* set window attributes */
	XSetWindowAttributes attr;
	attr.override_redirect = False;
//	attr.colormap = m_color_map;
	attr.event_mask =
		KeyPressMask|KeyReleaseMask|
		ButtonPressMask|ButtonReleaseMask|
		EnterWindowMask|LeaveWindowMask|
		PointerMotionMask|
		StructureNotifyMask|
		ExposureMask;
//	attr.border_pixel = BlackPixel(m_dpy, m_scr);
//	attr.background_pixel = BlackPixel(m_dpy, m_scr);
	attr.backing_store = Always;
	int attr_flags =
		CWOverrideRedirect |
		CWEventMask;
//		CWColormap |
//		CWBorderPixel |
//		CWBackPixel |
//		CWBackingStore;

	win_width = width;
	win_height = height;
	if (fullscreen)
	{
		resolution.set_mode(width, height);
		win_width = resolution.get_width();
		win_height = resolution.get_height();
		attr.override_redirect = True;
	}

	/* Create the window */
	m_win = XCreateWindow(
		m_dpy,
		m_root,
		0,
		0,
		win_width,
		win_height,
		0,
		CopyFromParent, // m_visual_info.depth,
		CopyFromParent, // m_visual_info.c_class
		m_visual_info.visual,
		attr_flags,
		&attr);

	cl_assert(m_win); // failed to create window!

	char *title = CL_ClanApplication::app->get_title();

	XTextProperty text_property;
	XStringListToTextProperty(&title, 1, &text_property);
	XSizeHints size_hints;
	size_hints.x = 0;
	size_hints.y = 0;
	size_hints.width = win_width;
	size_hints.height = win_height;
	size_hints.flags = PSize;
	if (!allow_resize) size_hints.flags |= PMinSize | PMaxSize;
	size_hints.min_width = win_width;
	size_hints.min_height = win_height;
	size_hints.max_width = win_width;
	size_hints.max_height = win_height;
//	size_hints.flags |= USPosition | USSize;
	XSetWMProperties(
		m_dpy,
		m_win,
		&text_property,
		&text_property,
		0,
		0,
		&size_hints,
		0,
		0);

	// Set WM_DELETE_WINDOW atom in WM_PROTOCOLS property (to get window_delete requests).
	wm_delete_window = XInternAtom (m_dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols (m_dpy, m_win, &wm_delete_window, 1);

//	XMapWindow(m_dpy, m_win);
	XMapRaised(m_dpy, m_win);
	
	if (fullscreen)
	{
		XSetInputFocus(m_dpy, m_win, RevertToParent, CurrentTime);
		XGrabPointer(m_dpy, m_win, true, 0, GrabModeAsync, GrabModeAsync, m_win, None, CurrentTime);
	}

	m_target = create_target();
	m_gc = XCreateGC(m_dpy, m_win, 0, 0);
	
	// Listen for keystrokes...
//	XSelectInput(m_dpy, m_win, KeyPressMask|KeyReleaseMask);

	m_initialized = true;
}

CL_Target_XImage *CL_XWindow_DisplayCard::create_target()
{
	try
	{
		m_target = new CL_Target_XImage_Shm(
			m_visual_info,
			m_dpy,
			get_width(),
			get_height());

		m_target_front = new CL_Target_XImage_Shm(
			m_visual_info,
			m_dpy,
			get_width(),
			get_height());
		
		return m_target;
	}
	catch (CL_Error err)
	{
		static bool warning = true;
		if (warning)
		{
			cl_info(info_display, "X11 Implementation: Fallback to normal XImages. This will be horribly slow!");
			warning = false;
		}

		m_target = new CL_Target_XImage_Std(
			m_visual_info,
			m_dpy,
			get_width(),
			get_height());

		m_target_front = new CL_Target_XImage_Std(
			m_visual_info,
			m_dpy,
			get_width(),
			get_height());

		return m_target;
	}
}

bool CL_XWindow_DisplayCard::is_initialized()
{
	return m_initialized;
}

#ifdef USE_VIDMODE
// needed by qsort
static int cmpmodes(const void *va, const void *vb)
{
	XF86VidModeModeInfo *a = *(XF86VidModeModeInfo**)va;
	XF86VidModeModeInfo *b = *(XF86VidModeModeInfo**)vb;
	if(a->hdisplay > b->hdisplay)
		return -1;
	return b->vdisplay - a->vdisplay;
}
#endif

void CL_XWindow_CompatibleCard::fill_modelist()
{
	if (modelist.empty())
	{
#ifdef USE_VIDMODE
		int vm_event, vm_error;
		int major_version, minor_version;
		int nmodes;
		XF86VidModeModeInfo **modes;
		
		/* Enumerate the available fullscreen modes */
		if (XF86VidModeQueryExtension(get_display(), &vm_event, &vm_error) &&
    			XF86VidModeQueryVersion(get_display(), &major_version, &minor_version) &&
    			XF86VidModeGetAllModeLines(get_display(), DefaultScreen(get_display()),&nmodes,&modes) ) 
		{
		        qsort(modes, nmodes, sizeof *modes, cmpmodes);

			for (int i=0; i<nmodes; ++i ) 
			{
				CL_VidMode* mode = new CL_VidMode(modes[i]->hdisplay, modes[i]->vdisplay, 0);
            			modelist.push_back(mode);	
    			}

    			XFree(modes);
		}
#endif /* USE_VIDMODE */
		CL_VidMode* mode = new CL_VidMode(
			DisplayWidth(get_display(), DefaultScreen(get_display())), 
			DisplayHeight(get_display(), DefaultScreen(get_display())), 
			0);
    		modelist.push_back(mode);
#ifdef DEBUG
#ifdef USE_VIDMODE
    		std::cout << "XFree86 VidMode is enabled" << std::endl;
#endif
		for (std::list<CL_VidMode*>::iterator it = modelist.begin(); it != modelist.end(); it++)
			std::cout << (*it)->width << "x" << (*it)->height << std::endl;
#endif /* DEBUG */
	}
}

void CL_XWindow_CompatibleCard::get_real_resolution(int* w, int* h)
{
#ifdef USE_VIDMODE
	XF86VidModeModeLine mode;
	int unused;

	if ( XF86VidModeGetModeLine(get_display(), DefaultScreen(get_display()), &unused, &mode) ) 
	{
		*w = mode.hdisplay;
		*h = mode.vdisplay;
		return;
	}
#endif
	*w = DisplayWidth(get_display(), DefaultScreen(get_display()));
	*h = DisplayHeight(get_display(), DefaultScreen(get_display()));
}

#endif
