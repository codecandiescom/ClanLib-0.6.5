/*
	$Id: displaycard_glx.h,v 1.6 2002/02/16 16:10:41 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_displaycard_glx
#define header_displaycard_glx

#ifdef USE_OPENGL

#include "Display/Display/X11/display_xwindow.h"
#include "Display/Display/X11/x11_resolution.h"
#include "GL/Generic/displaycard_gl_generic.h"
#include <GL/glx.h>
#include <iostream>

class CL_GLX_DisplayCard : public CL_GL_DisplayCard_Generic, public CL_KeepAlive
{
public:
	CL_GLX_DisplayCard(
		Display *dpy,
		Window root,
		int card_no);

	virtual ~CL_GLX_DisplayCard();

	// CL_XWindow_CompatibleCard:
	// --------------------------
	
	virtual Window get_window() { return win; }
	virtual Display *get_display() { return dpy; }
	virtual Window get_root() { return root; }

	// CL_GL_DisplayCard_Generic
	// -------------------------
	
	virtual int get_max_texture_size() { return max_texture_size; }

	// CL_DisplayCard functions:
	// -------------------------

	virtual void flip_display(bool sync=false);
	virtual void put_display(const class CL_Rect &rect);
	virtual void set_videomode(int width, int height, int bpp, bool fullscreen, bool allow_resize, bool video_memory);
	
	virtual void (*get_proc_address(const std::string& function_name))();

	virtual std::string get_name() { return "GLX Display"; }

	virtual void keep_alive();
	
private:
//	bool initialized;
	
	Window root;
	Window win;
	Display *dpy;

	GLXContext ctx;
	GLint max_texture_size;

	CL_X11Resolution resolution;
};

#endif

#endif
