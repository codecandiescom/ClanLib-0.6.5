/*
	$Id: network_unix.cpp,v 1.2 2001/02/23 00:04:50 cybernerd Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_NETWORK

#include <iostream>
#include <netdb.h>
#include <signal.h>

#include <Network/Generic/network_generic.h>
#include <Network/Unix/connection_provider_unix.h>
#include <Network/Unix/network_unix.h>
#include <Core/System/Unix/implementation.h>

void signal_pipe(int)
{
/*
	delete CL_Implementation_Network::delivery;
	exit(0); // bye bye
*/
	// hmm - who cares!
}

CL_Network_Generic *CL_Network_Unix::network = NULL;

extern "C"
{
        DL_PREFIX char *clan_module_identify() {
	        return "Clanlib Network implementation";
	}

        DL_PREFIX char *clan_module_abbreviation() {
	        return "net";
	}

        DL_PREFIX void clan_module_init() {
		CL_Implementation_Network::add_network();
	}
}

char *network_identify() {
  return clan_module_identify();
}

char *network_abbreviation() {
  return clan_module_abbreviation();
}

void network_init() {
  clan_module_init();
}


/*
void CL_Implementation_Network::add_network()
{
	char name[100];
	gethostname(name, 100);

	hostent *ent = gethostbyname(name);

	unsigned long local_ip = *((unsigned long *) *ent->h_addr_list);

	int pipes[4];
	pipe(pipes);
	pipe(&pipes[2]);

	int our_pid = getpid();
	if (fork()==0)
	{
		CL_UnixPipeConnection *delivery = new CL_UnixPipeConnection(pipes[0], pipes[3]);
		CL_Network_Unix::network = new CL_Network_Generic(local_ip, our_pid, delivery);

		CL_System_Generic::keep_alives.add(new CL_Network_Unix);
	}
	else
	{
		signal(SIGPIPE, signal_pipe);
		signal(SIGHUP, signal_pipe);
		signal(SIGCHLD, signal_pipe);
		signal(SIGTERM, signal_pipe);

		CL_ComputerID our_id;
		our_id.ip_addr = local_ip;
		our_id.pid = our_pid;

		CL_UnixPipeConnection delivery_connection(pipes[2], pipes[1]);
		CL_Connections_Unix *connections = new CL_Connections_Unix(&delivery_connection);
		
		delivery = new CL_NetDelivery(connections, our_id);
		delivery->run();
		delete delivery;

		exit(0);
	}
}
*/

void CL_Implementation_Network::add_network()
{
	CL_Connections_Unix *connections = new CL_Connections_Unix(NULL);

	signal(SIGPIPE, signal_pipe);

	CL_Network_Unix::network = new CL_Network_Generic(connections);
}

void CL_Implementation_Network::remove_network()
{
	if (CL_Network_Unix::network != NULL) delete CL_Network_Unix::network;
	CL_Network_Unix::network = NULL;
}

#endif // USE_NETWORK
