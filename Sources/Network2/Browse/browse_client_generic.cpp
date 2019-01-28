/*
	$Id: browse_client_generic.cpp,v 1.1 2002/11/02 19:46:04 mbn Exp $

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

CL_BrowseClient_Generic::CL_BrowseClient_Generic(const std::string &app_id)
: app_id(app_id), sock(CL_Socket::tcp), thread(this), stop_thread(false)
{
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

void CL_BrowseClient_Generic::connect(const CL_IPAddress &new_browse_master)
{
	browse_master = new_browse_master;
	thread.start();
}

void CL_BrowseClient_Generic::broadcast(int port)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_BrowseClient_Generic implementation:

void CL_BrowseClient_Generic::keep_alive()
{
	CL_MutexSection mutex_lock(&mutex);

	std::list< std::pair<CL_IPAddress, CL_NetPacket> >::iterator it;
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

	added_list.clear();
	updated_list.clear();
	removed_list.clear();

	// Pass on exception in worker thread to user thread.
	if (exception_thrown) throw CL_Error(exception);
}

void CL_BrowseClient_Generic::run()
{
	try
	{
		sock.connect(browse_master);

		sock.output.write_string(app_id.c_str());
		sock.output.write_int32(1);      // we are a client

		while (true)
		{
			if (sock.get_read_trigger()->wait(100) == false)
			{
				if (stop_thread) break;
				continue;
			}

			int type = sock.input.read_int32();
			if (type == 0) // added or modified
			{
				std::string address = sock.input.read_string();
				std::string port = sock.input.read_string();
				std::string desc = sock.input.read_string();
				
				CL_IPAddress addr(address, port);
				std::pair<CL_IPAddress, CL_NetPacket> params(addr, desc);

				CL_MutexSection mutex_lock(&mutex);
				// added_list.push_back(params);
				updated_list.push_back(params);
			}
			else // removed
			{
				std::string address = sock.input.read_string();
				std::string port = sock.input.read_string();

				CL_IPAddress addr(address, port);

				CL_MutexSection mutex_lock(&mutex);
				removed_list.push_back(addr);
			}
		}
	}
	catch (CL_Error err)
	{
		CL_MutexSection mutex_lock(&mutex);
		exception = err.message;
		exception_thrown = true;
	}
}
