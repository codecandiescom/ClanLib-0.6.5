/*
	$Id: setupmikmod.h,v 1.9 2002/01/22 10:14:01 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanMikMod="System"
//! header=mikmod.h

#ifndef header_setupmikmod
#define header_setupmikmod

//: ClanMikMod initialization.
class CL_SetupMikMod
{
public:
//! Operations:
	//: Call this function in init_modules() to use clanMikMod.
	static void init(bool register_resources_only = false);

	//: Call this function in deinit_modules().
	static void deinit();
};

#endif
