/*
	$Id: netgroup.h,v 1.12 2001/10/10 11:40:52 sphair Exp $

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

class CL_NetGroup_Generic;

//: Class used to group computers in a network game.
class CL_NetGroup
{
//! Construction:
public:
	//: Construct an empty group list.
	CL_NetGroup();

	//: Construct a group with one computer in it.
	CL_NetGroup(const CL_NetComputer &computer);

	//: Net Group Destructor
	virtual ~CL_NetGroup();

//! Attributes:
public:
	//: List of computers in this group.
	std::list<CL_NetComputer> &get_computers() const;

//! Operations:
public:
	//: Add computer to group.
	void add(const CL_NetComputer &computer);

	//: Remove computer from group.
	void remove(const CL_NetComputer &computer);

	//: Disconnects all computers in group.
	void disconnect();

//! Implementation:
private:
	CL_NetGroup_Generic *impl;
};

#endif
