/*
	$Id: soundbuffer_stream.cpp,v 1.5 2001/06/12 21:44:56 mbn Exp $

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

#include <API/Core/System/error.h>
#include <API/Sound/sound.h>
#include <API/Sound/soundbuffer.h>
#include <API/Sound/soundbuffer_session.h>
#include <API/Sound/stream_soundprovider.h>
#include <Sound/Sound/Generic/cardsession_manager.h>
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>
#include <Sound/Sound/Generic/soundbuffer_stream.h>
#include <Sound/Sound/Generic/soundcard_generic.h>

CL_SoundBuffer_Generic_Stream::CL_SoundBuffer_Generic_Stream(
	CL_StreamSoundProvider *_provider,
	bool _delete_provider)
{
	provider = _provider;
	delete_provider = _delete_provider;

	volume = -1; 
	pan = -1; 
	frequency = -1; 
}

CL_SoundBuffer_Generic_Stream::~CL_SoundBuffer_Generic_Stream()
{
/*
	The playback session manager reference counts the soundbuffers currently being
	played. This means that the constructor should never be called unless all
	reference throughout all clanlib (and the application) of the sound buffer has
	been released. Therefore the following code should not be needed, and in fact
	causes a problem with recursive cleanup if enabled.

	-- mbn 12. june 2001

	int num = CL_Sound::cards.size();
	for (int i=0; i<num; i++)
	{
		CL_SoundCard_Generic *gen = (CL_SoundCard_Generic *) CL_Sound::cards[i];
		gen->manager->remove_soundbuffer_playbacks(this);
	}
*/
	if (delete_provider) delete provider;
}

CL_StreamSoundProvider *CL_SoundBuffer_Generic_Stream::get_stream_provider() const
{
	return provider;
}

int CL_SoundBuffer_Generic_Stream::get_length() const
{
	return -1;
}

int CL_SoundBuffer_Generic_Stream::get_num_samples() const
{
	return -1;
}

int CL_SoundBuffer_Generic_Stream::get_frequency() const
{
	return frequency;
}

bool CL_SoundBuffer_Generic_Stream::set_frequency(int new_freq)
{
	frequency = new_freq;
	return true;
}

float CL_SoundBuffer_Generic_Stream::get_volume() const
{
	return volume;
}

bool CL_SoundBuffer_Generic_Stream::set_volume(float new_volume)
{
	volume = new_volume;
	return true;
}

float CL_SoundBuffer_Generic_Stream::get_pan() const
{
	return pan;
}

bool CL_SoundBuffer_Generic_Stream::set_pan(float new_pan)
{
	pan = new_pan;
	return true;
}

bool CL_SoundBuffer_Generic_Stream::is_playing(CL_SoundBuffer_Session **session, CL_SoundCard *card) const
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

void CL_SoundBuffer_Generic_Stream::stop(CL_SoundCard *card)
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

CL_SoundBuffer_Session CL_SoundBuffer_Generic_Stream::play(bool looping, CL_SoundCard *card)
{
	if (card == NULL) card = CL_Sound::get_current_card();

	CL_SoundBuffer_Session ret(prepare(looping, card));
	ret.set_looping(looping);
	ret.play();

	return ret;
}

CL_SoundBuffer_Session CL_SoundBuffer_Generic_Stream::prepare(bool looping, CL_SoundCard *card)
{
	if(card == NULL) card = CL_Sound::get_current_card();
	if(card == NULL)
	{ // If it's NULL then we can't do jack shit
		throw CL_Error("Cannot access sound card.  Please make sure it's not in use");
	}
	CL_CardSoundBuffer_Playback *playback =
		((CL_SoundCard_Generic *) card)->
			create_cardsoundbuffer_playback_streamed(this);

	CL_SoundBuffer_Session ret(
		CL_SoundBuffer_Session(
			((CL_SoundCard_Generic *) card)->manager->add_playback(playback, this), card));

	if (pan == -1) pan = 0;
	if (volume == -1) volume = 1;

	ret.set_volume(volume);
	ret.set_pan(pan);
	ret.set_looping(looping);
	if (frequency != -1) ret.set_frequency(frequency);

	return ret;
}
