/*
	$Id: soundbuffer.cpp,v 1.9 2002/01/15 16:00:39 mbn Exp $

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

#include "API/Sound/soundbuffer.h"
#include "API/Core/System/error.h"
#include "resourcetype_sample.h"
#include "soundbuffer_generic.h"
#include "soundbuffer_static.h"
#include "soundbuffer_stream.h"

CL_SoundBuffer *CL_SoundBuffer::load(
	const std::string &res_id,
	CL_ResourceManager *manager)
{
	return new CL_SoundBuffer(res_id, manager);
}

CL_SoundBuffer *CL_SoundBuffer::create(
	CL_StaticSoundProvider *provider,
	bool delete_provider)
{
	return new CL_SoundBuffer(provider, delete_provider);
}

CL_SoundBuffer *CL_SoundBuffer::create(
	CL_StreamSoundProvider *provider,
	bool delete_provider)
{
	return new CL_SoundBuffer(provider, delete_provider);
}

CL_SoundBuffer::CL_SoundBuffer()
: impl(NULL)
{
}
	
void CL_SoundBuffer::operator =(const CL_SoundBuffer &copy)
{
	resource.unload();
	if (impl) impl->release_reference();

	resource = copy.resource;
	impl = copy.impl;
	resource.load();
	if (impl) impl->add_reference();
}

CL_SoundBuffer::CL_SoundBuffer(
	const std::string &res_id,
	CL_ResourceManager *manager)
:
	impl(NULL)
{
	resource = manager->get_resource(std::string(res_id));
	if (resource.get_type() != "sample") throw CL_Error("Resource is not a sample!");

	resource.load();

	impl = ((CL_ResourceData_Sample *) resource.get_data("sample"))->soundbuffer->impl;
	impl->add_reference();
}

CL_SoundBuffer::CL_SoundBuffer(
	CL_StaticSoundProvider *provider,
	bool delete_provider)
:
	impl(new CL_SoundBuffer_Generic_Static(provider, delete_provider))
{
	impl->add_reference();
}

CL_SoundBuffer::CL_SoundBuffer(
	CL_StreamSoundProvider *provider,
	bool delete_provider)
:
	impl(new CL_SoundBuffer_Generic_Stream(provider, delete_provider))
{
	impl->add_reference();
}

CL_SoundBuffer::CL_SoundBuffer(const CL_SoundBuffer &copy)
: impl(copy.impl)
{
	if (impl) impl->add_reference();
}

CL_SoundBuffer::~CL_SoundBuffer()
{
	if (impl != NULL) impl->release_reference();
	resource.unload();
}

CL_StaticSoundProvider *CL_SoundBuffer::get_static_provider() const
{
	return impl->get_static_provider();
}

CL_StreamSoundProvider *CL_SoundBuffer::get_stream_provider() const
{
	return impl->get_stream_provider();
}

int CL_SoundBuffer::get_length() const
{
	return impl->get_length();
}

int CL_SoundBuffer::get_num_samples() const
{
	return impl->get_num_samples();
}

int CL_SoundBuffer::get_frequency() const
{
	return impl->get_frequency();
}

bool CL_SoundBuffer::set_frequency(int new_freq)
{
	return impl->set_frequency(new_freq);
}

float CL_SoundBuffer::get_volume() const
{
	return impl->get_volume();
}

bool CL_SoundBuffer::set_volume(float new_volume)
{
	return impl->set_volume(new_volume);
}

float CL_SoundBuffer::get_pan() const
{
	return impl->get_pan();
}

bool CL_SoundBuffer::set_pan(float new_pan)
{
	return impl->set_pan(new_pan);
}

bool CL_SoundBuffer::is_playing(
	CL_SoundBuffer_Session **session,
	CL_SoundCard *card) const
{
	return impl->is_playing(session, card);
}

void CL_SoundBuffer::stop(CL_SoundCard *card)
{
	impl->stop(card);
}

CL_SoundBuffer_Session CL_SoundBuffer::play(
	bool looping,
	CL_SoundCard *card)
{
	return impl->play(looping, card);
}

CL_SoundBuffer_Session CL_SoundBuffer::prepare(
	bool looping,
	CL_SoundCard *card)
{
	return impl->prepare(looping, card);
}
