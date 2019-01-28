/*
	$Id: soundbuffer_static.cpp,v 1.6 2001/10/01 20:57:05 mbn Exp $

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

#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include <API/Sound/sound.h>
#include <API/Sound/soundbuffer.h>
#include <API/Sound/soundbuffer_session.h>
#include <API/Sound/static_soundprovider.h>
#include <Sound/Sound/Generic/cardsession_manager.h>
#include <Sound/Sound/Generic/cardsoundbuffer_static.h>
#include <Sound/Sound/Generic/soundbuffer_static.h>
#include <Sound/Sound/Generic/soundcard_generic.h>
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>

CL_SoundBuffer_Generic_Static::CL_SoundBuffer_Generic_Static(
	CL_StaticSoundProvider *_provider,
	bool _delete_provider)
{
	provider = _provider;
	delete_provider = _delete_provider;
	
	int num_cards = CL_Sound::cards.size();
	card_buffers = new CL_CardSoundBuffer_Static*[num_cards];
	for (int i=0; i<num_cards; i++) card_buffers[i] = NULL;

	volume = -1; 
	pan = -1; 
	frequency = -1; 
}

CL_SoundBuffer_Generic_Static::~CL_SoundBuffer_Generic_Static()
{
	int num_cards = CL_Sound::cards.size();
	for (int i=0; i<num_cards; i++) delete card_buffers[i];
	if (delete_provider) delete provider;
}

CL_StaticSoundProvider *CL_SoundBuffer_Generic_Static::get_static_provider() const
{
	return provider;
}

int CL_SoundBuffer_Generic_Static::get_length() const
{
	return provider->data_size();
}

int CL_SoundBuffer_Generic_Static::get_num_samples() const 
{
	switch (provider->get_format())
	{
	case sf_8bit_signed:
		return provider->data_size();
	case sf_8bit_signed_stereo:
	case sf_16bit_signed:
		return provider->data_size()/2;

	case sf_16bit_signed_stereo:
		return provider->data_size()/4;
	}
	
	cl_assert(false); // invalid sample format.

	return -1;
}

int CL_SoundBuffer_Generic_Static::get_frequency() const
{
	return frequency;
}

bool CL_SoundBuffer_Generic_Static::set_frequency(int new_freq)
{
	frequency = new_freq;
	return true;
}

float CL_SoundBuffer_Generic_Static::get_volume() const
{
	return volume;
}

bool CL_SoundBuffer_Generic_Static::set_volume(float new_volume)
{
	volume = new_volume;
	return true;
}

float CL_SoundBuffer_Generic_Static::get_pan() const
{
	return pan;
}

bool CL_SoundBuffer_Generic_Static::set_pan(float new_pan)
{
	pan = new_pan;
	return true;
}

bool CL_SoundBuffer_Generic_Static::is_playing(CL_SoundBuffer_Session **session, CL_SoundCard *card) const
{
	if (session != NULL)
	{
		return (*session)->is_playing();
	}
	else
	{
		if (card != NULL)
		{
			return ((CL_SoundCard_Generic *) card)->manager->is_soundbuffer_playing((CL_SoundBuffer *) this);
		}
		else
		{
			if (CL_Sound::get_current_card() != NULL)
			{
				return ((CL_SoundCard_Generic *) CL_Sound::get_current_card())->manager->is_soundbuffer_playing((CL_SoundBuffer *) this);
			}
			else
			{
				return false;
			} 
		}
	}
	return true;
}

void CL_SoundBuffer_Generic_Static::stop(CL_SoundCard *card)
{
	if (card != NULL)
	{
		((CL_SoundCard_Generic *) card)->manager->remove_soundbuffer_playbacks(this);
	}
	else
	{
		if (CL_Sound::get_current_card() != NULL)
		{
			((CL_SoundCard_Generic *) CL_Sound::get_current_card())->manager->remove_soundbuffer_playbacks(this);
		}
	}
}

CL_SoundBuffer_Session CL_SoundBuffer_Generic_Static::play(bool looping, CL_SoundCard *card)
{
	if (card == NULL) card = CL_Sound::get_current_card();

	CL_SoundBuffer_Session ret = prepare(looping, card);
	ret.set_looping(looping);
	ret.play();

	return ret;
}

CL_SoundBuffer_Session CL_SoundBuffer_Generic_Static::prepare(bool looping, CL_SoundCard *card)
{
	if (card == NULL) card = CL_Sound::get_current_card();
	if (card == NULL) throw CL_Error("Cannot access sound card.  Please make sure it's not in use");

	if (card_buffers[card->card_no] == NULL)
	{
		provider->lock();

		volume = 1;
		pan = 0; 
		frequency = provider->get_frequency(); 

		card_buffers[card->card_no] =
			((CL_SoundCard_Generic *) card)->create_soundbuffer_static(this, provider);

		provider->unlock();
	}

	CL_SoundBuffer_Session ret(
		CL_SoundBuffer_Session(
			((CL_SoundCard_Generic *) card)->manager->add_playback(
				card_buffers[card->card_no]->prepare(), this),
			card));

	ret.set_volume(volume);
	ret.set_frequency(frequency);
	ret.set_pan(pan);

	return ret;
}
