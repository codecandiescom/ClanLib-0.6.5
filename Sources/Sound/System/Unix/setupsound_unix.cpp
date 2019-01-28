/*
	$Id: setupsound_unix.cpp,v 1.7 2002/01/22 10:14:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Sound/Sound/ClanSound/cdaudio_linux.h"
#include "Sound/System/Generic/setupsound_generic.h"
#include "API/Sound/setupsound.h"
#include "API/Sound/sound.h"
#include "Sound/Sound/ClanSound/soundcard_clan.h"

static int sound_ref_count = 0;
static class CL_SoundCard *cl_current_soundcard = 0;

void CL_SetupSound::init(bool register_resources_only)
{
	sound_ref_count++;
	if (sound_ref_count > 1) return;

	CL_SetupSound_Generic::init();

	if (register_resources_only) return;

	// todo: new clansound here.
	CL_Sound::cards.push_back(new CL_SoundCard_ClanSound);

	if (CL_Sound::cards.size() > 0) cl_current_soundcard = CL_Sound::cards[0];
}

void CL_SetupSound::deinit()
{
	sound_ref_count--;
	if (sound_ref_count > 0) return;

	int num_cards = CL_Sound::cards.size();
	for (int i=0; i<num_cards; i++) delete CL_Sound::cards[i]; 
	CL_Sound::cards.clear();
//	CL_Sound::num_cards = 0;

	CL_SetupSound_Generic::deinit();
}

void CL_SetupCDAudio::init(bool register_resources_only)
{
	if (register_resources_only) return;

	CL_CDDrive_Linux::init_cdaudio();
}

void CL_SetupCDAudio::deinit()
{
	// Do nothing
	return;
}

void CL_Sound::select_card(CL_SoundCard *card)
{
	cl_current_soundcard = card;
}

/*
CL_SoundCard *CL_Sound::get_current_card()
{
	cl_assert(cl_current_soundcard != NULL);
	return cl_current_soundcard;
}
*/
