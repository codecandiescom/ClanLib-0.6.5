
#include "API/Network2/NetSession/outputsource_netpacket.h"
#include "API/Network2/NetSession/netpacket.h"
#include "outputsource_netpacket_generic.h"
#include "netpacket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetPacket construction:

CL_OutputSource_NetPacket::CL_OutputSource_NetPacket(const CL_NetPacket &netpacket)
: impl(new CL_OutputSource_NetPacket_Generic(netpacket.impl, true))
{
}

CL_OutputSource_NetPacket::~CL_OutputSource_NetPacket()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetPacket attributes:

int CL_OutputSource_NetPacket::tell() const
{
	return impl->position;
}

int CL_OutputSource_NetPacket::size() const
{
	return impl->netpacket->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetPacket operations:

int CL_OutputSource_NetPacket::write(const void *data, int size)
{
	impl->netpacket->resize(impl->position+size);
	memcpy(impl->netpacket->data+impl->position, data, size);
	impl->position += size;
	return size;
}

void CL_OutputSource_NetPacket::open()
{
}

void CL_OutputSource_NetPacket::close()
{
}

CL_OutputSource *CL_OutputSource_NetPacket::clone()
{
	return new CL_OutputSource_NetPacket(*this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetPacket implementation:

CL_OutputSource_NetPacket::CL_OutputSource_NetPacket()
: impl(new CL_OutputSource_NetPacket_Generic(0, false))
{
}
