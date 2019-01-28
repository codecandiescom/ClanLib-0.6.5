/*
	$Id: ip_address.cpp,v 1.1 2002/11/02 19:46:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifdef USE_GETADDR
#include "ip_address_getaddr.h"
#define CL_IPAddress_Platform CL_IPAddress_GetAddr
#else
#include "ip_address_getsock.h"
#define CL_IPAddress_Platform CL_IPAddress_GetSock
#endif

CL_IPAddress::CL_IPAddress()
{
	impl = new CL_IPAddress_Platform;
}

CL_IPAddress::CL_IPAddress(const std::string &port)
{
	impl = new CL_IPAddress_Platform(port);
}

CL_IPAddress::CL_IPAddress(const std::string &hostname, const std::string &port)
{
	impl = new CL_IPAddress_Platform(hostname, port);
}

CL_IPAddress::CL_IPAddress(const CL_IPAddress &copy)
{
	impl = new CL_IPAddress_Platform(copy);
}

void CL_IPAddress::get_addrinfo(int type, sockaddr &addr, int &len) const
{
	impl->get_addrinfo(type, addr, len);
}

std::string CL_IPAddress::get_address() const
{
	return impl->get_address();
}

std::string CL_IPAddress::get_port() const
{
	return impl->get_port();
}

bool CL_IPAddress::operator == (const CL_IPAddress &other_instance) const
{
	return impl->operator ==(other_instance);
}

bool CL_IPAddress::operator < (const CL_IPAddress &other_instance) const
{
	return impl->operator <(other_instance);
}

bool CL_IPAddress::operator > (const CL_IPAddress &other_instance) const
{
	return impl->operator >(other_instance);
}

void CL_IPAddress::set_address(const std::string &hostname)
{
	impl->set_address(hostname);
}

void CL_IPAddress::set_address(const std::string &hostname, const std::string &port)
{
	impl->set_address(hostname, port);
}

void CL_IPAddress::set_port(const std::string &new_port)
{
	impl->set_port(new_port);
}

std::string CL_IPAddress::dns_lookup() const
{
	return impl->dns_lookup();
}
