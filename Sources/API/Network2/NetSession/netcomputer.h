/*
	$Id: netcomputer.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="NetSessions"
//! header=network.h

#ifndef header_netcomputer
#define header_netcomputer

#include "../Socket/ip_address.h"
#include "netsession.h"

class CL_NetComputer_Generic;

//: Class representing a computer in a network game.
class CL_NetComputer
{
//! Construction:
public:
	//: Creates a netcomputer object.
	CL_NetComputer();

	CL_NetComputer(const CL_NetComputer &copy);

	//: Net Computer Destructor.
	virtual ~CL_NetComputer();

//! Attributes:
public:
	//: Returns the IP address (in network byte order) of the computer.
	CL_IPAddress get_address() const;

	//: Returns the netsession that the computer is attached to.
	CL_NetSession get_session();

	//: Returns true if computer is disconnected.
	bool is_disconnected() const;

	//: Returns the disconenct reason.
	const std::string &get_disconnect_reason() const;

	//: Returns true if objects are the same.
	bool operator == (const CL_NetComputer &other_instance) const;

	//: Returns true if the other netcomputer is less.
	//- <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>
	bool operator < (const CL_NetComputer &other_instance) const;

	//: Returns true if the other netcomputer is greater.
	//- <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>
	bool operator > (const CL_NetComputer &other_instance) const;

//! Operations:
public:
	//: Disconnects the computer from server.
	void disconnect();

	//: Send packet to the specified channel.
	void send(const std::string &packet_channel, const CL_NetPacket &packet);

	//: Copy Operator.
	CL_NetComputer &operator = (const CL_NetComputer &other_instance);

//! Implementation:
public:
	CL_NetComputer(class CL_NetComputer_Generic *impl);

	CL_NetComputer_Generic *impl;
};

#endif
