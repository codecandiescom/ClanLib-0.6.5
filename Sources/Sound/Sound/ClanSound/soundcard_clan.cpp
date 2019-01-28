/*
	$Id: soundcard_clan.cpp,v 1.4 2002/02/12 20:37:06 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#ifdef USE_CLANSOUND

#include <API/Core/System/mutex.h>
#include <API/Sound/static_soundprovider.h>
#include <API/Sound/stream_soundprovider.h>
#include <API/Core/System/thread.h>
#include "API/Core/System/cl_assert.h"
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>
#include <Sound/Sound/Generic/cardsoundbuffer_static.h>
#include <Sound/Sound/Generic/soundbuffer_stream.h>
#include <Sound/Sound/Generic/soundbuffer_generic.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>
#include <Sound/Sound/ClanSound/soundbuffer_static_clan.h>
#include <Sound/Sound/ClanSound/cardplayback_clan.h>
#include <Sound/Sound/ClanSound/playback_stream.h>

CL_SoundCard_ClanSound::CL_SoundCard_ClanSound()
{
	name = "Unnamed sound card";
	card_no = 0;

#ifdef WIN32
	microphone = new CL_DirectSoundRecorder(CL_DirectSoundRecorder::type_mic);
	line_in = new CL_DirectSoundRecorder(CL_DirectSoundRecorder::type_line_in);
#endif
	
	mutex = CL_Mutex::create();

	exit_thread = false;
	thread = CL_Thread::create(this);
	thread->start();
}

CL_SoundCard_ClanSound::~CL_SoundCard_ClanSound()
{
	mutex->enter();
	exit_thread = true;
	mutex->leave();
	thread->wait();
	delete thread;
	delete mutex;
	mutex = NULL;
	thread = NULL;

#ifdef WIN32
	delete microphone;
	delete line_in;
#endif
}

// Functions inherited from API:
// -----------------------------

CL_StreamSoundProvider *CL_SoundCard_ClanSound::get_microphone()
{
#ifdef WIN32
	return microphone;
#else
	return NULL;
#endif
}

CL_StreamSoundProvider *CL_SoundCard_ClanSound::get_line_in()
{
#ifdef WIN32
	return line_in;
#else
	return NULL;
#endif
}

void CL_SoundCard_ClanSound::stop_all()
{
	// we need a generic strategy for this
}

void CL_SoundCard_ClanSound::set_global_volume(int /*volume*/)
{
	// and this one...
}

void CL_SoundCard_ClanSound::set_global_pan(int /*pan*/)
{
	// and this...
}

// Functions inherited from Generic:
// ---------------------------------

CL_CardSoundBuffer_Static *CL_SoundCard_ClanSound::create_soundbuffer_static(
	CL_SoundBuffer_Generic *owner,
	CL_StaticSoundProvider *provider)
{
	return new CL_CardBuffer_Static_ClanSound(this, provider, owner);
}

CL_CardSoundBuffer_Playback *CL_SoundCard_ClanSound::create_cardsoundbuffer_playback_streamed(
	CL_SoundBuffer_Generic_Stream *provider)
{
	return new CL_Playback_Stream(
		this,
		provider->get_stream_provider(),
		provider);
}

// Functions exported to CL_CardSoundBuffer_Playback:
// --------------------------------------------------

void CL_SoundCard_ClanSound::add(CL_CardPlayback_ClanSound *playback)
{
	if (mutex == NULL) return;

	mutex->enter();
	mixer.add(playback);
	mutex->leave();
}

void CL_SoundCard_ClanSound::remove(CL_CardPlayback_ClanSound *playback)
{
	if (mutex == NULL) return;

	mutex->enter();
	mixer.remove(playback);
	mutex->leave();
}

CL_Mutex *CL_SoundCard_ClanSound::get_mutex()
{
	cl_assert(mutex != NULL);
	return mutex;
}

// Private functions:
// ------------------

void CL_SoundCard_ClanSound::run()
{
	while (true)
	{
		mixer.wait();

		if (exit_thread) break;
		mixer.mix(mutex);
	}
}

#endif
