/*
	$Id: cardplayback_be.h,v 1.1 2001/03/06 15:09:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardplayback_be
#define header_cardplayback_be

#include "../Generic/cardsession_manager.h"
#include "soundcard_be.h"

class CL_CardPlayback_Be : public CL_CardSoundBuffer_Playback
{
protected:
	BSimpleGameSound* soundbuffer;

public:
	CL_CardPlayback_Be(BSimpleGameSound* buffer);
	virtual ~CL_CardPlayback_Be();

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

	virtual CL_SoundPlayBackDesc *add_playback_desc(CL_SoundPlayBackDesc *desc);
	virtual bool remove_playback_desc(CL_SoundPlayBackDesc *desc);

	virtual void play();
	virtual void stop();

	virtual bool is_playing();
};

#endif
