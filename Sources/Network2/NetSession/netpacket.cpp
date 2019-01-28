
#include "API/Network2/NetSession/netpacket.h"
#include "API/Network2/NetSession/netcomputer.h"
#include "netpacket_generic.h"
#include "inputsource_netpacket_generic.h"
#include "outputsource_netpacket_generic.h"
#include "netcomputer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket construction:

CL_NetPacket::CL_NetPacket()
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();
}

CL_NetPacket::CL_NetPacket(const void *data, int size)
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data, size);
}

CL_NetPacket::CL_NetPacket(const std::string &data)
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data.data(), data.length());
}

CL_NetPacket::CL_NetPacket(const CL_NetPacket &copy)
: impl(copy.impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;

	if (impl) impl->add_ref();
}

CL_NetPacket::~CL_NetPacket()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket attributes:

unsigned char *CL_NetPacket::get_data()
{
	return impl->data;
}

const unsigned char *CL_NetPacket::get_data() const
{
	return impl->data;
}

int CL_NetPacket::get_size() const
{
	return impl->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket operations:

void CL_NetPacket::resize(int new_size)
{
	impl->resize(new_size);
}

CL_NetPacket &CL_NetPacket::operator =(const CL_NetPacket &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();

	input.impl->netpacket = impl;
	output.impl->netpacket = impl;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket implementation:
