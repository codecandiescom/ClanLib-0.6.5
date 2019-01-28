
#include "inputsource_netpacket_generic.h"
#include "netpacket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket_Generic construction:

CL_InputSource_NetPacket_Generic::CL_InputSource_NetPacket_Generic(CL_NetPacket_Generic *netpacket, bool ref_count)
: netpacket(netpacket), position(0), ref_count(ref_count)
{
	if (ref_count) netpacket->add_ref();
}

CL_InputSource_NetPacket_Generic::~CL_InputSource_NetPacket_Generic()
{
	if (ref_count) netpacket->release_ref();
}
