/*
	$Id: ttf.h,v 1.6 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>ClanTTF provides a wrapper around freetype to give access to anti-aliased TrueType fonts for use in ClanLib.</p>
//! Global=TTF

#ifndef major_ttf_header
#define major_ttf_header

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "TTF/setupttf.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanTTF.lib")
#else
#pragma comment(lib, "clanTTFd.lib")
#endif
#endif

#endif
