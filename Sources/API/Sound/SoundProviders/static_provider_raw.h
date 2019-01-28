/*
	$Id: static_provider_raw.h,v 1.7 2002/06/13 14:43:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Simple sample support.
*/

//! clanSound="Sound Providers"
//! header=sound.h

#ifndef header_static_provider_raw
#define header_static_provider_raw

#include <iostream>
#include "../static_soundprovider.h"
#include "../soundbuffer.h"
#include "../../Core/System/cl_assert.h"

//: Static sample in a raw PCM format (no header in file).
class CL_Sample_RawData : public CL_StaticSoundProvider
{
public:
//! Construction:
	//: Create
	static CL_SoundBuffer *create(
		unsigned char *sound_data,
		int data_size,
		int bytes_per_sample,
		bool stereo,
		int frequency=22050)
	{
		return CL_SoundBuffer::create(
			new CL_Sample_RawData(
				sound_data,
				data_size,
				bytes_per_sample,
				stereo,
				frequency),
			true);
	}

	//: Sample RawData Constructor
	CL_Sample_RawData(
		unsigned char *sound_data,
		int data_size,
		int bytes_per_sample,
		bool stereo,
		int frequency=22050)
	{
		load_data(
			sound_data,
			data_size,
			bytes_per_sample,
			stereo,
			frequency);
	}

	//: Sample RawData Destructor
	virtual ~CL_Sample_RawData()
	{
		delete[] sample_data;
	}

//! Operations:
	//: Lock
	virtual void lock()
	{
		return;
	}

	//: Unlock
	virtual void unlock()
	{
	}

//! Attributes:
	//: Get format
	virtual SoundFormat get_format() const
	{
		return sample_format;
	}

	//: Data size
	virtual int data_size() const
	{
		return sample_size;
	}

	//: Get data
	virtual void *get_data() const
	{
		return sample_data;
	}

	//: Get frequency
	virtual int get_frequency() const
	{
		return sample_freq;
	}
//! Implementation:
private:
	struct WAVE_FORMAT
	{
		short formatTag;
		short nChannels;
		int nSamplesPerSec;
		int nAvgBytesPerSec;
		short nBlockAlign;
	};

	unsigned char *sample_data;
	SoundFormat sample_format;
	int sample_size;
	int sample_freq;

	void load_data(
		unsigned char *sound_data,
		int data_size,
		int bytes_per_sample,
		bool stereo,
		int frequency=22050)
	{

		sample_size = data_size;
		sample_freq = frequency;

		sample_data = new unsigned char[sample_size];
		memcpy(sample_data, sound_data, sample_size);

		if (stereo && bytes_per_sample == 2) sample_format = sf_16bit_signed_stereo;
		else if (stereo && bytes_per_sample == 1) sample_format = sf_8bit_signed_stereo;
		else if (!stereo && bytes_per_sample == 2) sample_format = sf_16bit_signed;
		else if (!stereo && bytes_per_sample == 1) sample_format = sf_8bit_signed;
		else
		{
//			std::cout << "Invalid sample format" << std::endl;
			cl_assert(false);
		}
	}
};

#endif
