
#include "API/Network2/NetObjects/netobject_server.h"
#include "API/Network2/NetObjects/netobject_controller.h"
#include "netobject_server_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server construction:

CL_NetObject_Server::CL_NetObject_Server(CL_NetObject_Controller *controller)
: impl(0)
{
	impl = new CL_NetObject_Server_Generic(controller->impl);
	impl->add_ref();
}

CL_NetObject_Server::CL_NetObject_Server(const CL_NetObject_Server &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Server::~CL_NetObject_Server()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server attributes:

int CL_NetObject_Server::get_obj_id() const
{
	return impl->obj_id;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server operations:

void CL_NetObject_Server::send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message)
{
	impl->send(group, msg_type, message);
}

void CL_NetObject_Server::send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message)
{
	impl->send(computer, msg_type, message);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server signals:

CL_Signal_v2<const CL_NetComputer &, CL_NetPacket &> &CL_NetObject_Server::sig_received_message(int msg_type)
{
	return impl->sig_received_message[msg_type];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server implementation:
