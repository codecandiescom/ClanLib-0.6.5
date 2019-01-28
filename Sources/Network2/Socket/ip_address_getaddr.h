/*
	$Id: ip_address_getaddr.h,v 1.1 2002/11/02 19:46:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef ip_address_getaddr_header
#define ip_address_getaddr_header

#include "ip_address_generic.h"

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#define _inline inline
	#include <ws2tcpip.h>
	#include <tpipv6.h>
	#include <windows.h>
#endif

//: Socket name; container class for an IP address and port.
class CL_IPAddress_GetAddr : public CL_IPAddress_Generic
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress_GetAddr();

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress_GetAddr(const std::string &port);

	//: Constructs an IP address using the passed address.
//	CL_IPAddress(const std::string &hostname);

	//: Construct an IP address using the hostname and the port.
	CL_IPAddress_GetAddr(const std::string &hostname, const std::string &port);

	//: Copy constructor.
	CL_IPAddress_GetAddr(const CL_IPAddress &copy);

//! Attributes:
public:
	//: Returns the internal addrinfo struct
	void get_addrinfo(int type, sockaddr &addr, int &len) const;

	//: Returns the IP address in network byte order.
	std::string get_address() const;

	//: Returns the IP port in host byte order.
	std::string get_port() const;

	//: Returns true if objects are the same.
	bool operator == (const CL_IPAddress &other_instance) const;

	//: Returns true if the other address is less.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	bool operator < (const CL_IPAddress &other_instance) const;

	//: Returns true if the other address is greater.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	bool operator > (const CL_IPAddress &other_instance) const;

//! Operations:
public:
	//: Set the IP address using a hostname.
	//param hostname: Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.  The port number cannot be passed here.
	void set_address(const std::string &hostname);

	void set_address(const std::string &hostname, const std::string &port);

	//: Set the IP port.
	void set_port(const std::string &new_port);

	//: Do a DNS lookup on the IP address.
	std::string dns_lookup() const;

private:
	addrinfo hints;
	addrinfo *info;
};


#endif