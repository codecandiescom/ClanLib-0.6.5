/*
	$Id: inputsource_memory.cpp,v 1.6 2001/04/29 17:04:27 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/System/cl_assert.h>
#include <API/Core/System/clanstring.h>
#include <API/Core/IOData/inputsource_memory.h>

CL_InputSource_Memory::CL_InputSource_Memory(std::string _data)
{
	data = _data;
	pos = 0;
}

CL_InputSource_Memory::~CL_InputSource_Memory()
{
}

int CL_InputSource_Memory::read(void *dest_data, int size)
{
	cl_assert(pos + size <= data.size());
	
	memcpy(dest_data, data.data() + pos, size);
	pos += size;

	return size;
}

void CL_InputSource_Memory::open()
{
	pos = 0;
}

void CL_InputSource_Memory::close()
{
}

CL_InputSource *CL_InputSource_Memory::clone() const
{
	return new CL_InputSource_Memory(data);
}

int CL_InputSource_Memory::tell() const
{
	return pos;
}

void CL_InputSource_Memory::seek(int new_pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		pos = new_pos;
		break;
	
	case seek_cur:
		pos += new_pos;
		break;
	
	case seek_end:
		pos = data.size() + new_pos;
		break;
	
	default: // invalid seek type!
		cl_assert(false);
	}
}

int CL_InputSource_Memory::size() const
{
	return data.size();
}

void CL_InputSource_Memory::push_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_Memory::pop_position()
{
	cl_assert(false); // not implemented yet.
}

std::string CL_InputSource_Memory::get_data() const
{
	return data;
}

