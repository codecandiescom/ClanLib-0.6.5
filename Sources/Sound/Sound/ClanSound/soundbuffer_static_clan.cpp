/*
	$Id: soundbuffer_static_clan.cpp,v 1.11 2001/09/24 20:21:38 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sound card impl. that uses the ClanSound library.

*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#ifdef USE_CLANSOUND

#include <API/Sound/static_soundprovider.h>
#include "API/Core/System/cl_assert.h"
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>
#include <Sound/Sound/Generic/cardsoundbuffer_static.h>
#include <Sound/Sound/ClanSound/soundbuffer_static_clan.h>
#include <Sound/Sound/ClanSound/playback_static.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>
#include "../Generic/soundbuffer_generic.h"

CL_CardBuffer_Static_ClanSound::CL_CardBuffer_Static_ClanSound(
	CL_SoundCard_ClanSound *card,
	CL_StaticSoundProvider *provider,
	CL_SoundBuffer_Generic *owner)
: CL_CardSoundBuffer_Static(provider)
{
	this->owner = owner;
	this->card = card;

	format = sf_16bit_signed_stereo;

	provider->lock();
	void *input = provider->get_data();
	int size = provider->data_size();
	switch (provider->get_format())
	{
	case sf_8bit_signed:
		{
			char *input_c = (char *) input;
			num_samples = size;
			data = new short[num_samples*2];
			short *data16 = get_data16();
			for (int i=0; i<num_samples; i++)
			{
				data16[i*2+0] = (((int) input_c[i]))*256;
				data16[i*2+1] = (((int) input_c[i]))*256;
			}
		}
		break;  
		
	case sf_8bit_signed_stereo:
		{
			char *input_c = (char *) input;
			num_samples = size/2;
			data = new short[num_samples*2];
			short *data16 = get_data16();
			for (int i=0; i<num_samples; i++)
			{
				data16[i*2+0] = ((int) input_c[i*2])*256;
				data16[i*2+1] = ((int) input_c[i*2+1])*256;
			}
		}
		break;
		
	case sf_16bit_signed:
		{
			short *input_s = (short *) input;
			num_samples = size/2;
			data = new short[num_samples*2];
			short *data16 = get_data16();
			for (int i=0; i<num_samples; i++)
			{
				data16[i*2+0] = input_s[i];
				data16[i*2+1] = input_s[i];
			}
		}
		break;
		
	case sf_16bit_signed_stereo:
		{
			num_samples = size/4;
			data = new short[num_samples*2];
			memcpy(data, input, size);
		}
		break;
		
	default:
		cl_assert(false);
	}
	
	provider->unlock();
}

CL_CardBuffer_Static_ClanSound::~CL_CardBuffer_Static_ClanSound()
{
	delete[] data;
	data = NULL;
	card = NULL;
}

CL_CardSoundBuffer_Playback *CL_CardBuffer_Static_ClanSound::prepare()
{
	return new CL_Playback_Static(card, this);
}

#endif
