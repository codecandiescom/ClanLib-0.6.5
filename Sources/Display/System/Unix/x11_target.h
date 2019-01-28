/*
	$Id: x11_target.h,v 1.5 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_x11_target
#define header_x11_target

#ifdef USE_X11

#include "X11/Xlib.h"
#include "X11/Xutil.h"

#include "display_target.h"

class CL_XWindow_CompatibleCard;

class CL_X11Target : public CL_DisplayTarget
{
public:
	// CL_DisplayTarget interface:
	// ---------------------------
	
	virtual const char *get_long_name();

	virtual const char *get_short_name();

	virtual void init();

	virtual void deinit();
	
	// X11 target:
	// -----------
	
	virtual CL_XWindow_CompatibleCard *create_displaycard(Display *dpy, Window root, int display_counter);

	static int display_counter;
	static Display *dpy;
	static Window root;
	static int default_scr;
};

#endif

#endif
