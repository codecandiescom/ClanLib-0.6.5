/*
	$Id: ip_address.cpp,v 1.11 2002/07/02 20:59:33 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "API/Network/ip_address.h"
#include "API/Core/System/error.h"

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <windows.h>
#endif

#ifndef INADDR_NONE
#define INADDR_NONE -1
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress construction:

CL_IPAddress::CL_IPAddress()
:  port(0)
{
	memset(ip,0,16);
#ifdef IPV6
	memcpy(ip,(void*)(&in6addr_any),sizeof(ip));
#else
	ip[0] = INADDR_ANY;
#endif
}

CL_IPAddress::CL_IPAddress(unsigned short port)
:  port(port)
{
	memset(ip,0,16);

#ifdef IPV6
	memcpy(ip,(void*)(&in6addr_any),sizeof(ip));
#else
	ip[0] = INADDR_ANY;
#endif

#ifdef DEBUG
	if(port < 1024)
		std::cout << "Warning: Using privledged port" << std::endl;
#endif
}

CL_IPAddress::CL_IPAddress(void *address)
: port(port)
{
	memset(ip,0,16);
  memcpy(ip,(void*)address,16);
#ifdef DEBUG
	if(port < 1024)
		std::cout << "Warning: Using privledged port" << std::endl;
#endif
}

CL_IPAddress::CL_IPAddress(unsigned int address, unsigned short port)
:  port(port)
{
	memset(ip,0,16);
	ip[0] = address;
#ifdef DEBUG
	if(port < 1024)
		std::cout << "Warning: Using privledged port" << std::endl;
#endif
}

CL_IPAddress::CL_IPAddress(const std::string &hostname, unsigned short port)
:  port(port)
{
	memset(ip,0,16);
	
#ifdef IPV6
	memcpy(ip,(void*)(&in6addr_any),sizeof(ip));
#else
	ip[0] = INADDR_ANY;
#endif

#ifdef DEBUG
	if(port < 1024)
		std::cout << "Warning: Using privledged port" << std::endl;
#endif
	set_address(hostname);
}

CL_IPAddress::CL_IPAddress(const CL_IPAddress &copy)
: port(copy.port)
{
	memcpy(ip,copy.ip,16);
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress attributes:

bool CL_IPAddress::operator == (const CL_IPAddress &other) const
{
	return memcmp(ip,other.ip,16) && (port == other.port);
}

bool CL_IPAddress::operator < (const CL_IPAddress &other) const
{
	if (memcmp(ip,other.ip,16)) return (port < other.port);
	return (ip < other.ip);
}

bool CL_IPAddress::operator > (const CL_IPAddress &other) const
{
	if (memcmp(ip,other.ip,16)) return (port > other.port);
	return (ip > other.ip);
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress operations:

void CL_IPAddress::set_address(const std::string &hostname)
{
	unsigned int addr = 0;

	addr = inet_addr(hostname.c_str());
	if (addr == INADDR_NONE)
	{
		hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) throw CL_Error("Could not lookup DNS name");

		addr = *((unsigned int*) host->h_addr_list[3]);
	}

	ip[0] = addr;
}

std::string CL_IPAddress::dns_lookup() const
{
#ifdef IPV6
	hostent *host = gethostbyaddr((const char *) &ip, sizeof(ip), AF_INET6);
#else
	hostent *host = gethostbyaddr((const char *) &ip[3], sizeof(ip[3]), AF_INET);
#endif
	if (host == NULL) throw CL_Error("Unable to lookup IP address");
	return std::string(host->h_name);
}
