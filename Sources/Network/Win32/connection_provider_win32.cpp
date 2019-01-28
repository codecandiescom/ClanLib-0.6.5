/*
	$Id: connection_provider_win32.cpp,v 1.2 2001/03/29 11:58:45 mbn Exp $

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

#include "API/Core/System/mutex.h"
#include "API/Core/System/cl_assert.h" 
#include "API/Core/System/error.h"
#include "API/Core/System/system.h"
#include <Network/Generic/network_delivery_impl.h>
#include <Network/Generic/network_delivery_socket.h>
#include <Network/Win32/connection_provider_win32.h>

/*********************************************************************
				CL_Connections_Win32 Implementation
*********************************************************************/

CL_Connections_Win32::CL_Connections_Win32()
{
}

CL_Connections_Win32::~CL_Connections_Win32()
{
}

CL_UDPConnection *CL_Connections_Win32::create_udp_connection(unsigned int port)
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

CL_Connection *CL_Connections_Win32::create_tcp_connection(
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

CL_Connection *CL_Connections_Win32::accept()
{
	std::list<CL_UniformAcceptSocket *>::iterator counter=accepting_connections.begin();
	while (counter!=accepting_connections.end())
	{
		CL_UniformSocket *ret = (*counter)->accept();
		if (ret != NULL) 
		{
			connections.push_back(ret);
			return ret;
		}
		counter++;
	}
	return NULL;
}

void CL_Connections_Win32::start_accept_on_port(int port)
{
	CL_UniformAcceptSocket *new_accept_socket = new CL_UniformAcceptSocket(this);
	bool res = new_accept_socket->bind(port);

	if (res == false) throw CL_Error("Failed to bind socket to port");

	accepting_connections.push_back(new_accept_socket);
}

void CL_Connections_Win32::stop_accept_on_port(int port)
{
	std::list<CL_UniformAcceptSocket *>::iterator counter=accepting_connections.begin();
	while (counter!=accepting_connections.begin())
	{
		if ((*counter)->get_port() == port)
		{
			delete *counter;
			accepting_connections.erase(counter);
			return;
		}
	}
}

void CL_Connections_Win32::wait_for_connection_data(CL_Mutex *mutex)
{
	fd_set fdread;
	FD_ZERO(&fdread);

	fd_set fdwrite;
	FD_ZERO(&fdwrite);

	mutex->enter();

	bool added_connection = false;

	std::list<CL_UniformSocket *>::iterator counter=connections.begin();
	while (counter!=connections.end())
	{
		if ((*counter)->connection_lost() == false)
		{
			added_connection = true;

			FD_SET((*counter)->get_socket(), &fdread);
			if ((*counter)->send_avail() == false)
				FD_SET((*counter)->get_socket(), &fdwrite);
		}
		counter++;
	}

	std::list<CL_UniformUDPConnection *>::iterator counter2=udp_connections.begin();
	while (counter2!=udp_connections.end())
	{
		added_connection = true;

		FD_SET((*counter2)->get_socket(), &fdread);
//		if ((*counter2)->send_avail() == false)
//			FD_SET((*counter)->get_socket(), &fdwrite);
		counter2++;
	}

	std::list<CL_UniformAcceptSocket *>::iterator counter3=accepting_connections.begin();
	while (counter3!=accepting_connections.end())
	{
		added_connection = true;

		FD_SET((*counter3)->get_socket(), &fdread);
		if ((*counter3)->send_avail() == false)
			FD_SET((*counter)->get_socket(), &fdwrite);
		counter3++;
	}

	mutex->leave();

	if (added_connection)
	{
		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 1000;

		::select(0, &fdread, &fdwrite, NULL, &tv);
	}
	else
	{
		CL_System::sleep(1000);
	}
}

void CL_Connections_Win32::remove_connection(CL_Connection *removed_connection)
{
	std::list<CL_UniformSocket *>::iterator counter=connections.begin();
	while (counter!=connections.end())
	{
		if ((*counter) == removed_connection) 
		{
			connections.erase(counter);
			return;
		}
		counter++;
	}
}
