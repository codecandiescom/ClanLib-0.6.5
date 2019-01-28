/*
	$Id: setupcore.h,v 1.15 2002/01/22 10:14:00 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_setupcore
#define header_setupcore

#ifdef WIN32
#include <windows.h>
#endif

//: ClanCore initialization functions.
class CL_SetupCore
{
public:
//! Operations:
	//: Initialize clanCore.
	//: <p>If you are using CL_ClanApplication to boot your
	//: application, you do not need to call this function.</p>
	//: <p>If using Win32 (and without CL_ClanApplication) -
	//: You must call CL_SetupCore::set_instance(HINSTANCE hInstance)
	//: before this calling this function</p>
	static void init(bool register_resources_only = false);

	//: Deinitialize clanCore.
	//: <p>If you are using CL_ClanApplication to boot your
	//: application, you do not need to call this function.</p>
	static void deinit();

#ifdef WIN32
	//: Set instance
	//: <p>If doing win32 initialization without using clanApp, you must call
	//: this function before calling CL_SetupCore::init().</p>
	static void set_instance(HINSTANCE hInstance);
#endif
};

#endif
