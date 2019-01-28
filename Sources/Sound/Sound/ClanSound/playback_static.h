/*
	$Id: playback_static.h,v 1.3 2001/06/20 12:35:08 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_playback_static
#define header_playback_static

#ifdef USE_CLANSOUND

#include "cardplayback_clan.h"
#include "soundbuffer_static_clan.h"

class CL_Playback_Static : public CL_CardPlayback_ClanSound
{
public:
	CL_Playback_Static(
		CL_SoundCard_ClanSound *card,
		CL_CardBuffer_Static_ClanSound *static_buffer);

	virtual ~CL_Playback_Static();

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
		return static_buffer->get_owner();
	}

	// Mixer interface (from cardplayback_clansound):
	// ----------------------------------------------
	
	virtual void get_playback_data(int *data, int num_samples);
	virtual void mix_to(int *data, int num_samples);

private:
	CL_CardBuffer_Static_ClanSound *static_buffer;

	void get_playback_data_8bit_signed(int *data, int num_samples);
	void get_playback_data_8bit_signed_stereo(int *data, int num_samples);
	void get_playback_data_16bit_signed(int *data, int num_samples);
	void get_playback_data_16bit_signed_stereo(int *data, int num_samples);

	float volume;
	float pan;
	int frequency;
	double pos;
	bool playing;
	bool looping;
};

#endif /* USE_CLANSOUND */
#endif
