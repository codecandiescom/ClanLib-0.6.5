/*
	$Id: setupgui.h,v 1.16 2002/01/22 10:14:01 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="System"
//! header=gui.h

#ifndef header_setupgui
#define header_setupgui

//: Setup GUI Class
class CL_SetupGUI
{
//! Operations:
public:
	//: Init the GUI.
	static void init(bool register_resources_only = false);

	//: Deinit the GUI.
	static void deinit();
};

#endif
