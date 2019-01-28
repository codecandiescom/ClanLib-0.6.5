
#include "outputsource_netpacket_generic.h"
#include "netpacket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_NetPacket_Generic construction:

CL_OutputSource_NetPacket_Generic::CL_OutputSource_NetPacket_Generic(CL_NetPacket_Generic *netpacket, bool ref_count)
: netpacket(netpacket), position(0), ref_count(ref_count)
{
	if (ref_count) netpacket->add_ref();
}

CL_OutputSource_NetPacket_Generic::~CL_OutputSource_NetPacket_Generic()
{
	if (ref_count) netpacket->release_ref();
}
