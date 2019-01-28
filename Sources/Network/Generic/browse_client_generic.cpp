/*
	$Id: browse_client_generic.cpp,v 1.6 2001/10/14 18:05:07 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#pragma warning (disable:4355)
#include <winsock2.h> // needed for ntohl and ntohs
#else
#include <netinet/in.h>
#endif

#include "browse_client_generic.h"
#include "API/Core/System/error.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/IOData/outputsource_memory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient_Generic construction:

CL_BrowseClient_Generic::CL_BrowseClient_Generic(
	const CL_IPAddress &browse_master,
	const std::string &app_id)
:
	browse_master(browse_master),
	app_id(app_id),
	sock(CL_Socket::tcp),
	thread(this),
	stop_thread(false)
{
	thread.start();
}

CL_BrowseClient_Generic::~CL_BrowseClient_Generic()
{
	// shut down worker thread.
	stop_thread = true;
	thread.wait();
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient_Generic operations:

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient_Generic implementation:

void CL_BrowseClient_Generic::keep_alive()
{
	CL_MutexSection mutex_lock(&mutex);

	std::list< std::pair<CL_IPAddress, std::string> >::iterator it;
	std::list<CL_IPAddress>::iterator it2;
	
	for (it = added_list.begin(); it != added_list.end(); it++)
	{
		sig_server_added(it->first, it->second);
	}

	for (it = updated_list.begin(); it != updated_list.end(); it++)
	{
		sig_server_updated(it->first, it->second);
	}

	for (it2 = removed_list.begin(); it2 != removed_list.end(); it2++)
	{
		sig_server_removed(*it2);
	}

	added_list = std::list< std::pair<CL_IPAddress, std::string> >();
	updated_list = std::list< std::pair<CL_IPAddress, std::string> >();
	removed_list = std::list<CL_IPAddress>();

	// Pass on exception in worker thread to user thread.
	if (exception_thrown) throw CL_Error(exception);
}

void CL_BrowseClient_Generic::run()
{
	char *buffer = NULL;

	try
	{
		sock.connect(browse_master);

		CL_OutputSource_Memory output;
		output.write_string(app_id.c_str());
		output.write_int32(1);      // we are a client
		sock.send(output.get_data().data(), output.get_data().size());

		while (true)
		{
			if (sock.get_read_trigger()->wait(100) == false)
			{
				if (stop_thread) break;
				continue;
			}

			int type = 0;
			if ((unsigned int)sock.recv(&type, sizeof(int)) < sizeof(int)) throw CL_Error("EOF during read operation");
			type = ntohl(type);

			if (type == 0 || type == 1) // added or modified
			{
				unsigned int ip = 0;
				unsigned short port = 0;

				if ((unsigned int)sock.recv(&ip, sizeof(int)) < sizeof(int)) throw CL_Error("EOF during read operation");
				if ((unsigned int)sock.recv(&port, sizeof(short)) < sizeof(short)) throw CL_Error("EOF during read operation");
				port = ntohs(port);

				unsigned int size = 0;
				if ((unsigned int)sock.recv(&size, sizeof(int)) < sizeof(int)) throw CL_Error("EOF during read operation");
				size = ntohl(size);
				buffer = new char[size];
				if ((unsigned int)sock.recv(buffer, size) < size) throw CL_Error("EOF during read operation");

				CL_IPAddress addr(ip, port);
				std::pair<CL_IPAddress, std::string> params(addr, std::string(buffer, size));

				CL_MutexSection mutex_lock(&mutex);
				if (type == 0) added_list.push_back(params);
				else updated_list.push_back(params);

				delete[] buffer;
				buffer = NULL;
			}
			else
			{
				unsigned int ip = 0;
				unsigned short port = 0;

				if ((unsigned int)sock.recv(&ip, sizeof(int)) < sizeof(int)) throw CL_Error("EOF during read operation");
				if ((unsigned int)sock.recv(&port, sizeof(short)) < sizeof(short)) throw CL_Error("EOF during read operation");
				port = ntohs(port);
				CL_IPAddress addr(ip, port);

				CL_MutexSection mutex_lock(&mutex);
				removed_list.push_back(addr);
			}
		}
	}
	catch (CL_Error err)
	{
		delete[] buffer;

		CL_MutexSection mutex_lock(&mutex);
		exception = err.message;
		exception_thrown = true;
	}
}
