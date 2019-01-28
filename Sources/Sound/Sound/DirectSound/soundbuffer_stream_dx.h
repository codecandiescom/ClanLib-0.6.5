/*
	$Id: soundbuffer_stream_dx.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_soundbuffer_stream_dx
#define header_soundbuffer_stream_dx

#include "API/Core/Sound/stream_soundprovider.h"
#include "Core/Sound/Generic/cardsoundbuffer_playback.h"
#include "Core/Sound/Generic/soundcard_generic.h"
#include "Core/Sound/Generic/soundbuffer_stream.h"
#include "API/Core/System/keep_alive.h"
#include "soundcard_dx.h"

class CL_CardSoundBufferPlaybackStream_DX : public CL_CardSoundBuffer_Playback, CL_KeepAlive
{
	CL_SoundCard_DX *parent_card;
	LPDIRECTSOUNDBUFFER soundbuffer;
	CL_StreamSoundProvider *provider;
	CL_StreamSoundProvider_Session *provider_session;
	CL_SoundBuffer_Generic_Stream *buffer;

	bool sound_is_playing;
	DWORD size_buffer;
	int cur_write_pos;
	DWORD fill_length;
	DWORD fill_position;
	DWORD secure_distance;

	bool wait_for_play_wrap;
	DWORD empty_pos;

	void update_buffer();
public:

	CL_CardSoundBufferPlaybackStream_DX(
		CL_SoundCard_DX *card,
		CL_SoundBuffer_Generic_Stream *soundbuffer);
	virtual ~CL_CardSoundBufferPlaybackStream_DX();

	// CL_CardSoundBuffer_Playback overloads
	virtual int get_position();
	virtual float get_position_relative();

	virtual bool set_position(int new_pos);
	virtual bool set_position_relative(float new_pos);

	virtual int get_length();

	virtual int get_frequency();
	virtual bool set_frequency(int new_freq);

	virtual float get_volume();
	virtual bool set_volume(float new_volume);

	virtual float get_pan();
	virtual bool set_pan(float new_pan);

	virtual void play();
	virtual void stop();

	virtual bool is_playing();
	virtual void set_looping(bool loop);

	virtual void keep_alive();



	virtual CL_SoundBuffer_Generic *get_owner();
};

#endif