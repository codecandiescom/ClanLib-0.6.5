/*
	$Id: soundbuffer_playback_dx.h,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_soundbuffer_playback_dx
#define header_soundbuffer_playback_dx

#include "Core/Sound/Generic/cardsoundbuffer_playback.h"

class CL_CardSoundBufferStatic_DX;
class CL_CardSoundBuffer_Playback_DX : public CL_CardSoundBuffer_Playback
{
	LPDIRECTSOUNDBUFFER buffer;
	CL_CardSoundBufferStatic_DX *parent_buffer;
	bool looping;
	void check_for_lost();
public:
	CL_CardSoundBuffer_Playback_DX(CL_CardSoundBufferStatic_DX *parent_buffer);
	virtual ~CL_CardSoundBuffer_Playback_DX();

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

	virtual void play();
	virtual void stop();

	virtual bool is_playing();
	virtual void set_looping(bool loop);

	virtual CL_SoundBuffer_Generic *get_owner();
};

#endif
