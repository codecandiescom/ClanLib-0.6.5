/*
	$Id: ip_address_getaddr.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef USE_GETADDR

#include "ip_address_getaddr.h"
#include "API/Core/System/error.h"
#include "API/Core/System/mutex.h"
#include "API/Network2/Socket/socket.h"

#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress construction:

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr() :
CL_IPAddress_Generic()
{
	// The error handling here isn't the best, ie it leaks memory
	memset(&hints,0,sizeof(hints));
	
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	int error = getaddrinfo(NULL, "0", &hints, &info);
	if(error)
		throw CL_Error(gai_strerror(error));

	hints.ai_socktype = SOCK_DGRAM;
	addrinfo *foo;
	error = getaddrinfo(NULL, "0", &hints, &foo);
	if(error)
		throw CL_Error(gai_strerror(error));
	
	addrinfo *tmp = info;
	while(tmp->ai_next)
	{
		tmp = tmp->ai_next;
	}
	tmp->ai_next = &foo[0];
	
	// Both lists have to merged by use because getaddrinfo can't return both at once.
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const std::string &port)
:CL_IPAddress_Generic()
{
	addrinfo hints;
	info = NULL;
	memset(&hints, 0, sizeof(hints));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	int error = getaddrinfo(NULL, port.c_str(), &hints, &info);

	if(error)
	{
		std::cout << gai_strerror(error) << std::endl;
		throw CL_Error(gai_strerror(error));
	}

	addrinfo *udp;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;
	error = getaddrinfo(NULL, port.c_str(), &hints, &udp);

	if( error )
	{
		std::cout << gai_strerror(error) << std::endl;
		throw CL_Error(gai_strerror(error));
	}
	
	addrinfo *tmp = &info[0];
	while(tmp->ai_next != NULL)
		tmp = tmp->ai_next;
	
	tmp->ai_next = udp;
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const std::string &hostname, const std::string &port)
:CL_IPAddress_Generic()
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0; // For now
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int error = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &info);
	
	if(error)
		throw CL_Error(gai_strerror(error));
}

CL_IPAddress_GetAddr::CL_IPAddress_GetAddr(const CL_IPAddress &copy)
:CL_IPAddress_Generic()
{
	set_address(copy.get_address(), copy.get_port());
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress attributes:

void CL_IPAddress_GetAddr::get_addrinfo(int type, sockaddr &addr, int &len) const
{
	addrinfo *tmp = info;

	while(tmp != NULL)
	{
		if(type == CL_Socket::tcp && (tmp->ai_socktype & SOCK_STREAM))
		{
			std::cout << "Addrlen " << sizeof(addr) << std::endl;
			len = tmp->ai_addrlen;
			memcpy(&addr,(tmp->ai_addr),sizeof(addr));
			return;
		}
		else if(type == CL_Socket::udp && (tmp->ai_socktype & SOCK_DGRAM))
		{
			len = tmp->ai_addrlen;
			memcpy(&addr,tmp->ai_addr,len);
			return;
		}
		tmp = tmp->ai_next;
	}
}

bool CL_IPAddress_GetAddr::operator == (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address() && get_port() == other.get_port())
		return true;
	else
		return false;
}

bool CL_IPAddress_GetAddr::operator < (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address())
		return atoi(get_port().c_str()) < atoi(other.get_port().c_str());
	else
		return get_address() < other.get_address();
}

bool CL_IPAddress_GetAddr::operator > (const CL_IPAddress &other) const
{
	if(get_address() == other.get_address())
		return atoi(get_port().c_str()) > atoi(other.get_port().c_str());
	else
		return get_address() > other.get_address();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IPAddress operations:

void CL_IPAddress_GetAddr::set_address(const std::string &hostname)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_protocol = PF_INET;
	int error = getaddrinfo(hostname.c_str(), get_port().c_str(), &hints, &info);

	if( error )
	{
		std::cout << "Error: " << gai_strerror(error) << std::endl;
		throw CL_Error(gai_strerror(error));
	}
}

void CL_IPAddress_GetAddr::set_address(const std::string &hostname, const std::string &port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0; // For now
	hints.ai_family = PF_INET;

	int error = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &info);
	
	if(error)
		throw CL_Error(gai_strerror(error));
}

void CL_IPAddress_GetAddr::set_port(const std::string &new_port)
{
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0; // For now
//	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int error = getaddrinfo(get_address().c_str(), new_port.c_str(), &hints, &info);

	std::cout << hints.ai_socktype << std::endl;
	
	if(error)
		throw CL_Error(gai_strerror(error));
}

std::string CL_IPAddress_GetAddr::dns_lookup() const
{
	char *name = new char[NI_MAXHOST];
	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, name, NI_MAXHOST, NULL, 0, 0);
	
	if(error)
	{
		delete[] name;
		throw CL_Error(gai_strerror(error));
	}
	
	std::string ret(name);
	delete[] name;
	
	return ret;
}

std::string CL_IPAddress_GetAddr::get_address() const
{
	char *addr = new char[NI_MAXHOST]; //Kinda ridiculous to allocate 1025 bytes for an address
	memset(addr, 0, NI_MAXHOST);

	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, addr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

	if(error)
	{
		delete[] addr;
		const char *err = gai_strerror(error);
		throw CL_Error((std::string)"Error in CL_IPAddress::get_address: " + (std::string)gai_strerror(error));
	}
	
	std::string ret(addr);
	delete[] addr;
	
	return ret;
}

std::string CL_IPAddress_GetAddr::get_port() const
{
	char *port = new char[NI_MAXSERV];
	memset(port, 0, NI_MAXSERV);
	
	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, NULL, 0, port, NI_MAXSERV, NI_NUMERICSERV);
	
	if(error)
	{
		delete[] port;
		throw CL_Error((std::string)"Error in CL_IPAddress::get_address: " + (std::string)gai_strerror(error));
	}
	
	std::string ret(port);
	delete[] port;
	
	return ret;
}

#endif

