/*
	$Id: outputsource_memory_generic.cpp,v 1.6 2001/04/29 17:04:28 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include <API/Core/IOData/outputsource.h>
#include "outputsource_memory_generic.h"

CL_OutputSource_MemoryGeneric::CL_OutputSource_MemoryGeneric(unsigned int blocksize)
{
	m_blocksize = blocksize;
	m_size = m_blocksize;
	m_data = new unsigned char[m_blocksize];
	m_pos = 0;
}

CL_OutputSource_MemoryGeneric::~CL_OutputSource_MemoryGeneric()
{
	delete[] m_data;
}

void *CL_OutputSource_MemoryGeneric::get_data() const
{
	return (void *) m_data;
}

int CL_OutputSource_MemoryGeneric::write(const void *data, int size)
{
	if (m_pos + size > m_size) // extend array with blocksize
	{
		int new_size = m_size + size + m_blocksize;

		unsigned char *old = m_data;
		m_data = new unsigned char[new_size];

		memcpy(m_data, old, m_pos);
		delete[] old;
		m_size = new_size;
	}
	
	memcpy(m_data + m_pos, data, size);
	m_pos += size;
	
	return size;
}

void CL_OutputSource_MemoryGeneric::open()
{
}

void CL_OutputSource_MemoryGeneric::close()
{
}

CL_OutputSource *CL_OutputSource_MemoryGeneric::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_MemoryGeneric::tell() const
{
	return m_pos;
}

int CL_OutputSource_MemoryGeneric::size() const
{
	return m_pos;
}

void CL_OutputSource_MemoryGeneric::purge()
{
	memcpy(m_data,0,m_size);
}
