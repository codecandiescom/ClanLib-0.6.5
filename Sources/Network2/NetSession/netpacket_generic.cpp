
#include "netpacket_generic.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic construction:

CL_NetPacket_Generic::CL_NetPacket_Generic()
: data(0), size(0), allocated_size(0), ref_count(0)
{
}

CL_NetPacket_Generic::~CL_NetPacket_Generic()
{
	delete[] data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic operations:

int CL_NetPacket_Generic::add_ref()
{
	return ++ref_count;
}

int CL_NetPacket_Generic::release_ref()
{
	ref_count--;
	if (ref_count > 0) return ref_count;

	delete this;
	return 0;
}

#define cl_min(a, b) ((a < b) ? a : b)

void CL_NetPacket_Generic::resize(int new_size)
{
	if (new_size <= allocated_size)
	{
		size = new_size;
		return;
	}

	int new_allocated_size = new_size + 8*1024;
	unsigned char *old_data = data;
	data = new unsigned char[new_allocated_size];
	memcpy(data, old_data, cl_min(size, new_size));
	delete[] old_data;
	data = data;
	allocated_size = new_allocated_size;
	size = new_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic implementation:
