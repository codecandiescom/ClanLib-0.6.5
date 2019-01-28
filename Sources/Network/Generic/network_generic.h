/*
#ifndef header_network_generic
#define header_network_generic

#include <stdio.h>
#include <list>
#include <queue>
#include <string>

#include "API/Network/network.h"
#include "API/Network/netcomputer.h"
#include "API/Network/netgroup.h"
#include "API/Network/netsession.h"
#include "API/Network/netmessage.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/error.h"
#include "API/Core/System/keep_alive.h"

#include "Core/IOData/Generic/inputsource_memory_generic.h"
#include "Core/IOData/Generic/outputsource_memory_generic.h"

#include "network_delivery_impl.h"

class CL_NetSession_Generic;
class CL_IncomingGame;

class CL_Network_Generic : public CL_Network, CL_KeepAlive
{
public:
	CL_Network_Generic(CL_ConnectionProvider *provider);
	~CL_Network_Generic();

	void find_sessions_broadcast(
		std::string game_id,
		int port);
	void find_session_at(
		std::string game_id,
		const std::string &host_address,
		int port);
	bool peek_game_found();
	CL_NetSession *receive_session_found(int timeout_millis);
	void clear_games_found();
	CL_NetSession *create_session(std::string session_id, int port);

	CL_ConnectionProvider *provider;
	std::queue<CL_NetSession*> queue;
	std::list<CL_NetSession_Generic*> netsessions;
	std::list<CL_IncomingGame*> incoming_netsessions;

	static CL_Network_Generic *self;
	
	void keep_alive();
private:
	CL_UDPConnection *udp_connection;
};

class CL_IncomingGame
{
public:
	CL_Connection *connection;
	std::string game_id;
};

#endif
*/

