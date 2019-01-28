/*
	$Id: playback_static.cpp,v 1.9 2001/10/01 20:42:17 mbn Exp $

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

#ifdef USE_CLANSOUND

#include <Sound/Sound/ClanSound/soundbuffer_static_clan.h>
#include <Sound/Sound/ClanSound/cardplayback_clan.h>
#include <Sound/Sound/ClanSound/playback_static.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>

#include <cmath>

CL_Playback_Static::CL_Playback_Static(
	CL_SoundCard_ClanSound *card,
	CL_CardBuffer_Static_ClanSound *static_buffer)
: CL_CardPlayback_ClanSound(card)
{
	volume = 1.0f;
	pan = 0.0f;
	frequency = 0;
	pos = 0;
	playing = false;
	looping = false;
	
	this->static_buffer = static_buffer;
	card->add(this);
}

CL_Playback_Static::~CL_Playback_Static()
{
	card->remove(this);
	static_buffer = NULL;
}

void CL_Playback_Static::mix_to(int *data, int num_samples)
{
	static int temp[16*1024*2];

	for (int i=0; i<num_samples; i+=16*1024)
	{
		int size = num_samples-i;
		if (size > 16*1024) size = 16*1024;
		
		get_playback_data(temp, size);
		filter(temp, size);
		for (int j=0; j<size*2; j++) data[i+j] += temp[j];
	}
}

// Functions inherited from Generic:
// ---------------------------------

int CL_Playback_Static::get_position()
{
	return (int) pos;
}

float CL_Playback_Static::get_position_relative()
{
	return pos / (float) static_buffer->get_size();
}

bool CL_Playback_Static::set_position(int new_pos)
{
	pos = new_pos;

	return true;
}

bool CL_Playback_Static::set_position_relative(float new_pos)
{
	pos = new_pos * static_buffer->get_size();
	if (pos < 0) pos = 0;
	if (pos > static_buffer->get_size()) pos = static_buffer->get_size();

	return true;
}

int CL_Playback_Static::get_length()
{
	return static_buffer->get_size();
}

int CL_Playback_Static::get_frequency()
{
	return frequency;
}

bool CL_Playback_Static::set_frequency(int new_freq)
{
	frequency = new_freq;
	return true;
}

float CL_Playback_Static::get_volume()
{
	return volume;
}

bool CL_Playback_Static::set_volume(float new_volume)
{
	volume = new_volume;
//	cs_set_volume(session_id, (int) (new_volume*128));
	return true;
}

float CL_Playback_Static::get_pan()
{
	return pan;
}

bool CL_Playback_Static::set_pan(float new_pan)
{
	pan = new_pan;
	return true;
}

void CL_Playback_Static::set_looping(bool new_looping)
{
	looping = new_looping;
}

bool CL_Playback_Static::get_looping()
{
	return false;
}

void CL_Playback_Static::play()
{
	playing = true;
}

void CL_Playback_Static::stop()
{
	playing = false;
}

bool CL_Playback_Static::is_playing()
{
	return playing;
}

// Mixer interface:
// ----------------

void CL_Playback_Static::get_playback_data(int *data, int num_samples)
{
	switch (static_buffer->get_format())
	{
	case sf_8bit_signed:
		get_playback_data_8bit_signed(data, num_samples);
		break;

	case sf_8bit_signed_stereo:
		get_playback_data_8bit_signed_stereo(data, num_samples);
		break;

	case sf_16bit_signed:
		get_playback_data_16bit_signed(data, num_samples);
		break;

	case sf_16bit_signed_stereo:
		get_playback_data_16bit_signed_stereo(data, num_samples);
		break;
	}
}

void CL_Playback_Static::get_playback_data_8bit_signed(int *data, int num_samples)
{
}

void CL_Playback_Static::get_playback_data_8bit_signed_stereo(int *data, int num_samples)
{
}

void CL_Playback_Static::get_playback_data_16bit_signed(int *data, int num_samples)
{
}

void CL_Playback_Static::get_playback_data_16bit_signed_stereo(int *data, int num_samples)
{
	int i;
	int freq = frequency;
	int vol = (int) (volume*128);

	if (freq == 0 || vol==0 || playing == false)
	{
		memset(data, 0, num_samples*sizeof(int)*2);
		return;
	}

	double speed = freq/(double) 22050;
	double fill_samples = (static_buffer->get_size()-pos) / speed;

	if (fill_samples >= num_samples) fill_samples = num_samples;
	else if (!looping) // reached end of buffer - pad the rest with silence
	{
		memset(
			data,
			0,
			sizeof(int)*num_samples*2);
		
		playing = false;
	}

	int *ptr = data;
	short *src_data = static_buffer->get_data16();
	for (i=0; i<fill_samples; i++)
	{
		int cur_pos = (int) pos;

		// debug: (remove this one if it hasn't occurred in a long
		// time.
		// cs_assert(cur_pos < static_buffer->get_size());

		*(ptr++) = src_data[cur_pos*2+0];
		*(ptr++) = src_data[cur_pos*2+1];
		
		pos += speed;
	}

	// apply volume and panning:
	ptr = data;
	int left_pan = (int) (128-pan*128);
	int right_pan = (int) (128+pan*128);
	if (left_pan < 0) left_pan = 0;
	if (left_pan > 128) left_pan = 128;
	if (right_pan < 0) right_pan = 0;
	if (right_pan > 128) right_pan = 128;

	for (i=0; i<fill_samples; i++)
	{
		// left channel:
		(*ptr) = (*ptr)*vol*left_pan/16384; ptr++;
		
		// right channel:
		(*ptr) = (*ptr)*vol*right_pan/16384; ptr++;
	}

	if (!playing) // stopped playing because buffer ended
	{
		pos = 0; // is this the behaviour of a directsound buffer? -- dok
	}

	if (looping && fill_samples < num_samples)
	{
		pos -= static_buffer->get_size();
		get_playback_data(ptr, (int)(num_samples - fill_samples));
	}
}

#endif

