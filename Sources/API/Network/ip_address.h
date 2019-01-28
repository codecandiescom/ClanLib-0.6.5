/*
	$Id: ip_address.h,v 1.20 2001/11/30 22:38:40 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="Sockets"
//! header=network.h

#ifndef header_ip_address
#define header_ip_address

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

//: Socket name; container class for an IP address and port.
class CL_IPAddress
{
//! Construction:
public:
	//: Constructs an IP address using INADDR_ANY and port 0.
	CL_IPAddress();

	//: Constructs an IP address using INADDR_ANY and the specified port.
	CL_IPAddress(unsigned short port);

	//: Constructor for IPv6 address
	CL_IPAddress(void *address);

	//: Constructs an IP address using the passed address and port.
	CL_IPAddress(unsigned int address, unsigned short port);

	//: Construct an IP address using a hostname.
	CL_IPAddress(const std::string &hostname, unsigned short port);

	//: Copy constructor.
	CL_IPAddress(const CL_IPAddress &copy);

//! Attributes:
public:
	//: Returns the IP address in network byte order.
	unsigned int get_address() const { return ip[3]; }

	unsigned int *get_address6() const { return (unsigned int * const)ip; }

	//: Returns the IP port in host byte order.
	unsigned short get_port() const { return port; }

	//: Returns the specified address field.
	unsigned char &operator[](int i) { return ((unsigned char*) &ip)[i]; }

	//: Returns the specified address field.
	const unsigned char &operator[](int i) const { return ((unsigned char*) &ip)[i]; }

	//: Returns true if objects are the same.
	bool operator == (const CL_IPAddress &other_instance) const;

	//: <p>Returns true if the other address is less. This is used for sorting
	//: purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//: the address based on lowest IP number address.</p>
	bool operator < (const CL_IPAddress &other_instance) const;

	//: <p>Returns true if the other address is greater. This is used for sorting
	//: purposes (eg. if you use a std::map<CL_IPAddress, CL_Socket>), and sorts
	//: the address based on lowest IP number address.</p>
	bool operator > (const CL_IPAddress &other_instance) const;

//! Operations:
public:
	//: Set the IP address, in network byte order.
	void set_address(unsigned int new_ip) { ip[3] = new_ip; }

	//: Set the IP address using a hostname.
	void set_address(const std::string &hostname);

	//: Set the IP port.
	void set_port(unsigned short new_port) 
	{ 
	#ifdef DEBUG
		if(new_port < 1024)
			std::cout << "Warning: Using privledged port" << std::endl;
	#endif
		port = new_port; 
	}

	//: Do a DNS lookup on the IP address.
	std::string dns_lookup() const;

//! Implementation:
private:
	// IP address in network byte order.
	unsigned int ip[4];

	// IP port in host byte order.
	unsigned short port;
};

#endif
