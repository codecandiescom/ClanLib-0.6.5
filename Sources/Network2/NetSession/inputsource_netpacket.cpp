
#include "API/Core/System/cl_assert.h"
#include "API/Network2/NetSession/inputsource_netpacket.h"
#include "API/Network2/NetSession/netpacket.h"
#include "inputsource_netpacket_generic.h"
#include "netpacket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket construction:

CL_InputSource_NetPacket::CL_InputSource_NetPacket(const CL_NetPacket &netpacket)
: impl(new CL_InputSource_NetPacket_Generic(netpacket.impl, true))
{
}

CL_InputSource_NetPacket::~CL_InputSource_NetPacket()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket attributes:

int CL_InputSource_NetPacket::tell() const
{
	return impl->position;
}

int CL_InputSource_NetPacket::size() const
{
	return impl->netpacket->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket operations:

#define cl_min(a, b) ((a < b) ? a : b)

int CL_InputSource_NetPacket::read(void *data, int size)
{
	int received = cl_min(size, impl->netpacket->size - impl->position);
	memcpy(data, impl->netpacket->data+impl->position, received);
	impl->position += received;
	return received;
}

void CL_InputSource_NetPacket::open()
{
}

void CL_InputSource_NetPacket::close()
{
}

CL_InputSource *CL_InputSource_NetPacket::clone() const
{
	return new CL_InputSource_NetPacket(*this);
}

void CL_InputSource_NetPacket::seek(int pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		impl->position = pos;
		break;

	case seek_cur:
		impl->position += pos;
		break;

	case seek_end:
		impl->position = size()+pos;
		break;
	}
}

void CL_InputSource_NetPacket::push_position()
{
	cl_assert(false); // not implemented.
}

void CL_InputSource_NetPacket::pop_position()
{
	cl_assert(false); // not implemented.
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket implementation:

CL_InputSource_NetPacket::CL_InputSource_NetPacket()
: impl(new CL_InputSource_NetPacket_Generic(0, false))
{
}
