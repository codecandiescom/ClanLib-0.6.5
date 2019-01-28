
#include "API/Core/System/error.h"
#include "API/Network2/NetSession/netcomputer.h"
#include "API/Network2/NetSession/netsession.h"
#include "netcomputer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer construction:

CL_NetComputer::CL_NetComputer()
: impl(0)
{
}

CL_NetComputer::CL_NetComputer(const CL_NetComputer &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetComputer::~CL_NetComputer()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer attributes:

CL_IPAddress CL_NetComputer::get_address() const
{
	if (impl) return impl->address;
	return CL_IPAddress(0);
}

CL_NetSession CL_NetComputer::get_session()
{
	if (impl == 0)
		throw CL_Error("CL_NetComputer::get_session() not allowed on an unattached netcomputer object");

	return CL_NetSession(impl->netsession);
}

bool CL_NetComputer::is_disconnected() const
{
	if(impl == 0)
		return true;
	else
		return impl->disconnected;
}

const std::string &CL_NetComputer::get_disconnect_reason() const
{
	static std::string null_msg("Unattached netcomputer object");

	if (impl == 0)
		return null_msg;
	else
		return impl->disconnect_reason;
}

bool CL_NetComputer::operator == (const CL_NetComputer &other_instance) const
{
	return (impl == other_instance.impl);
}

bool CL_NetComputer::operator < (const CL_NetComputer &other_instance) const
{
	return (impl < other_instance.impl);
}

bool CL_NetComputer::operator > (const CL_NetComputer &other_instance) const
{
	return (impl > other_instance.impl);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer operations:

void CL_NetComputer::disconnect()
{
}

void CL_NetComputer::send(const std::string &packet_channel, const CL_NetPacket &packet)
{
	if (impl == NULL)
		throw CL_Error("CL_NetComputer::send not allowed on an unattached netcomputer object");

	impl->send_packet(packet_channel, packet.impl);
}

CL_NetComputer &CL_NetComputer::operator = (const CL_NetComputer &other_instance)
{
	if (impl) impl->release_ref();
	impl = other_instance.impl;
	if (impl) impl->add_ref();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer implementation:

CL_NetComputer::CL_NetComputer(class CL_NetComputer_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}
