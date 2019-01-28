
#include "API/Network2/NetSession/netsession.h"
#include "netsession_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession construction:

CL_NetSession::CL_NetSession(const std::string &app_id)
: impl(0)
{
	impl = new CL_NetSession_Generic(app_id);
	impl->add_ref();
}

CL_NetSession::CL_NetSession(const CL_NetSession &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetSession::~CL_NetSession()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession attributes:

CL_NetGroup &CL_NetSession::get_all()
{
	return impl->all;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession signals:

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_connected()
{
	return impl->sig_computer_connected;
}

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_reconnected()
{
	return impl->sig_computer_reconnected;
}

CL_Signal_v1<CL_NetComputer &> &CL_NetSession::sig_computer_disconnected()
{
	return impl->sig_computer_disconnected;
}

CL_Signal_v1<CL_NetStream &> &CL_NetSession::sig_netstream_connect(const std::string &netstream)
{
	return impl->map_netstream_connect[netstream];
}

CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &CL_NetSession::sig_netpacket_receive(const std::string &packetchannel)
{
	return impl->map_netpacket_receive[packetchannel];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession operators:

void CL_NetSession::start_listen(const std::string &port)
{
	impl->start_listen(port);
}

void CL_NetSession::stop_listen()
{
	impl->stop_listen();
}

CL_NetComputer CL_NetSession::connect(const CL_IPAddress &address)
{
	return impl->connect(address);
}

CL_NetComputer CL_NetSession::connect_async(const std::string &hostname, const std::string &port)
{
	return impl->connect_async(hostname, port);
}

void CL_NetSession::show_debug(bool enable)
{
	impl->show_debug = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession implementation:

CL_NetSession::CL_NetSession(CL_NetSession_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}
