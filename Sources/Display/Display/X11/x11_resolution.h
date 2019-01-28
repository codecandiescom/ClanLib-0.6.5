/*
	$Id: x11_resolution.h,v 1.2 2001/04/21 18:16:20 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_x11resolution
#define header_x11resolution

#ifdef USE_X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef USE_VIDMODE
#include <X11/extensions/xf86vmode.h>
#endif

class CL_X11Resolution
{
public:
	CL_X11Resolution(Display *dpy, int scr);
	~CL_X11Resolution();
	
	void set_mode(int width, int height);
	void restore_mode();

	int get_width() const;
	int get_height() const;

private:
#ifdef USE_VIDMODE
	XF86VidModeModeInfo get_best_mode(int width, int height);

	XF86VidModeModeInfo orig_mode;
#endif
	Display *dpy;
	int scr;
	
	bool mode_set;
	struct
	{
		int x,y;
	} orig_view;
	
	int cur_width, cur_height;
	bool has_vidmode_extension;
};

#endif
#endif

