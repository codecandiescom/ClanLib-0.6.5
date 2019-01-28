/*
	$Id: mikmod.h,v 1.7 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>MikMod module playback support. Supports all the module formats of
//: MikMod, which includes MOD, XM, S3M, IT and more.</p>
//! Global=MikMod

#ifndef header_mikmod
#define header_mikmod

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "MikMod/setupmikmod.h"
#include "MikMod/streamed_mikmod_sample.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanMikMod.lib")
#else
#pragma comment(lib, "clanMikModd.lib")
#endif
#endif

#endif
