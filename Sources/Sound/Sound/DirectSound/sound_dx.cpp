/*
	$Id: sound_dx.cpp,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>

#include "sound_dx.h"
#include "soundcard_dx.h"

CL_SoundCard *CL_Sound_DX::cur_card = NULL;

BOOL WINAPI EnumerateSoundDevices(LPGUID lpGuid, LPCSTR lpstrDescription, LPCSTR lpstrModule, LPVOID lpContext);

CL_Sound_DX::CL_Sound_DX()
{
	std::vector<SSoundCardInfo *> soundinfo;
	BOOL result = DirectSoundEnumerate(EnumerateSoundDevices, &soundinfo);

	int card_no=0;

	CL_Sound::cards.push_back(new CL_SoundCard_DX(card_no++, "Default device", NULL));

	std::vector<SSoundCardInfo *>::iterator it;
	int i=1;
	for (it=soundinfo.begin();it!=soundinfo.end();it++)
	{
		CL_Sound::cards.push_back(new CL_SoundCard_DX(card_no++, soundinfo[i-1]->name, &soundinfo[i-1]->guid));
		i++;
	}

	for (it=soundinfo.begin();it!=soundinfo.end();it++)
	{
		delete *it;
	}

	CL_Sound_DX::cur_card = CL_Sound::cards[0];
	CL_Sound::preload_cards.push_back(CL_Sound_DX::cur_card);

	std::cout << "ClanLib: Number of sound cards: " << CL_Sound::cards.size() << std::endl;
}

CL_Sound_DX::~CL_Sound_DX()
{
	for (std::vector<CL_SoundCard *>::iterator it=CL_Sound::cards.begin();it!=CL_Sound::cards.end();it++)
	{
		delete *it;
	}
}

BOOL WINAPI EnumerateSoundDevices(
	LPGUID lpGuid,						
	LPCSTR lpstrDescription,	
	LPCSTR lpstrModule,				
	LPVOID lpContext)
{
	if (lpGuid!=NULL)
	{
		std::cout << lpstrDescription << ", " << lpstrModule << std::endl;
		SSoundCardInfo *info = new SSoundCardInfo;
		memcpy(&info->guid, lpGuid, sizeof(GUID));
		info->name = lpstrDescription;
		((std::vector<SSoundCardInfo *> *) lpContext)->push_back(info);
	}
	return TRUE;
}

CL_StreamSoundProvider *CL_Sound::get_microphone()
{
	if (CL_Sound_DX::cur_card != NULL) return CL_Sound_DX::cur_card->get_microphone();
	return NULL;
}

CL_StreamSoundProvider *CL_Sound::get_line_in()
{
	if (CL_Sound_DX::cur_card != NULL) return CL_Sound_DX::cur_card->get_line_in();
	return NULL;
}

void CL_Sound::select_card(CL_SoundCard *card)
{
	CL_Sound_DX::cur_card = card;
}

void CL_Sound::select_card(int card_no)
{
	CL_Sound_DX::cur_card = CL_Sound::cards[card_no];
}

CL_SoundCard *CL_Sound::get_current_card()
{
	return CL_Sound_DX::cur_card;
}
