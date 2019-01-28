/*
	$Id: setupttf.h,v 1.8 2002/01/22 10:14:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanTTF="System"
//! header=ttf.h

#ifndef header_setupttf
#define header_setupttf

//: ClanTTF initialization functions
class CL_SetupTTF
{
public:
//! Operations:
	//: Initialize the TTF library
	static void init(bool register_resources_only = false);

	//: Deinitialize the TTF library
	static void deinit();
};

#endif 
