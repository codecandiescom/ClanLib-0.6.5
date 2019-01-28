/*
	$Id: ip_address_generic.h,v 1.2 2002/11/02 19:57:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef ip_address_generic_header
#define ip_address_generic_header

#include <string>
#include "API/Network2/Socket/ip_address.h"

//: Socket name; container class for an IP address and port.
class CL_IPAddress_Generic
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress_Generic() {;}

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress_Generic(const std::string &port) {;}

	//: Constructs an IP address using the passed address.
//	CL_IPAddress(const std::string &hostname);

	//: Construct an IP address using the hostname and the port.
	CL_IPAddress_Generic(const std::string &hostname, const std::string &port) {;}

	//: Copy constructor.
	CL_IPAddress_Generic(const CL_IPAddress_Generic &copy) {;}

//! Attributes:
public:
	//: Returns the internal addrinfo struct
	virtual void get_addrinfo(int type, sockaddr &addr, int &len) const=0;

	//: Returns the IP address in network byte order.
	virtual std::string get_address() const=0;

	//: Returns the IP port in host byte order.
	virtual std::string get_port() const=0;

	//: Returns true if objects are the same.
	virtual bool operator == (const CL_IPAddress &other_instance) const=0;

	//: Returns true if the other address is less.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	virtual bool operator < (const CL_IPAddress &other_instance) const=0;

	//: Returns true if the other address is greater.
	//- <p>This is used for sorting
	//- purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//- the address based on lowest IP number address.</p>
	virtual bool operator > (const CL_IPAddress &other_instance) const=0;

//! Operations:
public:
	//: Set the IP address using a hostname.
	//param hostname: Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.  The port number cannot be passed here.
	virtual void set_address(const std::string &hostname)=0;

	virtual void set_address(const std::string &hostname, const std::string &port)=0;

	//: Set the IP port.
	virtual void set_port(const std::string &new_port)=0;

	//: Do a DNS lookup on the IP address.
	virtual std::string dns_lookup() const=0;
};

#endif
