
#include "API/Network2/NetObjects/netobject_controller.h"
#include "netobject_controller_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObjectController construction:

CL_NetObject_Controller::CL_NetObject_Controller(CL_NetSession *netsession, const std::string &channel)
: impl(0)
{
	impl = new CL_NetObject_Controller_Generic(netsession, channel);
}

CL_NetObject_Controller::CL_NetObject_Controller(const CL_NetObject_Controller &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Controller::~CL_NetObject_Controller()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObjectController attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller operations:

CL_NetObject_Controller &CL_NetObject_Controller::operator =(const CL_NetObject_Controller &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller signals:

CL_Signal_v3<CL_NetObject_Client &, int, CL_NetPacket &> &CL_NetObject_Controller::sig_create_object()
{
	return impl->sig_create_object;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller implementation:
