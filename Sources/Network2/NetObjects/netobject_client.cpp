
#include "API/Network2/NetObjects/netobject_client.h"
#include "API/Network2/NetObjects/netobject_controller.h"
#include "netobject_client_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client construction:

CL_NetObject_Client::CL_NetObject_Client(int obj_id, const CL_NetComputer &server, CL_NetObject_Controller_Generic *controller)
: impl(0)
{
	impl = new CL_NetObject_Client_Generic(obj_id, server, controller);
	impl->add_ref();
}

CL_NetObject_Client::CL_NetObject_Client(const CL_NetObject_Client &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Client::~CL_NetObject_Client()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client attributes:

int CL_NetObject_Client::get_obj_id() const
{
	return impl->obj_id;
}

CL_NetComputer &CL_NetObject_Client::get_server() const
{
	return impl->server;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client operations:

void CL_NetObject_Client::send(int msg_type, const CL_NetPacket &message)
{
	impl->send(msg_type, message);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client signals:

CL_Signal_v1<CL_NetPacket &> &CL_NetObject_Client::sig_received_message(int msg_type)
{
	return impl->sig_received_message[msg_type];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client implementation:
