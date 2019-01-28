/*
	$Id: setupsound.h,v 1.8 2002/01/22 10:14:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="System"
//! header=sound.h

#ifndef header_setupsound
#define header_setupsound

//: ClanSound initialization functions.
class CL_SetupSound
{
public:
//! Operations:
	//: Initialize the clanSound library.
	static void init(bool register_resources_only = false);
	
	//: Deinitialize the clanSound library.
	static void deinit();
};

//: ClanCDAudio init functions
class CL_SetupCDAudio
{
public:
//! Operations:
	//: Initialize the clanCDAudio extensions
	static void init(bool register_resources_only = false);
	
	//: Deinitialize the clanCDAudio extensions
	static void deinit();
};

#endif
