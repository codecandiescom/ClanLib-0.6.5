/*
	$Id: x11_resolution.cpp,v 1.6 2002/04/04 01:16:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>

#ifdef USE_X11
#include "API/Core/System/cl_assert.h"
#include "x11_resolution.h"

CL_X11Resolution::CL_X11Resolution(Display *dpy, int scr)
{
	this->dpy = dpy;
	this->scr = scr;
	mode_set = false;
	has_vidmode_extension = false;

#ifdef USE_VIDMODE
	int dummy;
	has_vidmode_extension =
		XF86VidModeQueryExtension(dpy, &dummy, &dummy) == True ? true : false;
#endif

	cur_width = DisplayWidth(dpy, scr);
	cur_height = DisplayHeight(dpy, scr);
}

CL_X11Resolution::~CL_X11Resolution()
{
// Can't do this here -- causes crash. See
// CL_XWindow_DisplayCard::~CL_XWindow_DisplayCard() instead.
//	restore_mode();
}
	
void CL_X11Resolution::set_mode(int width, int height)
{
#ifdef USE_VIDMODE
	if (!has_vidmode_extension) return;

	if (!mode_set) // save original mode
	{
		memset(&orig_mode, 0, sizeof(orig_mode));
		memset(&orig_view, 0, sizeof(orig_view));

		// Get mode info. (missing XF86VidModeGetModeInfo)
		XF86VidModeModeLine *l = (XF86VidModeModeLine*)((char*) &orig_mode + sizeof orig_mode.dotclock);
		cl_assert(XF86VidModeGetModeLine(dpy, scr, (int*) &orig_mode.dotclock, l) == True);

		// Get the viewport position.
		cl_assert(XF86VidModeGetViewPort(dpy, scr, &orig_view.x, &orig_view.y) == True);
	}
	
	XF86VidModeModeInfo mode = get_best_mode(width, height);

	if ((mode.vdisplay != 0) && (mode.hdisplay != 0))
	{
		// Warping the mouse to the window (don't use SetViewPort - buggy)
		XWarpPointer(dpy, None, RootWindow(dpy, scr), 0, 0, 0, 0, 0, 0);
		cl_assert(XF86VidModeSwitchToMode(dpy, scr, &mode) == True);
		XWarpPointer(dpy, None, RootWindow(dpy, scr), 0, 0, 0, 0, 0, 0);	
		// cl_assert(XF86VidModeSetViewPort(dpy, scr, 0, 0) == True);

		cur_width = mode.hdisplay;
		cur_height = mode.vdisplay;
		mode_set = true;
		XSync(dpy, True);
	}
#endif
}

void CL_X11Resolution::restore_mode()
{
#ifdef USE_VIDMODE
	if (!has_vidmode_extension) return;
	if (!mode_set) return;
	
	if (cur_width != orig_mode.hdisplay || cur_height != orig_mode.vdisplay)
	{
		cl_assert(XF86VidModeSwitchToMode(dpy, scr, &orig_mode) == True);
	}
	if (orig_view.x != 0 || orig_view.y != 0)
	{
#ifdef DEBUG
		std::cout << "setting viewport" << std::endl;
#endif
		cl_assert(XF86VidModeSetViewPort(dpy, scr, orig_view.x, orig_view.y) == True);
	}
	cur_width = DisplayWidth(dpy, scr);
	cur_height = DisplayHeight(dpy, scr);
	
	mode_set = false;
	XSync(dpy, True);
#endif
}

int CL_X11Resolution::get_width() const
{
	return cur_width;
}

int CL_X11Resolution::get_height() const
{
	return cur_height;
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

XF86VidModeModeInfo CL_X11Resolution::get_best_mode(int width, int height)
{
	XF86VidModeModeInfo **modes;
	XF86VidModeModeLine mode;
	int nmodes;
	int i;

	XF86VidModeModeInfo result;
	memset(&result, 0, sizeof(result));

	if (
		XF86VidModeGetModeLine(dpy, scr, &i, &mode) &&
		XF86VidModeGetAllModeLines(dpy, scr, &nmodes, &modes))
	{
		qsort(modes, nmodes, sizeof *modes, cmpmodes);
#ifdef DEBUG
		std::cout << "Available modes:" << std::endl;
		for (i=0; i<nmodes; ++i) 
		{
			std::cout << "Mode " << i << " : " <<  modes[i]->hdisplay << "x" <<  modes[i]->vdisplay << std::endl;
		}
#endif
		for (i=nmodes-1; i>= 0; --i) 
		{
			if (
				(modes[i]->hdisplay >= width) &&
				(modes[i]->vdisplay >= height) )
			{
				result = *modes[i];
				break;
			}
		}

   	    XFree(modes);
	}
	
	return result;
}
#endif /* USE_VIDMODE */

#endif
