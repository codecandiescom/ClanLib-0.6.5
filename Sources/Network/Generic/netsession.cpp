/*
	$Id: netsession.cpp,v 1.12 2002/07/02 20:59:33 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Network/netsession.h"
#include "netsession_generic.h"
#include "netsession_server.h"
#include "netsession_client.h"
#include "network_delivery_socket.h"

#ifdef WIN32
#include "../Win32/connection_provider_win32.h"
#else
#include "../Unix/connection_provider_unix.h"
#endif

#ifndef INADDR_NONE
#define INADDR_NONE -1
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession construction:

CL_NetSession::CL_NetSession(
	const std::string &app_id,
	int port)
:
	impl(NULL)
{
#ifdef WIN32
	CL_ConnectionProvider *connection_provider = new CL_Connections_Win32;
#else
	CL_ConnectionProvider *connection_provider = new CL_Connections_Unix;
#endif

	impl = new CL_NetSession_Server(connection_provider, app_id.c_str(), port);
	impl->add_ref();
}

CL_NetSession::CL_NetSession(
	const std::string &app_id,
	const std::string &hostname,
	int port)
:
	impl(NULL)
{
	// assume hostname is in dot notation:
	unsigned int addr = inet_addr(hostname.c_str());

	if (addr == INADDR_NONE)
	{
		// ok, hostname wasnt in dot notation. Let's try a dns lookup.

		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

#ifdef WIN32
	CL_ConnectionProvider *connection_provider = new CL_Connections_Win32;
#else
	CL_ConnectionProvider *connection_provider = new CL_Connections_Unix;
#endif

	impl = new CL_NetSession_Client(addr, port, app_id, connection_provider);
	impl->add_ref();
}

CL_NetSession::CL_NetSession(
	const CL_ServerInfo &server)
:
	impl(NULL)
{
	cl_assert(false); // not implemented yet.
}

CL_NetSession::CL_NetSession(const CL_NetSession &copy)
: impl(copy.impl)
{
	impl->add_ref();
}

CL_NetSession::~CL_NetSession()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession attributes:

CL_EventTrigger *CL_NetSession::get_trigger()
{
	return impl->get_trigger();
}

CL_NetComputer &CL_NetSession::get_server()
{
	return impl->get_server();
}

CL_NetGroup &CL_NetSession::get_all()
{
	return impl->get_all();
}

bool CL_NetSession::peek(int channel) const
{
	return impl->peek(channel);
}

CL_NetMessage CL_NetSession::receive(int channel, int timeout)
{
	return impl->receive(channel, timeout);
}

int CL_NetSession::access_status(int channel) const
{
	return impl->access_status(channel);
}

bool CL_NetSession::is_writable(int channel) const
{
	return impl->is_writable(channel);
}

bool CL_NetSession::is_readable(int channel) const
{
	return impl->is_readable(channel);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession operations:

void CL_NetSession::operator =(const CL_NetSession &session)
{
	if (impl != NULL) impl->release_ref();
	impl = session.impl;
	if (impl != NULL) impl->add_ref();
}

void CL_NetSession::send(
	const int dest_channel,
	const CL_NetGroup &dest,
	const CL_NetMessage &message,
	bool reliable)
{
	impl->send(
		dest_channel,
		dest,
		message,
		reliable);
}

CL_NetComputer CL_NetSession::receive_computer_leave()
{
	return impl->receive_computer_leave();
}

CL_NetComputer CL_NetSession::receive_computer_join()
{
	return impl->receive_computer_join();
}

CL_NetComputer CL_NetSession::receive_computer_rejoin()
{
	return impl->receive_computer_rejoin();
}

bool CL_NetSession::receive_session_closed()
{
	return impl->receive_session_closed();
}

int CL_NetSession::receive_access_changed()
{
	return impl->receive_access_changed();
}

void CL_NetSession::set_access(
	int channel,
	const CL_NetGroup &group,
	int access_rights)
{
	impl->set_access(channel, group, access_rights);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession signals:

/*CL_Signal_v1<CL_NetMessage &> &CL_NetSession::sig_receive_message(int netchannel)
{
	return impl->sig_receive_message;
}
*/
CL_Signal_v2<CL_NetMessage &, int> &CL_NetSession::sig_receive_message_default()
{
	return impl->sig_receive_message_default;
}

CL_Signal_v1<CL_NetComputer> &CL_NetSession::sig_computer_leave()
{
	return impl->sig_computer_leave;
}

CL_Signal_v1<CL_NetComputer> &CL_NetSession::sig_computer_join()
{
	return impl->sig_computer_join;
}

CL_Signal_v1<CL_NetComputer> &CL_NetSession::sig_computer_rejoin()
{
	return impl->sig_computer_rejoin;
}

CL_Signal_v1<int> &CL_NetSession::sig_access_changed()
{
	return impl->sig_access_changed;
}

CL_Signal_v0 &CL_NetSession::sig_session_closed()
{
	return impl->sig_session_closed;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession implementation:

CL_NetSession::CL_NetSession(CL_NetSession_Generic *impl)
: impl(impl)
{
	impl->add_ref();
}
