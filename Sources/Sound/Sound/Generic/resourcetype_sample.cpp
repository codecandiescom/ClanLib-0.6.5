/*
	$Id: resourcetype_sample.cpp,v 1.8 2002/01/16 01:45:43 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sample resource type

*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "resourcetype_sample.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resourceoptions.h"
#include <API/Core/IOData/inputsource.h>
#include <API/Core/IOData/inputsource_provider.h>
#include <API/Core/IOData/outputsource.h>
#include "API/Core/IOData/outputsource_provider.h"
#include <API/Sound/soundbuffer.h>
#include <API/Sound/static_soundprovider.h>
#include <API/Sound/stream_soundprovider.h>
#include <API/Sound/SoundProviders/static_provider_wave.h>
#include <API/Sound/SoundProviders/stream_provider_wave.h>

/////////////////////////////////////////////////////////////////////////////
//!Construction:

CL_ResourceData_Sample::CL_ResourceData_Sample(class CL_Resource &resource)
: CL_ResourceData(resource)
{
	slot_load_file = resource.sig_load_file().connect(this, &CL_ResourceData_Sample::on_load_file);
	slot_load_datafile = resource.sig_load_datafile().connect(this, &CL_ResourceData_Sample::on_load_datafile);
	slot_save_datafile = resource.sig_save_datafile().connect(this, &CL_ResourceData_Sample::on_save_datafile);
	slot_unload = resource.sig_unload().connect(this, &CL_ResourceData_Sample::on_unload);

	resource.attach_data("sample", this);
}

CL_ResourceData_Sample::~CL_ResourceData_Sample()
{
}

/////////////////////////////////////////////////////////////////////////////
//!Attributes:

/////////////////////////////////////////////////////////////////////////////
//!Implementation:

void CL_ResourceData_Sample::on_load_file()
{
	CL_Resource resource = get_resource();
	CL_ResourceOptions &options = resource.get_options();

	if (options.exists("stream"))
	{
		soundbuffer = CL_SoundBuffer::create(
			new CL_Streamed_WaveSample(
				resource.get_location().c_str(),
				resource.get_manager().get_resource_provider(),
				options.exists("loop")),
			true);
	}
	else
	{
		soundbuffer = CL_SoundBuffer::create(
			new CL_Sample(
				resource.get_location().c_str(),
				resource.get_manager().get_resource_provider()),
			true);
	}
}

void CL_ResourceData_Sample::on_load_datafile(class CL_InputSourceProvider *input_provider)
{
	CL_InputSource *input = input_provider->open_source(get_resource().get_name().c_str());

	CL_Resource resource = get_resource();
//	CL_ResourceOptions &options = resource.get_options();

	int streamed = false; // todo: somehow write this into the stream.

	if (streamed)
	{
		soundbuffer = new CL_SoundBuffer(
			new CL_Streamed_WaveSample(
				resource.get_name().c_str(),
				resource.get_manager().get_resource_provider(),
				false /*options->exists("loop")*/ ),
			true);
	}
	else
	{
		soundbuffer = new CL_SoundBuffer(
			new CL_Sample(
				resource.get_name().c_str(),
				resource.get_manager().get_resource_provider()),
			true);
	}

	delete input;
}

void CL_ResourceData_Sample::on_save_datafile(class CL_OutputSourceProvider *output_provider)
{
	CL_OutputSource *output = output_provider->open_source(get_resource().get_name().c_str());

	CL_InputSourceProvider *provider = get_resource().get_manager().get_resource_provider();
	cl_assert(provider != NULL);

	CL_InputSource *wavefile = provider->open_source(get_resource().get_location().c_str());
	cl_assert(wavefile != NULL);

	int size_left = wavefile->size();
	while (size_left > 0)
	{
		unsigned char buffer[32768];
		int read = wavefile->read(buffer, 32768);
		size_left -= read;
		output->write(buffer, read);
	}

	delete wavefile;
	delete output;
}

void CL_ResourceData_Sample::on_unload()
{
	delete soundbuffer;
	soundbuffer = NULL;
}



















#ifdef OBSOLETE

#include <API/Core/IOData/inputsource.h>
#include <API/Core/IOData/inputsource_provider.h>
#include <API/Core/IOData/outputsource.h>
#include <API/Core/Resources/resource.h>
#include <API/Core/Resources/resource_manager.h>
#include <API/Core/Resources/resourceoptions.h>
#include <API/Core/Resources/resourcetype.h>
#include <API/Core/System/clanstring.h>
#include <API/Core/System/cl_assert.h>
#include <API/Sound/soundbuffer.h>
#include <API/Sound/static_soundprovider.h>
#include <API/Sound/stream_soundprovider.h>
#include <API/Sound/SoundProviders/static_provider_wave.h>
#include <API/Sound/SoundProviders/stream_provider_wave.h>
#include <Sound/Sound/Generic/resourcetype_sample.h>

/******************************************************************************
						class CL_Res_Sample
******************************************************************************/

CL_Res_Sample::CL_Res_Sample() : CL_ResourceType("sample")
{
}

CL_Resource *CL_Res_Sample::create_from_location(
	std::string name,
	std::string location,
	CL_ResourceOptions *options,
	CL_ResourceManager *parent)
{
	CL_String ext = CL_String(location).right(4);
	ext.to_lower();

	if ((options->exists("type") && options->get_value("type") == "sample") ||
		(options->exists("wav") || ext == ".wav"))
	{
		return new CL_SampleFileResource(
			name,
			location,
			options,
			parent);
	}

	return NULL;
}

CL_Resource *CL_Res_Sample::create_from_serialization(
	std::string name,
	CL_ResourceManager *parent)
{
	return new CL_SampleDatafileResource(
		name,
		parent);
}

/******************************************************************************
						class CL_WritableSampleResource
******************************************************************************/

CL_WritableSampleResource::CL_WritableSampleResource(std::string name)
: CL_Resource("sample", name)
{ 
	loaded = false;
	loaded_sample = NULL;
	load_count = 0;
}

void CL_WritableSampleResource::serialize_save(CL_OutputSource *output)
{
	CL_InputSourceProvider *provider = CL_InputSourceProvider::create_file_provider(".");
	cl_assert(provider != NULL);

	CL_InputSource *wavefile = provider->open_source(sample_location);
	cl_assert(wavefile != NULL);

	int size_left = wavefile->size();
	while (size_left > 0)
	{
		unsigned char buffer[32768];
		int read = wavefile->read(buffer, 32768);
		size_left -= read;
		output->write(buffer, read);
	}

	delete wavefile;
	delete provider;
}

void CL_WritableSampleResource::load()
{
	load_count++;
	if (loaded) return;

	create_sample();
}

void CL_WritableSampleResource::unload()
{
	load_count--;
	if (load_count == 0)
	{
		delete loaded_sample;
		loaded = false;
	}
}

/******************************************************************************
						class CL_SampleFileResource
******************************************************************************/

CL_SampleFileResource::CL_SampleFileResource(
	std::string name,
	std::string location, 
	CL_ResourceOptions *_options, 
	CL_ResourceManager *_parent)
: CL_WritableSampleResource(name)
{
	sample_location = location;
	options = _options;
	parent = _parent;
}

CL_SampleFileResource::~CL_SampleFileResource()
{
	delete options;
}

CL_SoundBuffer *CL_SampleFileResource::create_sample()
{
	if (loaded) return loaded_sample;

	if (options->exists("stream"))
	{
		loaded_sample = CL_SoundBuffer::create(
			new CL_Streamed_WaveSample(
				sample_location,
				parent->get_resource_provider(),
				options->exists("loop")),
			true);
	}
	else
	{
		loaded_sample = CL_SoundBuffer::create(
			new CL_Sample(
				sample_location,
				parent->get_resource_provider()),
			true);
	}

	loaded = true;
	return loaded_sample;
}


/******************************************************************************
						class CL_SampleDatafileResource
******************************************************************************/

CL_SampleDatafileResource::CL_SampleDatafileResource(
	std::string name,
	CL_ResourceManager *_parent)
: CL_WritableSampleResource(name)
{
	sample_location = "";
	options = NULL;
	parent = _parent;
}

CL_SampleDatafileResource::~CL_SampleDatafileResource()
{
	delete options;
}

CL_SoundBuffer *CL_SampleDatafileResource::create_sample()
{
	if (loaded) return loaded_sample;

	int streamed = false; // todo: somehow write this into the stream.

	if (streamed)
	{
		loaded_sample = CL_SoundBuffer::create(
			new CL_Streamed_WaveSample(
				get_name().c_str(),
				parent->get_resource_provider(),
				false /*options->exists("loop")*/ ),
			true);
	}
	else
	{
		loaded_sample = CL_SoundBuffer::create(
			new CL_Sample(
				get_name().c_str(),
				parent->get_resource_provider()),
			true);
	}
	
	loaded = true;
	return loaded_sample;
}
#endif
