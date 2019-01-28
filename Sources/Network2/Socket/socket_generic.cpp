/*
	$Id: socket_generic.cpp,v 1.1 2002/11/02 19:46:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#define snprintf _snprintf
#endif

#include "socket_generic.h"
#include <cstdio>

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic static operations:
/*
sockaddr_in CL_Socket_Generic::create_sockaddr_in(const CL_IPAddress &address)
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = address.get_address();
	addr_in.sin_port = htons(address.get_port());
	return addr_in;
} */

CL_IPAddress CL_Socket_Generic::create_ip_address(const sockaddr_in &addr_in)
{
	char port[32];
	memset(port, 0, 32);
	
#ifdef _MSC_VER
	snprintf(port,6,"%d",ntohs(addr_in.sin_port));
#else
	std::snprintf(port,6,"%d",ntohs(addr_in.sin_port));
#endif
	
	char *addr = inet_ntoa(addr_in.sin_addr);
	
	return CL_IPAddress((std::string)addr, (std::string)port);

}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic construction:

CL_Socket_Generic::CL_Socket_Generic()
: sock(-1), read(NULL), write(NULL), exception(NULL) , ref_count(0)
{
}

CL_Socket_Generic::~CL_Socket_Generic()
{
	delete read;
	delete write;
	delete exception;

	disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic operations:

void CL_Socket_Generic::add_ref()
{
	ref_count++;
}

void CL_Socket_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic overridables:

CL_EventTrigger_Socket *CL_Socket_Generic::create_read_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::read);
}

CL_EventTrigger_Socket *CL_Socket_Generic::create_write_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::write);
}

CL_EventTrigger_Socket *CL_Socket_Generic::create_exception_trigger()
{
	return new CL_EventTrigger_Socket(this, CL_EventTrigger_Socket::exception);
}

void CL_Socket_Generic::keep_alive()
{
	// performance note: it would be much more efficient if we maintained a global
	// list of all sockets instanciated, and then do one select for them all.
	//
	// I'm too lazy to do that - at least until it becomes a real performance problem.
	// -- mbn 2. marts 2001

	fd_set rfds, wfds, efds;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	FD_SET(sock, &rfds);
	FD_SET(sock, &wfds);
	FD_SET(sock, &efds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int nread = 0;
	char buf = 0;

	int result = select(sock+1, &rfds, &wfds, &efds, &tv);
	if (result > 0)
	{
		if (FD_ISSET(sock, &rfds)) 
		{

			// Peek and see if there is anydata on the socket, if not 
			// the remote side has disconnected.
			nread = recv(sock, &buf, 1, MSG_PEEK);

			if(nread == 0)
			{
				sig_disconnected();
				disconnect();
			}
			else
				sig_read_triggered();
		}
		if (FD_ISSET(sock, &wfds)) sig_write_triggered();
		if (FD_ISSET(sock, &efds)) sig_exception_triggered();
	}
}

void CL_Socket_Generic::disconnect()
{

	if(sock != -1)
	{
		#ifdef WIN32
		closesocket(sock);
		#else
		close(sock);
		#endif
	}
	
	sock = -1;

}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic implementation:
