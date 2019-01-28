/*
	$Id: soundbuffer_stream_dx.cpp,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>
#include "API/Core/System/cl_assert.h"

#include "soundbuffer_stream_dx.h"

#define STREAM_BUFFER_LENGTH_SECS 2

CL_CardSoundBufferPlaybackStream_DX::CL_CardSoundBufferPlaybackStream_DX(
	CL_SoundCard_DX *card,
	CL_SoundBuffer_Generic_Stream *parent_buffer)
{
	buffer = parent_buffer;
	parent_card = card;
	provider = parent_buffer->get_stream_provider();
	provider_session = provider->begin_session();

	int num_channels;
	int bps;

	switch (provider_session->get_format())
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
	format.nSamplesPerSec = provider_session->get_frequency(); 
	format.wBitsPerSample = bps; 
	format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8; 
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign; 
	format.cbSize = 0; 
 
	DSBUFFERDESC desc;
	desc.dwSize = sizeof(DSBUFFERDESC); 
	desc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	//	DSBCAPS_CTRLDEFAULT has been removed in DirectX7

	desc.dwBufferBytes = format.nAvgBytesPerSec*STREAM_BUFFER_LENGTH_SECS; 
	desc.dwReserved = 0; 
	desc.lpwfxFormat = &format; 

	size_buffer = desc.dwBufferBytes;
	secure_distance = int(float(format.nAvgBytesPerSec) / 10);
	fill_position = 0;

	soundbuffer = NULL;

	if (parent_card->soundcard != NULL)
	{
		HRESULT err = parent_card->soundcard->CreateSoundBuffer(&desc, &soundbuffer, NULL);
	}

	sound_is_playing = false;
	empty_pos = -1;

	update_buffer();
}

CL_CardSoundBufferPlaybackStream_DX::~CL_CardSoundBufferPlaybackStream_DX()
{
	provider->end_session(provider_session);
	if (soundbuffer != NULL) soundbuffer->Release();
}

int CL_CardSoundBufferPlaybackStream_DX::get_position()
{
	return -1;
}

float CL_CardSoundBufferPlaybackStream_DX::get_position_relative()
{
	return -1;
}

bool CL_CardSoundBufferPlaybackStream_DX::set_position(int new_pos)
{
	return false;
}


bool CL_CardSoundBufferPlaybackStream_DX::set_position_relative(float new_pos)
{
	return false;
}

int CL_CardSoundBufferPlaybackStream_DX::get_length()
{
	return -1;
}

CL_SoundBuffer_Generic *CL_CardSoundBufferPlaybackStream_DX::get_owner()
{
	return buffer;
}

int CL_CardSoundBufferPlaybackStream_DX::get_frequency()
{
	if (soundbuffer == NULL) return -1;

	DWORD ret;
	soundbuffer->GetFrequency(&ret);
	return ret;
}

bool CL_CardSoundBufferPlaybackStream_DX::set_frequency(int new_freq)
{
	if (soundbuffer == NULL) return false;

	HRESULT err = soundbuffer->SetFrequency(new_freq);
	if (err != DS_OK) { return false; }

	return true;
}

float CL_CardSoundBufferPlaybackStream_DX::get_volume()
{
	if (soundbuffer == NULL) return -1;

	long temp;
	soundbuffer->GetVolume(&temp);

	return pow(10, temp/20);
}

bool CL_CardSoundBufferPlaybackStream_DX::set_volume(float new_volume)
{
	if (soundbuffer == NULL) return false;

	if (new_volume == 0) soundbuffer->SetVolume(DSBVOLUME_MIN);
	else soundbuffer->SetVolume(100*20*log10(new_volume));

	return true;
}

float CL_CardSoundBufferPlaybackStream_DX::get_pan()
{
	if (soundbuffer == NULL) return -1;

	long ret;
	soundbuffer->GetPan(&ret);

	return ret;
}

bool CL_CardSoundBufferPlaybackStream_DX::set_pan(float new_pan)
{
	if (soundbuffer == NULL) return false;

	if (new_pan < 0)
		soundbuffer->SetPan(DSBPAN_CENTER-DSBPAN_LEFT*new_pan);
	else
		soundbuffer->SetPan(DSBPAN_CENTER+DSBPAN_RIGHT*new_pan);

	return true;
}

void CL_CardSoundBufferPlaybackStream_DX::play()
{
	if (soundbuffer == NULL) return;

	soundbuffer->SetCurrentPosition(0);
	soundbuffer->Play(0, 0, DSBPLAY_LOOPING);
	sound_is_playing = true;
}

void CL_CardSoundBufferPlaybackStream_DX::stop()
{
	if (soundbuffer == NULL) return;

	soundbuffer->Stop();
	sound_is_playing = false;
}

bool CL_CardSoundBufferPlaybackStream_DX::is_playing()
{
	if (soundbuffer == NULL) return false;

	DWORD status;
	soundbuffer->GetStatus(&status);

	return (status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING;
}

void CL_CardSoundBufferPlaybackStream_DX::set_looping(bool loop)
{
	if (soundbuffer == NULL) return;
}

char *get_dsounderr(HRESULT);
void CL_CardSoundBufferPlaybackStream_DX::update_buffer()
{
	if (soundbuffer == NULL) return;

	unsigned char *ptr1, *ptr2;
	DWORD size1, size2;
	DWORD written1 = 0;
	DWORD written2 = 0;
	DWORD playpos, writepos;
	HRESULT err;
	
	err = soundbuffer->GetCurrentPosition(&playpos, &writepos);
	cl_assert(err == DS_OK);

	if (playpos > fill_position) fill_length = playpos - fill_position - secure_distance;
	else fill_length = (size_buffer-fill_position) + playpos - secure_distance;

	if (fill_length > secure_distance)
	{
		err = soundbuffer->Lock(fill_position, fill_length, (void **) &ptr1, &size1, (void **) &ptr2, &size2, 0);
		if (err != DS_OK)
		{
			std::cout << "SHIT! We are going down!..." << std::endl;
			std::cout << "fill_position: " << fill_position << ", fill_length: " << fill_length << std::endl;
			std::cout << "size_buffer: " << size_buffer << ", playpos: " << playpos;
			std::cout << ", writepos: " << writepos << std::endl;
			char *error = get_dsounderr(err);
			std::cout << error << std::endl;
		}
		cl_assert(err == DS_OK);

		written1 = provider_session->get_data(ptr1, size1);
		if (provider_session->get_format() == sf_8bit_signed)
		{
			for (DWORD i=0;i<written1;i++) ptr1[i] = (unsigned char) (short(ptr1[i])+128);
		}
		if (written1 == size1)
		{
			written2 = provider_session->get_data(ptr2, size2);
			if (provider_session->get_format() == sf_8bit_signed)
			{
				for (DWORD i=0;i<written2;i++) ptr2[i] = (unsigned char) (short(ptr2[i])+128);
			}
			if (written2 != size2)
			{
				if (provider_session->get_format() == sf_8bit_signed) memset(&ptr2[written2], 128, size2-written2);
				else memset(&ptr2[written2], 0, size2-written2);
				written2 = size2;
			}
		}
		else
		{
			if (provider_session->get_format() == sf_8bit_signed) 
			{
				memset(&ptr1[written1], 128, size1-written1);
				written1 = size1;
				memset(ptr2, 128, size2);
				written2 = size2;
			}
			else
			{
				memset(&ptr1[written1], 0, size1-written1);
				written1 = size1;
				memset(ptr2, 0, size2);
				written2 = size2;
			}
		}

		soundbuffer->Unlock(ptr1, written1, ptr2, written2);

		fill_position += fill_length;
		if (fill_position >= size_buffer) fill_position -= size_buffer;

		if (provider_session->eof() && empty_pos == -1)
		{
			empty_pos = fill_position;
			wait_for_play_wrap = playpos >= empty_pos;
		}

		if (empty_pos != -1)
		{
			if (wait_for_play_wrap && playpos < empty_pos) wait_for_play_wrap = false;
			if (!wait_for_play_wrap && playpos >= empty_pos)
			{
				soundbuffer->Stop();
				sound_is_playing = false;
			}
		}
	}
}

void CL_CardSoundBufferPlaybackStream_DX::keep_alive()
{
	if (sound_is_playing)
	{
		update_buffer();
	}
}
