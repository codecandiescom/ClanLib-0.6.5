/*
	$Id: socket_generic.cpp,v 1.7 2001/11/22 00:43:11 plasmoid Exp $

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

#include "socket_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic static operations:

sockaddr_in CL_Socket_Generic::create_sockaddr_in(const CL_IPAddress &address)
{
	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = address.get_address();
	addr_in.sin_port = htons(address.get_port());
	return addr_in;
}

#ifdef IPV6
sockaddr_in6 CL_Socket_Generic::create_sockaddr_in6(const CL_IPAddress &address)
{
	sockaddr_in6 addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin6_family = AF_INET6;
	memcpy(addr_in.sin6_addr.s6_addr,address.get_address6(),16);
	addr_in.sin6_port = htons(address.get_port());
	return addr_in;
}
#endif

CL_IPAddress CL_Socket_Generic::create_ip_address(const sockaddr_in &addr_in)
{
	return CL_IPAddress(addr_in.sin_addr.s_addr, ntohs(addr_in.sin_port));
}

#ifdef IPV6
CL_IPAddress CL_Socket_Generic::create_ip_address(const sockaddr_in6 &addr_in)
{
	return CL_IPAddress((void*)(&addr_in.sin6_addr));
}
#endif

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
	if (ref_count == 0)
	{
		#ifdef WIN32
		closesocket(sock);
		#else
		close(sock);
		#endif
		delete this;
	}
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

	int result = select(sock+1, &rfds, &wfds, &efds, &tv);
	if (result > 0)
	{
		if (FD_ISSET(sock, &rfds)) sig_read_triggered();
		if (FD_ISSET(sock, &wfds)) sig_write_triggered();
		if (FD_ISSET(sock, &efds)) sig_exception_triggered();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Socket_Generic implementation:
