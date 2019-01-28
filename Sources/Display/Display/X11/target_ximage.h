/*
	$Id: target_ximage.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_ximage
#define header_target_ximage

#ifdef USE_X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "API/Display/Display/target.h"

class CL_Target_XImage : public CL_Target
{
public:
	virtual void put_image(int x, int y, Drawable win, GC gc)=0;
	virtual void put_image(int x, int y, const class CL_Rect &rect, Drawable win, GC gc)=0;
};

#endif /*USE_X11*/

#endif
