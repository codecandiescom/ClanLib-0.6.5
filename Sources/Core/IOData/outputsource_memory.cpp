/*
	$Id: outputsource_memory.cpp,v 1.4 2001/04/29 17:04:27 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/System/cl_assert.h>
#include <API/Core/IOData/outputsource_memory.h>

CL_OutputSource_Memory::CL_OutputSource_Memory()
{
	pos = 0;
}

CL_OutputSource_Memory::~CL_OutputSource_Memory()
{
}

std::string CL_OutputSource_Memory::get_data() const
{
	return data;
}

int CL_OutputSource_Memory::write(const void *source_data, int size)
{
	data.append((const char *) source_data, size);
	pos += size;
	
	return size;
}

void CL_OutputSource_Memory::open()
{
}

void CL_OutputSource_Memory::close()
{
}

CL_OutputSource *CL_OutputSource_Memory::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_Memory::tell() const 
{
	return pos;
}

int CL_OutputSource_Memory::size() const 
{
	return pos;
}
