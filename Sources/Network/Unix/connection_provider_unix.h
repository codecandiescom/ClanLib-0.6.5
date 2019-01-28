/*
	$Id: connection_provider_unix.h,v 1.4 2001/09/08 19:24:20 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_network_delivery_unix
#define header_network_delivery_unix

#include <list>

#include "Network/Generic/network_delivery_impl.h"
#include "API/Core/System/mutex.h"

class CL_UniformSocket;
class CL_UniformUDPConnection;
class CL_UniformAcceptSocket;

class CL_Connections_Unix : public CL_ConnectionProvider
{
protected:
	std::list<CL_UniformSocket *> connections;
	std::list<CL_UniformUDPConnection *> udp_connections;
	std::list<CL_UniformAcceptSocket *> accepting_connections;

public:
	CL_Connections_Unix();
	virtual ~CL_Connections_Unix();

	virtual CL_UDPConnection *create_udp_connection(unsigned int port);
	virtual CL_Connection *get_client();
	virtual CL_Connection *create_tcp_connection(
		int ip_addr,
		int port);
	virtual CL_Connection *accept();
	virtual void start_accept_on_port(int port);
	virtual void stop_accept_on_port(int port);
	virtual void wait_for_connection_data(CL_Mutex *mutex);
	virtual void remove_connection(CL_Connection *removed_connection);
};

#endif
