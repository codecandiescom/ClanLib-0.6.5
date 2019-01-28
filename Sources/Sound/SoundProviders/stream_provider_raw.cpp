/*
	$Id: stream_provider_raw.cpp,v 1.3 2001/12/11 20:44:24 mbn Exp $

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
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Sound/SoundProviders/stream_provider_raw.h"

/*****************************
     CL_Streamed_RawSample
*****************************/

CL_SoundBuffer *CL_Streamed_RawSample::create(
	const std::string &filename,
	SoundFormat format,
	int frequency,
	CL_InputSourceProvider *inputprovider, 
	bool looped)
{
	return CL_SoundBuffer::create(
		new CL_Streamed_RawSample(
			filename,
			format,
			frequency,
			inputprovider,
			looped),
		true);
}

CL_Streamed_RawSample::CL_Streamed_RawSample(
	const std::string &_filename,
	SoundFormat _format,
	int _frequency,
	CL_InputSourceProvider *_inputprovider,
	bool _looped)
{
	filename = _filename;
	looped = _looped;
	format = _format;
	frequency = _frequency;

	if (_inputprovider == NULL)
	{
		inputprovider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		inputprovider = _inputprovider->clone();
	}
}

CL_Streamed_RawSample::~CL_Streamed_RawSample()
{
	delete inputprovider;
}

CL_StreamSoundProvider_Session *CL_Streamed_RawSample::begin_session()
{
	return new CL_Streamed_RawSample_Session(
		inputprovider->open_source(filename.c_str()),
		format,
		frequency,
		looped);
}

void CL_Streamed_RawSample::end_session(CL_StreamSoundProvider_Session *session)
{
	delete session;
}

/********************************
  CL_Streamed_RawSample_Session
********************************/

CL_Streamed_RawSample_Session::CL_Streamed_RawSample_Session(
	CL_InputSource *_input,
	SoundFormat _format,
	int _frequency,
	bool _looped)
{
	input = _input;
	looped = _looped;
	cl_assert(input != NULL);

	sample_size = input->size();
	sample_freq = _frequency;
	sample_format = _format;
	sample_left = sample_size;
}

CL_Streamed_RawSample_Session::~CL_Streamed_RawSample_Session()
{
	delete input;
}

bool CL_Streamed_RawSample_Session::eof() const
{
	if (sample_left <= 0 && !looped) return true;
	return false;
}

int CL_Streamed_RawSample_Session::get_data(void *data_ptr, int data_requested)
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
				if (get_format() == sf_8bit_signed)
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
				if (get_format() == sf_8bit_signed)
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
			if (get_format() == sf_8bit_signed)
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
		if (get_format() == sf_8bit_signed)
		{
			for (int i=0;i<read;i++)
			{
				((unsigned char *) data_ptr)[i] = char(short(((unsigned char *) data_ptr)[i])+128);
			}
		}

		return read;
	}
}

int CL_Streamed_RawSample_Session::get_frequency() const
{
	return sample_freq;
}

SoundFormat CL_Streamed_RawSample_Session::get_format() const
{
	return sample_format;
}
