
#include "API/Network2/NetSession/netgroup.h"
#include "API/Network2/NetSession/netcomputer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup construction:

CL_NetGroup::CL_NetGroup()
{
}

CL_NetGroup::CL_NetGroup(const CL_NetComputer &computer)
{
	push_back(computer);
}

CL_NetGroup::CL_NetGroup(const std::list<CL_NetComputer> &group) : std::list<CL_NetComputer>(group)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup operations:

void CL_NetGroup::send(const std::string &packet_channel, const CL_NetPacket &packet)
{
	for (iterator it = begin(); it != end(); it++)
	{
		(*it).send(packet_channel, packet);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup implementation:
