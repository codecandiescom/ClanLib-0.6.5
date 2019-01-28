/*
	$Id: vorbis.h,v 1.7 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>Ogg Vorbis playback. With this library, you can play music with the same
//: quality as todays mp3, but with a completely open sound format (mp3 is
//: patented by some greedy companies that want money for each mp3 you use in a
//: commercial product).</p>
//! Global=Vorbis

#ifndef header_vorbis
#define header_vorbis

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Vorbis/vorbis_soundprovider.h"
#include "Vorbis/setupvorbis.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanVorbis.lib")
#else
#pragma comment(lib, "clanVorbisd.lib")
#endif
#endif

#endif
