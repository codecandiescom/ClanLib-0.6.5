/*
	$Id: netcomputer.h,v 1.15 2001/11/12 20:32:31 mbn Exp $

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

#include "netsession.h"

class CL_NetComputer_Generic;

//: Class representing a computer in a network game.
class CL_NetComputer
{
//! Construction:
public:
	//: Creates an unattached netcomputer object.
	CL_NetComputer();

	//: Creates an attached netcomputer object.
	CL_NetComputer(const CL_NetComputer &copy);

	//: Net Computer Destructor
	virtual ~CL_NetComputer();

//! Attributes:
public:
	//: Returns the IP address (in network byte order) of the computer.
	unsigned long get_address() const;

	//: Returns the netsession that the computer is attached to.
	CL_NetSession get_session();

	//: Returns true if objects are the same.
	bool operator == (const CL_NetComputer &other_instance) const;

	//: Returns true if objects are not the same.
	bool operator != (const CL_NetComputer &other_instance) const;

	//: <p>Returns true if the other netcomputer is less. This is used for sorting
	//: purposes (eg. if you use a std::map<CL_NetComputer, Player>), and sorts
	//: the netcomputers based on lowest IP number address.</p>
	bool operator < (const CL_NetComputer &other_instance) const;

	//: <p>Returns true if the other netcomputer is greater. This is used for sorting
	//: purposes (eg. if you use a std::map<CL_NetComputer, Player>), and sorts
	//: the netcomputers based on lowest IP number address.</p>
	bool operator > (const CL_NetComputer &other_instance) const;

//! Operations:
public:
	//: Disconnects the computer from server.
	void disconnect();

	//: Copy Operator
	CL_NetComputer &operator = (const CL_NetComputer &other_instance);

//! Implementation:
public:
	CL_NetComputer(class CL_NetComputer_Generic *impl);
	CL_NetComputer_Generic *impl;
};

#endif
