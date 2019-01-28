/*
	$Id: glx_target.cpp,v 1.3 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "glx_target.h"
#include "displaycard_glx.h"

const char *CL_GLXTarget::get_long_name()
{
	return "OpenGL GLX";
}

const char *CL_GLXTarget::get_short_name()
{
	return "glx";
}

CL_XWindow_CompatibleCard *CL_GLXTarget::create_displaycard(
	Display *dpy,
	Window root,
	int display_counter)
{
	return new CL_GLX_DisplayCard(dpy, root, display_counter);
}
