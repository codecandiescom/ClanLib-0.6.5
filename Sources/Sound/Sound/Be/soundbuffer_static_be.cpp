/*
	$Id: soundbuffer_static_be.cpp,v 1.1 2001/03/06 15:09:24 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#include "Core/precomp.h"

#include <API/Core/Sound/static_soundprovider.h>
#include <Core/Sound/Be/soundbuffer_static_be.h>
#include <Core/Sound/Be/cardplayback_be.h>
#include <Core/Sound/Generic/cardsoundbuffer_playback.h>
#include <Core/Sound/Generic/cardsoundbuffer_static.h>

CL_CardBuffer_Static_Be::CL_CardBuffer_Static_Be(
	CL_StaticSoundProvider *provider)
: CL_CardSoundBuffer_Static(provider)
{
	int frame_count = 0;
	gs_audio_format format;
	
	soundbuffer = NULL;
	
	switch (provider->get_format())
	{
	case sf_8bit_signed:
		format.format = gs_audio_format::B_GS_U8;
		format.channel_count = 1;
		frame_count = provider->data_size();
		break;
	case sf_8bit_signed_stereo:
		format.format = gs_audio_format::B_GS_U8;
		format.channel_count = 2;
		frame_count = provider->data_size() / 2;
		break;
	case sf_16bit_signed:
		format.format = gs_audio_format::B_GS_S16;
		format.channel_count = 1;
		frame_count = provider->data_size() / 2;
		break;
	case sf_16bit_signed_stereo:
		format.format = gs_audio_format::B_GS_S16;
		format.channel_count = 2;
		frame_count = provider->data_size() / 4;
		break;
	}
	
	format.frame_rate = provider->get_frequency();
	format.buffer_size = provider->data_size();
//	format.byte_order = B_LITTLE_ENDIAN; //FIXME

	provider->lock();

	char *data = (char*)provider->get_data();
	if (format.format == gs_audio_format::B_GS_U8)
	{
		unsigned char *new_data = new unsigned char[provider->data_size()];

		char *pdata = data;
		unsigned char *bpos = new_data;
		for (int i=0;i<provider->data_size();i++)
		{
			*(bpos++) = (unsigned char) ((short)(*pdata++)+128);
		}
		
		data = (char*)new_data;
	}

	soundbuffer = new BSimpleGameSound(data,frame_count,&format);
	if (soundbuffer->InitCheck() != B_OK)
	{
		std::cout << "Something went wrong with sound initialization" << std::endl;
		delete soundbuffer;
		soundbuffer = NULL;
	}
	
	provider->unlock();

	if (format.format == gs_audio_format::B_GS_U8)
	{
		delete data;
	}
}

CL_CardBuffer_Static_Be::~CL_CardBuffer_Static_Be()
{
	if (soundbuffer)
		delete soundbuffer;
}

CL_CardSoundBuffer_Playback *CL_CardBuffer_Static_Be::prepare(
	CL_SoundPlayBackDesc * /*desc*/)
{
	return new CL_CardPlayback_Be(soundbuffer);
}
