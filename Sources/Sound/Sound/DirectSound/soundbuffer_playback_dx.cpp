/*
	$Id: soundbuffer_playback_dx.cpp,v 1.2 2001/09/08 19:24:21 japj Exp $

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
#include "API/Core/Sound/static_soundprovider.h"

#include "soundbuffer_playback_dx.h"
#include "soundbuffer_static_dx.h"

CL_CardSoundBuffer_Playback_DX::CL_CardSoundBuffer_Playback_DX(CL_CardSoundBufferStatic_DX *_parent_buffer)
{
	parent_buffer = _parent_buffer;
	looping = false;

	if (parent_buffer->parent_card->soundcard != NULL)
	{
		HRESULT err = parent_buffer->parent_card->soundcard->DuplicateSoundBuffer(parent_buffer->soundbuffer, &buffer);
		if (err != DS_OK) 
		{
			buffer = NULL;
		}
	}
	else
	{
		buffer = NULL;
	}
}

CL_CardSoundBuffer_Playback_DX::~CL_CardSoundBuffer_Playback_DX()
{
	if (buffer != NULL) 
	{
		std::cout << "Released duplicate soundbuffer..." << std::endl;
		buffer->Release();
		buffer = NULL;
	}
}

void CL_CardSoundBuffer_Playback_DX::check_for_lost()
{
	if (buffer == NULL) return;

	DWORD status;
	buffer->GetStatus(&status);
	if (status == DSBSTATUS_BUFFERLOST)
	{
		std::cout << "Soundbuffer lost" << std::endl;
		parent_buffer->reload_data(true);
	}
}

int CL_CardSoundBuffer_Playback_DX::get_position()
{
	if (buffer == NULL) return -1;

	DWORD res, dummy;

	buffer->GetCurrentPosition(&res, &dummy);

	return res;
}

float CL_CardSoundBuffer_Playback_DX::get_position_relative()
{
	if (buffer == NULL) return (float) -1;

	DWORD res, dummy;

	HRESULT err = buffer->GetCurrentPosition(&res, &dummy);

	return float(res) / get_length();
}

bool CL_CardSoundBuffer_Playback_DX::set_position(int new_pos)
{
	if (buffer == NULL) return false;

	HRESULT err = buffer->SetCurrentPosition(new_pos);
	if (err != DS_OK) return false;
	return true;
}

bool CL_CardSoundBuffer_Playback_DX::set_position_relative(float new_pos)
{
	if (buffer == NULL) return false;

	HRESULT err = buffer->SetCurrentPosition(int(new_pos*get_length()));
	if (err != DS_OK) return false;
	return true;
}

int CL_CardSoundBuffer_Playback_DX::get_length()
{
	if (buffer == NULL) return -1;

	return parent_buffer->provider->data_size();
}

CL_SoundBuffer_Generic *CL_CardSoundBuffer_Playback_DX::get_owner()
{
//This should be inline
	return parent_buffer->buffer;
}

int CL_CardSoundBuffer_Playback_DX::get_frequency()
{
	if (buffer == NULL) return -1;

	DWORD ret;
	buffer->GetFrequency(&ret);
	return ret;
}

bool CL_CardSoundBuffer_Playback_DX::set_frequency(int new_freq)
{
	if (buffer == NULL) return false;

	HRESULT err = buffer->SetFrequency(new_freq);
	if (err != DS_OK) { std::cout << "SetFreq failed" << std::endl; return false; }

	return true;
}

float CL_CardSoundBuffer_Playback_DX::get_volume()
{
	if (buffer == NULL) return -1;

	long temp;
	buffer->GetVolume(&temp);

	return pow(10, temp/20);
}

bool CL_CardSoundBuffer_Playback_DX::set_volume(float new_volume)
{
	if (buffer == NULL) return false;

	if (new_volume == 0) buffer->SetVolume(DSBVOLUME_MIN);
	else buffer->SetVolume(100*20*log10(new_volume));

	return true;
}

float CL_CardSoundBuffer_Playback_DX::get_pan()
{
	if (buffer == NULL) return -1;

	long ret;
	buffer->GetPan(&ret);

	return ret;
}

bool CL_CardSoundBuffer_Playback_DX::set_pan(float new_pan)
{
	if (buffer == NULL) return false;

	if (new_pan < 0)
		buffer->SetPan(DSBPAN_CENTER-DSBPAN_LEFT*new_pan);
	else
		buffer->SetPan(DSBPAN_CENTER+DSBPAN_RIGHT*new_pan);

	return true;
}

void CL_CardSoundBuffer_Playback_DX::play()
{
	if (buffer == NULL) return;

	check_for_lost();
	HRESULT err;

	int flags = 0;
	if (looping) flags = DSBPLAY_LOOPING;
	err = buffer->Play(0, 0, flags);

	cl_assert(err == DS_OK);
}

void CL_CardSoundBuffer_Playback_DX::stop()
{
	if (buffer == NULL) return;

	buffer->Stop();
}

bool CL_CardSoundBuffer_Playback_DX::is_playing()
{
	if (buffer == NULL) return false;

	DWORD status;
	buffer->GetStatus(&status);

	return (status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING;
}

void CL_CardSoundBuffer_Playback_DX::set_looping(bool loop)
{
	if (buffer == NULL) return;

	looping = loop;
}
