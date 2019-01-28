/*
	$Id: sound.cpp,v 1.2 2002/02/17 00:45:24 mbn Exp $

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

#include <API/Sound/sound.h>
#include <API/Sound/cd_audio.h>

std::vector<CL_CDDrive *> CL_CDAudio::cd_drives;

std::list<CL_SoundCard*> CL_Sound::preload_cards;
std::vector<CL_SoundCard*> CL_Sound::cards;

void CL_Sound::clear_card_preload()
{
	CL_Sound::preload_cards.clear();
}

void CL_Sound::add_card_preload(CL_SoundCard *card)
{
	CL_Sound::preload_cards.push_front(card);
}

void CL_Sound::remove_card_preload(CL_SoundCard *card)
{
	CL_Sound::preload_cards.remove(card);
}

CL_StreamSoundProvider *CL_Sound::get_microphone()
{
	return get_current_card()->get_microphone();
}

CL_StreamSoundProvider *CL_Sound::get_line_in()
{
	return get_current_card()->get_line_in();
}

CL_SoundCard *CL_Sound::get_current_card()
{
	return cards[0];
}
