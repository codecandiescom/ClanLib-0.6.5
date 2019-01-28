/*
	$Id: stream_provider_wave.cpp,v 1.6 2002/08/14 14:00:13 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Streamed sample

*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Sound/SoundProviders/stream_provider_wave.h"

#include <iostream>

/*****************************
      CL_Streamed_WaveSample
*****************************/

CL_SoundBuffer *CL_Streamed_WaveSample::create(
	const std::string &filename,
	CL_InputSourceProvider *inputprovider, 
	bool looped)
{
	return CL_SoundBuffer::create(
		new CL_Streamed_WaveSample(
			filename,
			inputprovider,
			looped),
		true);
}

CL_Streamed_WaveSample::CL_Streamed_WaveSample(const std::string &_filename, CL_InputSourceProvider *_inputprovider, bool _looped)
{
	filename = _filename;
	looped = _looped;

	if (_inputprovider == NULL)
	{
		inputprovider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		inputprovider = _inputprovider->clone();
	}
}

CL_Streamed_WaveSample::~CL_Streamed_WaveSample()
{
	delete inputprovider;
}

CL_StreamSoundProvider_Session *CL_Streamed_WaveSample::begin_session()
{
	return new CL_Streamed_WaveSample_Session(
		inputprovider->open_source(filename.c_str()),
		looped);
}

void CL_Streamed_WaveSample::end_session(CL_StreamSoundProvider_Session *session)
{
	delete session;
}

/*****************************
  CL_Streamed_WaveSample_Session
*****************************/

CL_Streamed_WaveSample_Session::CL_Streamed_WaveSample_Session(
	CL_InputSource *_input, bool _looped)
{
	input = _input;
	looped = _looped;
	cl_assert(input != NULL);

	// Check to see if this is really a .wav-file
	char temp[12];
	input->read(temp, 4);
	input->seek(4, CL_InputSource::seek_cur);
	input->read(&temp[4], 8);
	input->seek(4, CL_InputSource::seek_cur);

	if (memcmp(temp, "RIFFWAVEfmt ", 12) != 0)
		throw CL_Error("Invalid RIFF WAVE header");

//	cl_assert(!(memcmp(temp, "RIFFWAVEfmt ", 12)));

	WAVE_FORMAT format;
	format.formatTag = input->read_short16();
	format.nChannels = input->read_ushort16();
	format.nSamplesPerSec = input->read_uint32();
	format.nAvgBytesPerSec = input->read_uint32();
	format.nBlockAlign = input->read_ushort16();

	/* FIXME: find a better way to handle format struct padding */
	input->read(temp, 2);

	// Another sanity check
	input->read(temp, 4);
	temp[4] = 0;
	if (memcmp(temp, "data", 4) != 0)
		throw CL_Error("Invalid RIFF data header");
//	cl_assert(!(memcmp(temp, "data", 4)));

	sample_size = input->read_uint32();
	sample_freq = format.nSamplesPerSec;

	int bytes_per_sample = format.nAvgBytesPerSec / format.nSamplesPerSec;

	if (format.nChannels == 2 && bytes_per_sample == 4) sample_format = sf_16bit_signed_stereo;
	else if (format.nChannels == 2 && bytes_per_sample == 2) sample_format = sf_8bit_signed_stereo;
	else if (format.nChannels == 1 && bytes_per_sample == 2) sample_format = sf_16bit_signed;
	else if (format.nChannels == 1 && bytes_per_sample == 1) sample_format = sf_8bit_signed;
	else
	{
		std::cout << "    Invalid wave file format         " << std::endl;
		std::cout << "---------------------------------" << std::endl;
		std::cout << "Sample size: " << sample_size << std::endl;
		std::cout << "Sample frequency: " << sample_freq << std::endl;
		std::cout << "Number of channels: " << format.nChannels << std::endl;
		std::cout << "Number of bytes pr. sample: " << bytes_per_sample << std::endl;
		std::cout << "---------------------------------" << std::endl;

		throw CL_Error("Invalid wave file format");
	}

	sample_left = sample_size;
}

CL_Streamed_WaveSample_Session::~CL_Streamed_WaveSample_Session()
{
	delete input;
}

bool CL_Streamed_WaveSample_Session::eof() const
{
	if (sample_left <= 0 && !looped) return true;
	return false;
}

int CL_Streamed_WaveSample_Session::get_data(void *data_ptr, int data_requested)
{
	if (sample_left <= 0) 
	{
		if (looped)
		{
			sample_left = sample_size;
			input->seek(0, CL_InputSource::seek_set);
		}
		else
		{
			return 0;
		}
	}

	if (looped)
	{
		if (data_requested > sample_left)
		{
			int pos = 0;
			while (data_requested > 0)
			{
				input->read(&((unsigned char *) data_ptr)[pos], sample_left);
				if (get_format() == sf_8bit_signed || get_format() == sf_8bit_signed_stereo)
				{
					for (int i=0;i<sample_left;i++)
					{
						((unsigned char *) data_ptr)[pos+i] = char(short(((unsigned char *) data_ptr)[pos+i])+128);
					}
				}
				data_requested -= sample_left;
				pos += sample_left;

				input->seek(0, CL_InputSource::seek_set);

				int bytes_read = input->read(&((unsigned char *)data_ptr)[pos], data_requested);
				data_requested -= bytes_read;
				if (get_format() == sf_8bit_signed || get_format() == sf_8bit_signed_stereo)
				{
					for (int i=0;i<bytes_read;i++)
					{
						((unsigned char *) data_ptr)[pos+i] = char(short(((unsigned char *) data_ptr)[pos+i])+128);
					}
				}
				pos += bytes_read;

				sample_left = sample_size - bytes_read;
			}

			return data_requested;
		}
		else
		{
			sample_left -= data_requested;
			int read = input->read(data_ptr, data_requested);
			if (get_format() == sf_8bit_signed || get_format() == sf_8bit_signed_stereo)
			{
				for (int i=0;i<read;i++)
				{
					((unsigned char *) data_ptr)[i] = char(short(((unsigned char *) data_ptr)[i])+128);
				}
			}
			return read;
		}
	}
	else
	{
		sample_left -= data_requested;
		if (sample_left < 0) 
		{
			data_requested += sample_left;
		}
		int read = input->read(data_ptr, data_requested);
		if (get_format() == sf_8bit_signed || get_format() == sf_8bit_signed_stereo)
		{
			for (int i=0;i<read;i++)
			{
				((unsigned char *) data_ptr)[i] = char(short(((unsigned char *) data_ptr)[i])+128);
			}
		}

		return read;
	}
}

int CL_Streamed_WaveSample_Session::get_frequency() const
{
	return sample_freq;
}

SoundFormat CL_Streamed_WaveSample_Session::get_format() const
{
	return sample_format;
}
