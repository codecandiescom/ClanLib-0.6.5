/*
	$Id: ip_address_getsock.cpp,v 1.2 2002/11/03 20:45:52 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef USE_GETADDR

#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "ip_address_getsock.h"

#include <cstdlib>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <windows.h>
#endif


/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress construction:

CL_IPAddress_GetSock::CL_IPAddress_GetSock()
: ip(INADDR_ANY), port(0)
{
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const std::string &new_port)
: ip(INADDR_ANY)
{
	port = CL_String(new_port).get_as_int();
	if(port > 65535)
		throw CL_Error("Error: Port specified as greater than 65535");
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const std::string &hostname, const std::string &new_port)
{
	port = CL_String(new_port).get_as_int();
	if(port > 65535)
		throw CL_Error("Error: Port specified as greater than 65535");

	unsigned int addr = 0;

	addr = inet_addr(hostname.c_str());
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

	ip = addr;
}

CL_IPAddress_GetSock::CL_IPAddress_GetSock(const CL_IPAddress &copy)
{
	set_address(copy.get_address(),copy.get_port());
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress attributes:

void CL_IPAddress_GetSock::get_addrinfo(int type, sockaddr &addr, int &len) const
{
	sockaddr_in ret;
	ret.sin_family = AF_INET;
	ret.sin_port = htons(port);
	ret.sin_addr.s_addr = ip;
	len = sizeof(sockaddr_in);
	memcpy(&addr, &ret, len);
}

std::string CL_IPAddress_GetSock::get_address() const
{
	struct in_addr foo;
	foo.s_addr = ip;
	char *addr = inet_ntoa(foo);
	return std::string(addr);	
}

std::string CL_IPAddress_GetSock::get_port() const
{
	CL_String port(port);
	return port;
}

bool CL_IPAddress_GetSock::operator == (const CL_IPAddress &other) const
{
	return (get_address() == other.get_address()) && (get_port() == other.get_port());
}

bool CL_IPAddress_GetSock::operator < (const CL_IPAddress &other) const
{
	if (get_address() == other.get_address())
		return atoi(get_port().c_str()) < atoi(other.get_port().c_str());
	return get_address() < other.get_address();
}

bool CL_IPAddress_GetSock::operator > (const CL_IPAddress &other) const
{
	if (get_address() == other.get_address())
		return atoi(get_port().c_str()) > atoi(other.get_port().c_str());
	return get_address() > other.get_address();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress operations:

void CL_IPAddress_GetSock::set_address(const std::string &hostname)
{
	unsigned int addr = 0;

	addr = inet_addr(hostname.c_str());
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[0]);
	}

	ip = addr;
}

void CL_IPAddress_GetSock::set_address(const std::string &hostname, const std::string &port)
{
	set_address(hostname);
	set_port(port);
}

void CL_IPAddress_GetSock::set_port(const std::string &new_port)
{
	port = atoi(new_port.c_str());
	if(port > 65535)
		throw CL_Error("Error: Port specified as greater than 65535.");
}

std::string CL_IPAddress_GetSock::dns_lookup() const
{
	hostent *host = gethostbyaddr((const char *) &ip, sizeof(ip), AF_INET);
	if (host == NULL) throw CL_Error("Unable to lookup IP address");
	return std::string(host->h_name);
}

#endif
