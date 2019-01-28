/*
	$Id: soundcard_be.cpp,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Core/Sound/static_soundprovider.h>
#include <API/Core/Sound/stream_soundprovider.h>
#include "API/Core/System/cl_assert.h"
#include <Core/Sound/Generic/cardsoundbuffer_playback.h>
#include <Core/Sound/Generic/cardsoundbuffer_static.h>
#include <Core/Sound/Generic/soundbuffer_stream.h>
#include <Core/Sound/Be/soundbuffer_static_be.h>
#include <Core/Sound/Be/soundbuffer_stream_be.h>
#include <Core/Sound/Be/soundcard_be.h>

CL_SoundCard_Be::CL_SoundCard_Be()
{
	name = "Be soundcard";
}

CL_SoundCard_Be::~CL_SoundCard_Be()
{
}

// Functions inherited from API:
// -----------------------------

CL_StreamSoundProvider *CL_SoundCard_Be::get_microphone()
{
	return NULL;
}

CL_StreamSoundProvider *CL_SoundCard_Be::get_line_in()
{
	return NULL;
}

void CL_SoundCard_Be::stop_all()
{
}

void CL_SoundCard_Be::set_global_volume(int /*volume*/)
{
}

void CL_SoundCard_Be::set_global_pan(int /*pan*/)
{
}

// Functions inherited from Generic:
// ---------------------------------

CL_CardSoundBuffer_Static *CL_SoundCard_Be::create_soundbuffer_static(
		CL_StaticSoundProvider *provider)
{
	return new CL_CardBuffer_Static_Be(provider);
}

CL_CardSoundBuffer_Playback *CL_SoundCard_Be::create_cardsoundbuffer_playback_streamed(
		CL_SoundBuffer_Generic_Stream *provider,
		CL_SoundPlayBackDesc * /*desc*/)
{
	return new CL_CardPlayback_Be_Stream(
		provider->get_stream_provider());
}
