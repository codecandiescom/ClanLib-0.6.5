/*
	$Id: png.h,v 1.6 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>PNG image loading support.</p>
//! Global=PNG

#ifndef header_png
#define header_png

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "PNG/provider_png.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanPNG.lib")
#else
#pragma comment(lib, "clanPNGd.lib")
#endif
#endif

#endif
