/*
	$Id: soundbuffer_static_dx.cpp,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>
#include "API/Core/Sound/static_soundprovider.h"

#include "soundbuffer_playback_dx.h"
#include "soundbuffer_static_dx.h"

CL_CardSoundBufferStatic_DX::CL_CardSoundBufferStatic_DX(
	CL_SoundCard_DX *_parent_card,
	CL_SoundBuffer_Generic *buffer,
	CL_StaticSoundProvider *provider)
: CL_CardSoundBuffer_Static(provider)
{
	this->buffer = buffer;
	parent_card = _parent_card;

	int num_channels;
	int bps;
	HRESULT err;

	switch (provider->get_format())
	{
	case sf_8bit_signed:
		num_channels = 1;
		bps = 8;
		break;
	case sf_8bit_signed_stereo:
		num_channels = 2;
		bps = 8;
		break;
	case sf_16bit_signed:
		num_channels = 1;
		bps = 16;
		break;
	case sf_16bit_signed_stereo:
		num_channels = 2;
		bps = 16;
		break;
	}

	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM; 
	format.nChannels = num_channels; 
	format.nSamplesPerSec = provider->get_frequency(); 
	format.wBitsPerSample = bps; 
	format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8; 
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign; 
	format.cbSize = 0; 
 
	DSBUFFERDESC desc;
	desc.dwSize = sizeof(DSBUFFERDESC); 
	desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY; 
	// DSBCAPS_CTRLDEFAULT has been removed in DirectX7

	desc.dwBufferBytes = provider->data_size(); 
	desc.dwReserved = 0; 
	desc.lpwfxFormat = &format; 

	soundbuffer = NULL;

	if (parent_card->soundcard != NULL)
	{
		err = parent_card->soundcard->CreateSoundBuffer(&desc, &soundbuffer, NULL);
		if (err != DS_OK) std::cout << "ERROR! " << __FILE__ << ": " << __LINE__ << std::endl;
		reload_data(false);
	}
}

CL_CardSoundBufferStatic_DX::~CL_CardSoundBufferStatic_DX()
{
	if (soundbuffer != NULL) soundbuffer->Release();
}

void CL_CardSoundBufferStatic_DX::reload_data(bool lock_provider)
{
	if (soundbuffer == NULL) return;

	if (lock_provider) provider->lock();

	char *buffer1;
	DWORD num_bytes1;
	HRESULT err = soundbuffer->Lock(0, provider->data_size(), (void**) &buffer1, &num_bytes1, NULL, 0, 0);
	if (err != DS_OK) std::cout << "ERROR! " << __FILE__ << ": " << __LINE__ << std::endl;

	if (provider->get_format() == sf_8bit_signed)
	{
		char *pdata = (char *) provider->get_data();
		char *bpos = buffer1;
		for (int i=0;i<provider->data_size();i++)
		{
			*(bpos++) = (unsigned char) (short(*(pdata++))+128);
		}
	}
	else
	{
		memcpy(buffer1, provider->get_data(), provider->data_size());
	}

	err = soundbuffer->Unlock(buffer1, num_bytes1, NULL, 0);
	if (err != DS_OK) std::cout << "ERROR! " << __FILE__ << ": " << __LINE__ << std::endl;

	if (lock_provider) provider->unlock();
}

CL_CardSoundBuffer_Playback *CL_CardSoundBufferStatic_DX::prepare()
{
	return new CL_CardSoundBuffer_Playback_DX(this);
}
