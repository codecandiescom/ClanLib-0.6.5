/*
	$Id: sound.h,v 1.8 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>Sound support. If you want sound in your application, you defiantly
//: want to link to this library.</p>
//! Global=Sound

#ifndef header_sound_api
#define header_sound_api

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Sound/setupsound.h"
#include "Sound/sound.h"
#include "Sound/soundformat.h"
#include "Sound/static_soundprovider.h"
#include "Sound/stream_soundprovider.h"
#include "Sound/soundbuffer_session.h"
#include "Sound/soundbuffer.h"
#include "Sound/cd_audio.h"
#include "Sound/soundfilter.h"

#include "Sound/SoundProviders/static_provider_wave.h"
#include "Sound/SoundProviders/static_provider_raw.h"
#include "Sound/SoundProviders/stream_provider_wave.h"
#include "Sound/SoundProviders/stream_provider_raw.h"

#include "Sound/SoundFilters/echofilter.h"
#include "Sound/SoundFilters/inverse_echofilter.h"
#include "Sound/SoundFilters/fadefilter.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanSound.lib")
#else
#pragma comment(lib, "clanSoundd.lib")
#endif
#endif

#endif
