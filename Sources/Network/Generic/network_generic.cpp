/*
	$Id: network_generic.cpp,v 1.4 2001/12/11 20:44:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef OBSOLUTE_STUFF

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <API/Core/System/error.h>
#include <API/Core/System/system.h>
#include <API/Network/netsession.h>
#include <API/Network/network.h>
#include <Network/Generic/network_delivery_impl.h>
#include <Network/Generic/netsession_client.h>
#include <Network/Generic/netsession_server.h>
#include <Network/Generic/network_generic.h>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <windows.h>
#endif

CL_Network_Generic *CL_Network_Generic::self = NULL;

CL_Network_Generic::CL_Network_Generic(CL_ConnectionProvider *provider)
{
	this->provider = provider;
	self = this;
	udp_connection = provider->create_udp_connection(0);
}

CL_Network_Generic::~CL_Network_Generic()
{
	delete udp_connection;
	delete provider;
	self = NULL;
}

void CL_Network_Generic::find_sessions_broadcast(
	std::string game_id,
	int port)
{
	CL_OutputSource_MemoryGeneric msg;
	msg.write_int32(0); // ping message.
	msg.write_int32(game_id.length());
	msg.write(game_id.c_str(), game_id.length());

	CL_UDPConnectionPacket netmsg;
	netmsg.ip_addr = 0;
	netmsg.port = port;
	netmsg.size = msg.size();
	netmsg.data = msg.get_data();

	udp_connection->broadcast(netmsg);
}

void CL_Network_Generic::find_session_at(
	std::string app_id,
	const std::string &host_address,
	int port)
{
	// lookup dns name, if needed.
	unsigned int addr = 0;
	
	addr = inet_addr(host_address);
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(host_address);
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

	queue.push(
		new CL_NetSession_Client(
			addr,
			port,
			app_id,
			this));
}

bool CL_Network_Generic::peek_game_found()
{
	if (udp_connection->peek())
	{
		CL_UDPConnectionPacket netmsg = udp_connection->receive();
		CL_InputSource_MemoryGeneric input(netmsg.data, netmsg.size);

		if (input.read_int32() == 1)
		{
			std::string game_id = input.read_string();
			queue.push(
				new CL_NetSession_Client(
					netmsg.ip_addr,
					netmsg.port,
					game_id,
					this));
		}
	}

	return !queue.empty();
}

CL_NetSession *CL_Network_Generic::receive_session_found(int timeout_millis)
{
	if (timeout_millis > 0) CL_System::sleep(timeout_millis);
	peek_game_found();

	if (queue.empty()) throw CL_Error("Game not found");
	CL_NetSession *session = queue.front();
	queue.pop();
	return session;
}

void CL_Network_Generic::clear_games_found()
{
	while (queue.empty() == false)
	{
		delete queue.front();
		queue.pop();
	}
}

CL_NetSession *CL_Network_Generic::create_session(std::string game_id, int port)
{
	return new CL_NetSession_Server(
		this,
		game_id.c_str(),
		port);
}

void CL_Network_Generic::keep_alive()
{
}

/*
	CL_Network API static function calls:
	-------------------------------------
*/

void CL_Network::find_sessions_broadcast(
	std::string game_id,
	int port)
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	CL_Network_Generic::self->find_sessions_broadcast(
		game_id,
		port);
}
	
void CL_Network::find_session_at(
	std::string app_id,
	const std::string &host_address,
	int port)
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	CL_Network_Generic::self->find_session_at(
		app_id,
		host_address,
		port);
}
	
bool CL_Network::peek_session_found()
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	return CL_Network_Generic::self->peek_game_found();
}

CL_NetSession *CL_Network::receive_session_found(int timeout)
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	return CL_Network_Generic::self->receive_session_found(timeout);
}

void CL_Network::clear_sessions_found()
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	CL_Network_Generic::self->clear_sessions_found();
}

CL_NetSession *CL_Network::create_session(
	std::string session_id,
	int port)
{
	if (CL_Network_Generic::self == NULL)
		throw CL_Error("Network not initialized!");

	return CL_Network_Generic::self->create_session(
		session_id,
		port);
}

// --- netgroup: ---

CL_NetGroup::CL_NetGroup()
{
}

CL_NetGroup::CL_NetGroup(CL_NetComputer *comp)
{
	computers.push_back(comp);
}

#endif
