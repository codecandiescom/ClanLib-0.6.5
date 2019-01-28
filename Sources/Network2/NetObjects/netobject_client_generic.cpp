
#include "API/Network2/NetSession/netpacket.h"
#include "netobject_client_generic.h"
#include "netobject_controller_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client construction:

CL_NetObject_Client_Generic::CL_NetObject_Client_Generic(
	int obj_id,
	const CL_NetComputer &server,
	CL_NetObject_Controller_Generic *controller)
: obj_id(obj_id), server(server), controller(controller), ref_count(0)
{
	CL_NetObject_Controller_Generic::ClientID client_id(server, obj_id);
	controller->client_objects[client_id] = this;
}

CL_NetObject_Client_Generic::~CL_NetObject_Client_Generic()
{
	CL_NetObject_Controller_Generic::ClientID client_id(server, obj_id);
	controller->client_objects.erase(controller->client_objects.find(client_id));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client_Generic operations:

void CL_NetObject_Client_Generic::receive(int msg_type, CL_NetPacket &packet)
{
	std::map< int, CL_Signal_v1<CL_NetPacket &> >::iterator it;
	it = sig_received_message.find(msg_type);
	if (it != sig_received_message.end())
	{
		it->second(packet);
	}
	else
	{
		// No message handler found.
		// Inform application somehow?
	}
}

void CL_NetObject_Client_Generic::send(int msg_type, const CL_NetPacket &message)
{
	CL_NetPacket netpacket;
	netpacket.output.write_bool8(true);
	netpacket.output.write_int32(obj_id);
	netpacket.output.write_int32(msg_type);
	netpacket.output.write(message.get_data(), message.get_size());
	server.send(controller->channel, netpacket);
}
