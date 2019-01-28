/*
	$Id: connection_provider_unix.cpp,v 1.3 2001/04/23 21:46:43 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_NETWORK

#include <API/Core/System/cl_assert.h>
#include <API/Core/System/error.h>
#include <API/Core/System/mutex.h>
#include <Network/Generic/network_delivery_impl.h>
#include <Network/Generic/network_delivery_socket.h>
#include <Network/Unix/network_unix.h>
#include <Network/Unix/connection_provider_unix.h>

/*********************************************************************
				CL_Connections_Unix Implementation
*********************************************************************/

CL_Connections_Unix::CL_Connections_Unix()
{
}

CL_Connections_Unix::~CL_Connections_Unix()
{
}

CL_UDPConnection *CL_Connections_Unix::create_udp_connection(unsigned int port)
{
	CL_UniformUDPConnection *ret = new CL_UniformUDPConnection;
	bool res = ret->bind(port);

	if (res) 
	{
		udp_connections.push_back(ret);
		return ret;
	}
	else
	{
		delete ret;
		return NULL;
	}
}

CL_Connection *CL_Connections_Unix::get_client()
{
	return NULL;
}

CL_Connection *CL_Connections_Unix::create_tcp_connection(
	int ip_addr,
	int port)
{
	CL_UniformSocket *socket = new CL_UniformSocket(this);
	socket->init_socket();
	bool res = socket->try_connect(ip_addr, port);

	if (res) 
	{
		connections.push_back(socket);
		return socket;
	}
	else
	{
		delete socket;
		return NULL;
	}
}

CL_Connection *CL_Connections_Unix::accept()
{
	for (
		std::list<CL_UniformAcceptSocket*>::iterator it = accepting_connections.begin();
		it != accepting_connections.end();
		it++)
	{
		CL_UniformSocket *ret = (*it)->accept();
		if (ret != NULL) 
		{
			connections.push_back(ret);
			return ret;
		}
	}
	return NULL;
}

void CL_Connections_Unix::start_accept_on_port(int port)
{
	CL_UniformAcceptSocket *new_accept_socket = new CL_UniformAcceptSocket(this);
	bool res = new_accept_socket->bind(port);

	// Argh - we need some robustness bad!
	if (res == false) throw CL_Error("Port already in use an other application!");

	accepting_connections.push_back(new_accept_socket);
}

void CL_Connections_Unix::stop_accept_on_port(int port)
{
	for (
		std::list<CL_UniformAcceptSocket *>::iterator it = accepting_connections.begin();
		it != accepting_connections.end();
		it++)
	{
		if ((*it)->get_port() == port)
		{
			delete (*it);
			it = accepting_connections.erase(it);
			return;
		}
	}
}

void CL_Connections_Unix::wait_for_connection_data(CL_Mutex *mutex)
{
	mutex->enter();

	fd_set fdread;
	FD_ZERO(&fdread);

	fd_set fdwrite;
	FD_ZERO(&fdwrite);

	int highest_fd = -1;
/*
	if (client_connection != NULL)
	{
		highest_fd = client_connection->get_socket();
		FD_SET(client_connection->get_socket(), &fdread);
	}
*/
	for (
		std::list<CL_UniformSocket *>::iterator counter = connections.begin();
		counter != connections.end();
		counter++)
	{
		if ((*counter)->connection_lost() == false)
		{
			if ((*counter)->get_socket() > highest_fd)
				highest_fd = (*counter)->get_socket();

			FD_SET((*counter)->get_socket(), &fdread);

			if ((*counter)->send_avail() == false)
				FD_SET((*counter)->get_socket(), &fdwrite);
		}
	}

	for (
		std::list<CL_UniformUDPConnection *>::iterator counter2 = udp_connections.begin();
		counter2 != udp_connections.end();
		counter2++)
	{
		if ((*counter2)->get_socket() > highest_fd)
			highest_fd = (*counter2)->get_socket();

		FD_SET((*counter2)->get_socket(), &fdread);
//		if ((*counter2)->send_avail() == false)
//			FD_SET((*counter)->get_socket(), &fdwrite);
	}

	for (
		std::list<CL_UniformAcceptSocket *>::iterator counter3 = accepting_connections.begin();
		counter3 != accepting_connections.end();
		counter3++)
	{
		if ((*counter3)->get_socket() > highest_fd)
			highest_fd = (*counter3)->get_socket();

		if ((*counter3)->send_avail() == false)
			FD_SET((*counter3)->get_socket(), &fdwrite);

		FD_SET((*counter3)->get_socket(), &fdread);
	}
	
	mutex->leave();

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	::select(highest_fd+1, &fdread, &fdwrite, NULL, &tv);

	// don't assert if select() fails. It can happen because we select
	// outside the mutex section, and the game thread may close a socket in
	// the mean time.
}

void CL_Connections_Unix::remove_connection(CL_Connection *removed_connection)
{
	connections.remove((CL_UniformSocket *) removed_connection);
}

#endif // USE_NETWORK
