/*
	$Id: static_provider_wave.cpp,v 1.6 2002/08/14 13:58:29 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Simple sample support.

*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Core/IOData/cl_endian.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"
#include "API/Sound/SoundProviders/static_provider_raw.h"
#include "API/Sound/SoundProviders/static_provider_wave.h"

CL_SoundBuffer *CL_Sample::create(const std::string &sample_id, CL_InputSourceProvider *provider)
{
	return CL_SoundBuffer::create(new CL_Sample(sample_id, provider), true);
}

CL_Sample::CL_Sample(const std::string &s_id, CL_InputSourceProvider *_provider) : sample_id(s_id)
{
	if (_provider == NULL)
	{
		provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		provider = _provider->clone();
	}
	sample_data = NULL;
}

CL_Sample::~CL_Sample()
{
	delete[] sample_data;
	delete provider;
}

void CL_Sample::load_data()
{
	CL_InputSource *source = provider->open_source(sample_id.c_str());
	cl_assert(source != NULL);

	// Check to see if this is really a .wav-file
	char temp[12];
	source->read(temp, 4);
	source->seek(4, CL_InputSource::seek_cur);
	source->read(&temp[4], 8);
	source->seek(4, CL_InputSource::seek_cur);

	if (memcmp(temp, "RIFFWAVEfmt ", 12) != 0)
	{
		throw CL_Error("Invalid RIFF WAVE header!");
	}
//	cl_assert(!(memcmp(temp, "RIFFWAVEfmt ", 12)));

	WAVE_FORMAT format;
	format.formatTag = source->read_short16();
	format.nChannels = source->read_ushort16();
	format.nSamplesPerSec = source->read_uint32();
	format.nAvgBytesPerSec = source->read_uint32();
	format.nBlockAlign = source->read_ushort16();

	/* FIXME: find a better way to handle format struct padding */
	source->read(temp, 2);

	// Another sanity check
	source->read(temp, 4);
	temp[4] = 0;
	if (memcmp(temp, "data", 4) != 0)
	{
		throw CL_Error("Invalid RIFF data chunk!");
	}
//	cl_assert(!(memcmp(temp, "data", 4)));

	sample_size = source->read_uint32();
	sample_freq = format.nSamplesPerSec;

	int bytes_per_sample = format.nAvgBytesPerSec / format.nSamplesPerSec;

	if (format.nChannels == 2 && bytes_per_sample == 4) sample_format = sf_16bit_signed_stereo;
	else if (format.nChannels == 2 && bytes_per_sample == 2) sample_format = sf_8bit_signed_stereo;
	else if (format.nChannels == 1 && bytes_per_sample == 2) sample_format = sf_16bit_signed;
	else if (format.nChannels == 1 && bytes_per_sample == 1) sample_format = sf_8bit_signed;
	else
	{
		std::cout <<"    Invalid wave file format         " <<std::endl;
		std::cout <<"---------------------------------" <<std::endl;
		std::cout <<"Sample size: " << sample_size <<std::endl;
		std::cout <<"Sample frequency: " << sample_freq <<std::endl;
		std::cout <<"Number of channels: " << format.nChannels <<std::endl;
		std::cout <<"Number of bytes pr. sample: " << bytes_per_sample <<std::endl;
		std::cout <<"---------------------------------" <<std::endl;

		throw CL_Error("Invalid wave file format");
	}

	sample_data = new signed char[sample_size];
	if (sample_format == sf_8bit_signed || sample_format == sf_8bit_signed_stereo)
	{
		for (int i=0;i<sample_size;i++)
			sample_data[i] = source->read_uchar8()-128;
	}
	else	// 16 bit
	{
		for (int i=0;i<sample_size;i+=2)
			*(short *)&sample_data[i] = source->read_short16();
	}

	delete source;
}

void CL_Sample::lock()
{
	if (sample_data != NULL) return;

	load_data();
}

void CL_Sample::unlock()
{
	delete[] sample_data;
	sample_data = NULL;
}

SoundFormat CL_Sample::get_format() const
{
	return sample_format;
}

int CL_Sample::data_size() const
{
	return sample_size;
}

void *CL_Sample::get_data() const
{
	return sample_data;
}

int CL_Sample::get_frequency() const
{
	return sample_freq;
}
