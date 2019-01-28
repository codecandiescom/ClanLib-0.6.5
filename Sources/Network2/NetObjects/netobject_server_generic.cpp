
#include "netobject_server_generic.h"
#include "netobject_controller_generic.h"
#include "API/Network2/NetSession/netpacket.h"
#include "API/Network2/NetSession/netgroup.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic construction:

CL_NetObject_Server_Generic::CL_NetObject_Server_Generic(
	CL_NetObject_Controller_Generic *controller)
: obj_id(controller->obj_id_counter++), controller(controller), ref_count(0)
{
	controller->server_objects[obj_id] = this;
}

CL_NetObject_Server_Generic::~CL_NetObject_Server_Generic()
{
	controller->server_objects.erase(controller->server_objects.find(obj_id));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic operations:

void CL_NetObject_Server_Generic::receive(int msg_type, const CL_NetComputer &from, CL_NetPacket &packet)
{
	std::map< int, CL_Signal_v2<const CL_NetComputer &, CL_NetPacket &> >::iterator it;
	it = sig_received_message.find(msg_type);
	if (it != sig_received_message.end())
	{
		it->second(from, packet);
	}
	else
	{
		// No message handler found.
		// Inform application somehow?
	}
}

void CL_NetObject_Server_Generic::send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message)
{
	CL_NetPacket netpacket;
	netpacket.output.write_bool8(false);
	netpacket.output.write_int32(obj_id);
	netpacket.output.write_int32(msg_type);
	netpacket.output.write(message.get_data(), message.get_size());
	group.send(controller->channel, netpacket);
}

void CL_NetObject_Server_Generic::send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message)
{
	CL_NetPacket netpacket;
	netpacket.output.write_bool8(false);
	netpacket.output.write_int32(obj_id);
	netpacket.output.write_int32(msg_type);
	netpacket.output.write(message.get_data(), message.get_size());
	computer.send(controller->channel, netpacket);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic implementation:
