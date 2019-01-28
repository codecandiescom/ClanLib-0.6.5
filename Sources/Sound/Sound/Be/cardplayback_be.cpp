/*
	$Id: cardplayback_be.cpp,v 1.1 2001/03/06 15:09:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <math.h>

#include <Core/Sound/Be/cardplayback_be.h>

CL_CardPlayback_Be::CL_CardPlayback_Be(BSimpleGameSound *buffer)
{
	if (buffer)
	{
		if (buffer->IsPlaying())
		{
			soundbuffer = (BSimpleGameSound*)buffer->Clone();
		} else
		{
			soundbuffer = buffer;
		}
		if (soundbuffer->InitCheck() != B_OK)
		{
			std::cout << "Something went wrong with sound initialization" << std::endl;
			delete soundbuffer;
			soundbuffer = NULL;
		}
	} else
	{
		soundbuffer = NULL;
	}
}

CL_CardPlayback_Be::~CL_CardPlayback_Be()
{
	std::cout << "deleted" << std::endl;
	if (soundbuffer)
		delete soundbuffer;
}

// Functions inherited from Generic:
// ---------------------------------

int CL_CardPlayback_Be::get_position()
{
	return -1;
}

float CL_CardPlayback_Be::get_position_relative()
{
	return 0;
}

bool CL_CardPlayback_Be::set_position(int new_pos)
{
	return false;
}

bool CL_CardPlayback_Be::set_position_relative(float new_pos)
{
	return false;
}

int CL_CardPlayback_Be::get_length()
{
	return 0;
}

int CL_CardPlayback_Be::get_frequency()
{
	if (soundbuffer==NULL) return 0;

	gs_attribute attr;
	attr.attribute = B_GS_SAMPLING_RATE;
	
	soundbuffer->GetAttributes(&attr,1);
	return (int)attr.value;
}

bool CL_CardPlayback_Be::set_frequency(int new_freq)
{
	if (soundbuffer==NULL) return 0;

	gs_attribute attr;
	attr.attribute = B_GS_SAMPLING_RATE;
	attr.value = new_freq;

	return (soundbuffer->SetAttributes(&attr,1) == B_OK);
}

float CL_CardPlayback_Be::get_volume()
{
	if (soundbuffer==NULL) return 0;

	return soundbuffer->Gain();
}

bool CL_CardPlayback_Be::set_volume(float new_volume)
{
	if (soundbuffer==NULL) return 0;

	return (soundbuffer->SetGain(new_volume) == B_OK);
}

float CL_CardPlayback_Be::get_pan()
{
	if (soundbuffer==NULL) return 0;

	return soundbuffer->Pan();
}

bool CL_CardPlayback_Be::set_pan(float new_pan)
{
	if (soundbuffer==NULL) return 0;

	return (soundbuffer->SetPan(new_pan) == B_OK);
}

CL_SoundPlayBackDesc *CL_CardPlayback_Be::add_playback_desc(
	CL_SoundPlayBackDesc * desc)
{
	return desc;
}

bool CL_CardPlayback_Be::remove_playback_desc(
	CL_SoundPlayBackDesc * /*desc*/)
{
	return true;
}

void CL_CardPlayback_Be::play()
{
	if (soundbuffer==NULL) return;

	soundbuffer->StartPlaying();
}

void CL_CardPlayback_Be::stop()
{
	if (soundbuffer==NULL) return;

	soundbuffer->StopPlaying();
}

bool CL_CardPlayback_Be::is_playing()
{
	return soundbuffer->IsPlaying();
}
