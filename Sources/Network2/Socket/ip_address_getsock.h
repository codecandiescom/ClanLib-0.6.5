/*
	$Id: ip_address_getsock.h,v 1.1 2002/11/02 19:46:07 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_ip_address_getsock
#define header_ip_address_getsock

#include <string>
#include "ip_address_generic.h"

//: Socket name; container class for an IP address and port.
class CL_IPAddress_GetSock : public CL_IPAddress_Generic
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress_GetSock();

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress_GetSock(const std::string &port);

	//: Construct an IP address using a hostname.
	CL_IPAddress_GetSock(const std::string &hostname, const std::string &port);

	//: Copy constructor.
	CL_IPAddress_GetSock(const CL_IPAddress &copy);

//! Attributes:
public:
	//: Returns the internal addrinfo struct
	void get_addrinfo(int type, sockaddr &addr, int &len) const;

	//: Returns the IP address as a string
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
	//: Set the IP address, in network byte order.
	void set_address(const std::string &hostname);

	//: Set the IP address using a hostname.
	void set_address(const std::string &hostname, const std::string &port);

	//: Set the IP port.
	void set_port(const std::string &new_port);

	//: Do a DNS lookup on the IP address.
	std::string dns_lookup() const;

//! Implementation:
private:
	// IP address in network byte order.
	unsigned int ip;

	// IP port in host byte order.
	unsigned short port;
};

#endif
