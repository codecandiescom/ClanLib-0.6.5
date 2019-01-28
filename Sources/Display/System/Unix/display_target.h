/*
	$Id: display_target.h,v 1.2 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_target
#define header_display_target

class CL_DisplayTarget
{
public:
	virtual const char *get_long_name()=0;
	// eg. Linux FBDEV target

	virtual const char *get_short_name()=0;
	// eg. fbdev

	virtual void init()=0;
	// called when display target has been selected and is to be activated.
	// (in CL_SetupDisplay::init())

	virtual void deinit()=0;
	// called when display target is to be shut down (in CL_SetupDisplay::deinit()).
};

#endif
