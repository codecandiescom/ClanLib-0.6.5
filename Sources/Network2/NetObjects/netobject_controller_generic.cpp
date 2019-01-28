
#include "netobject_controller_generic.h"
#include "netobject_server_generic.h"
#include "netobject_client_generic.h"
#include "API/Network2/NetObjects/netobject_client.h"
#include "API/Network2/NetSession/netpacket.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic construction:

CL_NetObject_Controller_Generic::CL_NetObject_Controller_Generic(
	CL_NetSession *netsession,
	const std::string &channel)
: netsession(netsession), channel(channel), obj_id_counter(0), ref_count(0)
{
	slot_received_netpacket = netsession->sig_netpacket_receive(channel).connect(
		this, &CL_NetObject_Controller_Generic::on_received_netpacket);
}

CL_NetObject_Controller_Generic::~CL_NetObject_Controller_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic operations:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic implementation:

void CL_NetObject_Controller_Generic::on_received_netpacket(
	CL_NetPacket &packet,
	CL_NetComputer &from)
{
	bool server_obj = packet.input.read_bool8();
	int obj_id = packet.input.read_int32();
	int msg_type = packet.input.read_int32();
	int sub_pos = packet.input.tell();
	CL_NetPacket subpacket(packet.get_data()+sub_pos, packet.get_size()-sub_pos);

	if (server_obj)
	{
		std::map<int, CL_NetObject_Server_Generic *>::iterator it;
		it = server_objects.find(obj_id);
		if (it != server_objects.end())
		{
			it->second->receive(msg_type, from, subpacket);
		}
		else
		{
			// No such server object. Junk message.
			// Maybe we should allow application to deal with this kind of messages?
		}
	}
	else
	{
		std::map<ClientID, CL_NetObject_Client_Generic *>::iterator it;
		it = client_objects.find(ClientID(from, obj_id));
		if (it != client_objects.end())
		{
			it->second->receive(msg_type, subpacket);
		}
		else
		{
			// No such client object. Construct new one and inform application.

			CL_NetObject_Client netobj(obj_id, from, this);
			sig_create_object(netobj, msg_type, subpacket);
		}
	}
}
