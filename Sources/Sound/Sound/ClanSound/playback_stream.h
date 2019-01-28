/*
	$Id: playback_stream.h,v 1.2 2002/02/01 22:04:54 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#ifndef header_playback_stream
#define header_playback_stream

#ifdef USE_CLANSOUND

#include "API/Sound/stream_soundprovider.h"
#include "API/Core/System/keep_alive.h"
#include "soundcard_clan.h"
#include "cardplayback_clan.h"
#include "Core/System/Unix/init_linux.h"
#include "../Generic/soundbuffer_generic.h"

class CL_Playback_Stream
: public CL_CardPlayback_ClanSound//, CL_KeepAlive
{
public:
	CL_Playback_Stream(
		CL_SoundCard_ClanSound *card,
		CL_StreamSoundProvider *provider,
		CL_SoundBuffer_Generic *owner);
	virtual ~CL_Playback_Stream();

	virtual void keep_alive();

	// Functions inherited from Generic:
	// ---------------------------------
	virtual int get_position();
	virtual float get_position_relative();

	virtual bool set_position(int new_pos);
	virtual bool set_position_relative(float new_pos);

	virtual int get_length(); // -1 if unknown

	virtual int get_frequency();
	virtual bool set_frequency(int new_freq);

	virtual float get_volume();
	virtual bool set_volume(float new_volume);

	virtual float get_pan();
	virtual bool set_pan(float new_pan);

	virtual void set_looping(bool looping);
	virtual bool get_looping();

	virtual void play();
	virtual void stop();

	virtual bool is_playing();
	
	virtual CL_SoundBuffer_Generic *get_owner()
	{
		return owner;
	}

	// Mixer interface (from cardplayback_clansound):
	// ----------------------------------------------
	
	virtual void get_playback_data(int *data, int num_samples);
	virtual void mix_to(int *data, int num_samples);

private:
	int ask_update();
	int update(void *data, int data_size);
	void update_format(int pos, int num_samples, void *data);
	void flush();
	int get_samples_available();

private:
	CL_StreamSoundProvider *provider;
	CL_StreamSoundProvider_Session *provider_session;
	CL_SoundBuffer_Generic *owner;

	int format;
	float volume;
	float pan;
	int frequency;
	volatile double pos;

	bool playing;
	volatile int filled_pos;
	bool first_time;
	
	short *ring_buffer;
	int buffer_size;
	bool stream_eof;
};

#endif /* USE_CLANSOUND */

#endif
