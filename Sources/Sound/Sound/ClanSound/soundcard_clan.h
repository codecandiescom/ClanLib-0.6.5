/*
	$Id: soundcard_clan.h,v 1.2 2002/02/12 20:37:06 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_soundcard_clan
#define header_soundcard_clan

#ifdef USE_CLANSOUND

#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "Sound/Sound/Generic/soundcard_generic.h"
#include "mixer.h"

#ifdef WIN32
#include "directsound_recorder.h"
#endif

class CL_SoundCard_ClanSound : public CL_SoundCard_Generic, CL_Runnable
{
public:
	CL_SoundCard_ClanSound();
	virtual ~CL_SoundCard_ClanSound();

	// Member vars from CL_SoundCard:
//	CL_String name;
//	int card_no;

	// Functions inherited from API:
	// -----------------------------
	virtual CL_StreamSoundProvider *get_microphone();
	virtual CL_StreamSoundProvider *get_line_in();

	virtual void stop_all();
	virtual void set_global_volume(int volume);
	virtual void set_global_pan(int pan);

	// Functions inherited from Generic:
	// ---------------------------------
	virtual CL_CardSoundBuffer_Static *create_soundbuffer_static(
		CL_SoundBuffer_Generic *owner,
		CL_StaticSoundProvider *provider);
		
	virtual CL_CardSoundBuffer_Playback *create_cardsoundbuffer_playback_streamed(
		CL_SoundBuffer_Generic_Stream *provider);

	// Functions exported to CL_CardSoundBuffer_Playback:
	// --------------------------------------------------
	void add(CL_CardPlayback_ClanSound *playback);
	void remove(CL_CardPlayback_ClanSound *playback);

	CL_Mutex *get_mutex();

private:
	CL_CSMixer mixer;
	CL_Mutex *mutex;
	CL_Thread *thread;
	volatile bool exit_thread;
	
	virtual void run();
	// Runs the mixer sound thread.

#ifdef WIN32
	CL_DirectSoundRecorder *microphone;
	CL_DirectSoundRecorder *line_in;
#endif
};

#endif /* USE_CLANSOUND */

#endif
