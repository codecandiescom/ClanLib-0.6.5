/*
	$Id: glx_target.h,v 1.4 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_glx_target
#define header_glx_target

#include "Display/System/Unix/x11_target.h"

class CL_GLXTarget : public CL_X11Target
{
public:
	virtual const char *get_long_name();

	virtual const char *get_short_name();

	virtual CL_XWindow_CompatibleCard *create_displaycard(Display *dpy, Window root, int display_counter);
};

#endif
