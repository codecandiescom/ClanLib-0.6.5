/*
	$Id: application.h,v 1.5 2002/03/01 22:00:15 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>Application bootstrapping library. When booting your application,
//: you can either use the low level, platform dependent approach in clanCore,
//: or you can use the CL_ClanApplication interface in clanApp.</p>
//! Global=App

#ifndef header_application
#define header_application

#include "Application/clanapp.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanApp.lib")
#else
#pragma comment(lib, "clanAppd.lib")
#endif
#endif

#endif
