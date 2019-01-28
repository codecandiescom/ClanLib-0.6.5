/*
	$Id: netgroup.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_netgroup
#define header_netgroup

#include <list>
#include "netcomputer.h"

//: Represents a group of computers.
class CL_NetGroup : public std::list<CL_NetComputer>
{
//! Construction:
public:
	//: Constructs a group.
	//param computer: Add this computer to group.
	//param group: Add this group of computers to the group.
	CL_NetGroup();

	CL_NetGroup(const CL_NetComputer &computer);

	CL_NetGroup(const std::list<CL_NetComputer> &group);

//! Operations:
public:
	//: Send packet to all the computers in the group.
	void send(const std::string &packet_channel, const CL_NetPacket &packet);

//! Implementation:
private:
};

#endif
