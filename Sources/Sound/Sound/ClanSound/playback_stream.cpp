/*
	$Id: playback_stream.cpp,v 1.8 2002/02/01 22:04:54 mbn Exp $

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

#include <API/Core/System/mutex.h>
#include <API/Sound/stream_soundprovider.h>
#include "API/Core/System/cl_assert.h"
#include <Sound/Sound/ClanSound/cardplayback_clan.h>
#include <Sound/Sound/ClanSound/playback_stream.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>

CL_Playback_Stream::CL_Playback_Stream(
	CL_SoundCard_ClanSound *card,
	CL_StreamSoundProvider *_provider,
	CL_SoundBuffer_Generic *owner)
: CL_CardPlayback_ClanSound(card)
{
	this->owner = owner;
	provider = _provider;
	provider_session = provider->begin_session();
	
	frequency = provider_session->get_frequency();
	volume = 256;

	format = provider_session->get_format();
	pos = 0;
	playing = false;
	filled_pos = 0;
	first_time = true;
	
	buffer_size = frequency/4;
	ring_buffer = new short[buffer_size*2];
	stream_eof = false;

	card->add(this);
}

CL_Playback_Stream::~CL_Playback_Stream()
{
	card->remove(this);

	delete[] ring_buffer;
	ring_buffer = NULL;
	provider->end_session(provider_session);
}

void CL_Playback_Stream::keep_alive()
{
	// if eof stream, don't ask any new data since that doesn't make sense
	if (!stream_eof)
	{
		int bytes_wanted = ask_update();
		if (bytes_wanted == 0) return;

		char *copy_buffer = new char[bytes_wanted];
		
		int bytes_given = provider_session->get_data(copy_buffer, bytes_wanted);

		if (bytes_given > 0) update(copy_buffer, bytes_given);
		delete[] copy_buffer;

		if (provider_session->eof()) 
			stream_eof = true;
	}
}

int CL_Playback_Stream::get_position()
{
	return (int) pos;
}

float CL_Playback_Stream::get_position_relative()
{
	return pos / get_length();
}

bool CL_Playback_Stream::set_position_relative(float new_pos)
{
	return set_position((int) (new_pos * get_length()));
}

int CL_Playback_Stream::get_length()
{
	return -1;
}

int CL_Playback_Stream::get_frequency()
{
	return frequency;
}

bool CL_Playback_Stream::set_frequency(int new_freq)
{
	frequency = new_freq;
	return true;
}

float CL_Playback_Stream::get_volume()
{
	return volume;
}

bool CL_Playback_Stream::set_volume(float new_volume)
{
	volume = new_volume;
	return true;
}

float CL_Playback_Stream::get_pan()
{
	return pan;
}

bool CL_Playback_Stream::set_pan(float new_pan)
{
	pan = new_pan;
	return true;
}

void CL_Playback_Stream::set_looping(bool looping)
{
	if (looping == false) return;
	static bool warning = true;

	if (warning)
	{
		cl_info(info_sound, "Not implemented yet.");
		warning = false;
	}
}

bool CL_Playback_Stream::get_looping()
{
	return false;
}

void CL_Playback_Stream::play()
{
	playing = true;
}

void CL_Playback_Stream::stop()
{
	playing = false;
}

bool CL_Playback_Stream::is_playing()
{
	return playing;
}

bool CL_Playback_Stream::set_position(int new_pos)
{
	if (provider_session->set_position(new_pos))
	{
		flush();
		return true;
	}
	
	return false;
}

void CL_Playback_Stream::flush()
{
}

void CL_Playback_Stream::get_playback_data(int *data, int num_samples)
{
	int i;
	int freq = frequency;
	int vol = (int) (volume*128);

	if (freq == 0 || vol == 0)
	{
		memset(data, 0, num_samples*sizeof(int)*2);
		return;
	}

	double speed = freq/(double) 22050;

	keep_alive(); // Ask for data.

/*
	int samples_available = (int) (get_samples_available()*speed);
	if (num_samples > samples_available) // clear the rest of the buffer
	{
		memset(&data[samples_available], 0, num_samples-samples_available);
		num_samples = samples_available;
		
		if (stream_eof) playing = false; // reached end of stream.
	}
*/

	// pos is volatile, so we make a copy of it to make the compiler optimize better.
	double cached_pos = pos;
	int *ptr = data;
	for (i=0; i<num_samples; i++)
	{
		int cur_pos = (int) cached_pos;

		if (cur_pos == filled_pos) // out of data. fill with zeroes until we get some.
		{
			ring_buffer[cur_pos*2+0] = 0;
			ring_buffer[cur_pos*2+1] = 0;

			if (stream_eof) playing = false;
			continue;
		}

		if (cur_pos < buffer_size)
		{
			*(ptr++) = ring_buffer[cur_pos*2+0];
			*(ptr++) = ring_buffer[cur_pos*2+1];
		}

		cached_pos += speed;
		if (cur_pos < filled_pos && cached_pos >= filled_pos)
		{
			cached_pos = filled_pos;
			if (stream_eof) playing = false;
		}

		if (cur_pos != (int) cached_pos)
		{
			ring_buffer[cur_pos*2+0] = 0;
			ring_buffer[cur_pos*2+1] = 0;
		}

		if (cached_pos >= buffer_size)
		{
			cached_pos -= buffer_size;
			if (cached_pos >= filled_pos)
			{
				cached_pos = filled_pos;
				if (stream_eof) playing = false;
			}
		}
	}
	pos = cached_pos;

//	if (num_samples >= samples_available) pos = filled_pos; // reached end of buffer

	// apply volume and panning:
	ptr = data;
	int left_pan = int (128-pan*128);
	int right_pan = int (128+pan*128);
	if (left_pan < 0) left_pan = 0;
	if (left_pan > 128) left_pan = 128;
	if (right_pan < 0) right_pan = 0;
	if (right_pan > 128) right_pan = 128;

	for (i=0; i<num_samples; i++)
	{
		// left channel:
		(*ptr) = (*ptr)*vol*left_pan/16384; ptr++;
	     		
		// right channel:
		(*ptr) = (*ptr)*vol*right_pan/16384; ptr++;
	}
}

void CL_Playback_Stream::mix_to(int *data, int num_samples)
{
	if (playing == false) return;

	static int temp[16*1024*2];

	for (int i=0; i<num_samples; i+=16*1024)
	{
		int size = num_samples-i;
		if (size > 16*1024) size = 16*1024;
		
		get_playback_data(temp, size);
		filter(temp, size);
		int k = i*2;
		for (int j=0; j<size*2; j++) data[k+j] += temp[j];
	}
}

int CL_Playback_Stream::get_samples_available()
{
	if (pos < filled_pos) return (int) (pos+(buffer_size-filled_pos));
	if (pos > filled_pos) return (int) (filled_pos - pos);

	return 0;
}

int CL_Playback_Stream::ask_update()
{
//	CL_MutexSection(card->get_mutex());

	int _filled_pos = filled_pos;
	int _pos = (int) pos;

	int space_left = buffer_size;
	
	if (_pos < _filled_pos) space_left = (int) (buffer_size-_filled_pos+_pos);
	if (_pos > _filled_pos) space_left = (int) (_pos-_filled_pos);

//	std::cout << "read: " << _pos << ", write: " << _filled_pos << ", space left: " << space_left << std::endl;

	space_left -= 16;
	if (space_left <= buffer_size/5) return 0; // only fill if more than 20% of buffer is empty.

	switch (format)
	{
	case sf_8bit_signed:
		break;
	
	case sf_8bit_signed_stereo:
	case sf_16bit_signed:
		space_left *= 2;
		break;
		
	case sf_16bit_signed_stereo:
		space_left *= 4;
		break;
		
	default:
		cl_assert(false);
	}
	
	return space_left;
}

int CL_Playback_Stream::update(void *data, int data_size)
{
//	CL_MutexSection(card->get_mutex());

	if (first_time)
	{
		first_time = false;
		filled_pos = (int) pos;
	}

	int num_samples = data_size;
	int sample_size=0;

	switch (format)
	{
	case sf_8bit_signed:
		sample_size = 1;
		break;
	
	case sf_8bit_signed_stereo:
	case sf_16bit_signed:
		sample_size = 2;
		break;
		
	case sf_16bit_signed_stereo:
		sample_size = 4;
		break;
		
	default:
		cl_assert(false);
	}
	
	num_samples /= sample_size;

	if (filled_pos+num_samples > buffer_size) // exceeds end of ring buffer
	{
		char *_data = (char *) data;

		int fill_length = buffer_size - filled_pos;
	
		update_format(filled_pos, fill_length, data);
		update_format(0, num_samples - fill_length, _data+fill_length*sample_size);
		
		filled_pos = num_samples - fill_length;
	}
	else
	{
		update_format(filled_pos, num_samples, data);
		filled_pos += num_samples;
	}

	return 0;
}

void CL_Playback_Stream::update_format(int pos, int num_samples, void *data)
{
	switch (format)
	{
	case sf_8bit_signed:
		{
			unsigned char *input_c = (unsigned char *) data;
			for (int i=0; i<num_samples; i++)
			{
				ring_buffer[(pos+i)*2+0] = input_c[i];
				ring_buffer[(pos+i)*2+1] = input_c[i];
			}
		}
		break;
	
	case sf_8bit_signed_stereo:
		{
			unsigned char *input_c = (unsigned char *) data;
			for (int i=0; i<num_samples; i++)
			{
				ring_buffer[(pos+i)*2+0] = input_c[i*2+0];
				ring_buffer[(pos+i)*2+1] = input_c[i*2+1];
			}
		}
		break;

	case sf_16bit_signed:
		{
			short *input_s = (short *) data;
			for (int i=0; i<num_samples; i++)
			{
				ring_buffer[(pos+i)*2+0] = input_s[i];
				ring_buffer[(pos+i)*2+1] = input_s[i];
			}
		}
		break;
		
	case sf_16bit_signed_stereo:
		{
			short *input_s = (short *) data;
			for (int i=0; i<num_samples; i++)
			{
				ring_buffer[(pos+i)*2+0] = input_s[i*2+0];
				ring_buffer[(pos+i)*2+1] = input_s[i*2+1];
			}
		}
		break;
		
	default:
		cl_assert(false);
	}
}

#endif
