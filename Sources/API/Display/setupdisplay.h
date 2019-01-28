/*
	$Id: setupdisplay.h,v 1.7 2002/01/22 10:14:00 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="System"
//! header=display.h

#ifndef header_setupdisplay
#define header_setupdisplay

//: ClanDisplay initialization functions.
class CL_SetupDisplay
{
public:
//! Construction:
	//: Initialize the clanDisplay library.
	static void init(bool register_resources_only = false);
	
	//: Deinitialize the clanDisplay library.
	static void deinit();
};

#endif
