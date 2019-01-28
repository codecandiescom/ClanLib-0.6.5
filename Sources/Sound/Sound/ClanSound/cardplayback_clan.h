/*
	$Id: cardplayback_clan.h,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardplayback_clan
#define header_cardplayback_clan

#ifdef USE_CLANSOUND

#include "../Generic/cardsession_manager.h"
#include "soundcard_clan.h"

class CL_CardPlayback_ClanSound : public CL_CardSoundBuffer_Playback
{
protected:
	CL_CardPlayback_ClanSound(CL_SoundCard_ClanSound *card);

public:
	virtual ~CL_CardPlayback_ClanSound();
	
	// Mixer interface:
	// ----------------
	
	virtual void get_playback_data(int *data, int num_samples)=0;
	virtual void mix_to(int *data, int num_samples)=0;

protected:
	CL_SoundCard_ClanSound *card;
};

#endif /* USE_CLANSOUND */

#endif
