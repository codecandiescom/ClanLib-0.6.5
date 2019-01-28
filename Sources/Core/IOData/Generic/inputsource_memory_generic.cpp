/*
	$Id: inputsource_memory_generic.cpp,v 1.11 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include "inputsource_memory_generic.h"
#include "outputsource_memory_generic.h"

CL_InputSource_MemoryGeneric::CL_InputSource_MemoryGeneric(
	void *_data,
	int size,
	bool delete_data)
{
	data = (unsigned char *) _data;
	pos = 0;
	length = size;
	delete_data = delete_data;
}

CL_InputSource_MemoryGeneric::CL_InputSource_MemoryGeneric(
	const CL_InputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	pos = 0;
	length = MG->size();
	delete_data = true;
	memcpy(data, MG->data, length);
}

CL_InputSource_MemoryGeneric::~CL_InputSource_MemoryGeneric()
{
	if (delete_data) delete[] data;
}

int CL_InputSource_MemoryGeneric::read(void *_data, int size)
{
	cl_assert(pos + size <= length);
	
	memcpy(_data, data + pos, size);
	pos += size;

	return size;
}

void CL_InputSource_MemoryGeneric::open()
{
	pos = 0;
}

void CL_InputSource_MemoryGeneric::close()
{
}

CL_InputSource *CL_InputSource_MemoryGeneric::clone() const
{
	CL_InputSource_MemoryGeneric *ret = new CL_InputSource_MemoryGeneric(data, length, delete_data);
	ret->seek(pos, seek_set);
	return ret;
}

int CL_InputSource_MemoryGeneric::tell() const
{
	return pos;
}

void CL_InputSource_MemoryGeneric::seek(int _pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		pos = _pos;
		break;
	
	case seek_cur:
		pos += _pos;
		break;
	
	case seek_end:
		pos = length + _pos;
		break;
	
	default: // invalid seek type!
		cl_assert(false);
	}
}

int CL_InputSource_MemoryGeneric::size() const
{
	return length;
}

void CL_InputSource_MemoryGeneric::push_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_MemoryGeneric::pop_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_MemoryGeneric::purge()
{
	memcpy(data, 0, length);
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(unsigned char *_data, unsigned int _size, bool _delete_data)
{
	data = _data;
	size = _size;
	delete_data = _delete_data;
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(CL_InputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	size = MG->size();
	delete_data = true;
	MG->read(data, size);
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(CL_OutputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	size = MG->size();
	delete_data = true;
	memcpy(data, MG->get_data(), size);
}

CL_InputSource *CL_InputSourceProvider_Memory::open_source(const std::string &)
{
	return new CL_InputSource_MemoryGeneric(data, size, delete_data);
}

CL_InputSourceProvider *CL_InputSourceProvider_Memory::clone()
{
 	cl_assert(false);
 	return NULL;
}

