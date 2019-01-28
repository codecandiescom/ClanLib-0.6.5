/*
	$Id: browse_master.h,v 1.14 2001/10/10 11:40:52 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="Browsing"
//! header=network.h

#ifndef header_browse_master
#define header_browse_master

class CL_IPAddress;
class CL_BrowseMaster_Generic;

class CL_BrowseMaster
//: Browse master.
//- <p>A browse master is a service that browse servers can register themselves to, and
//- browse clients can then connect to the master and get a list of servers.</p>
//-
//- Normally, this is used in games to provide a list of available servers.
{
//! Construction:
public:
	//: Construct a browse master.
	CL_BrowseMaster(const CL_IPAddress &bind_address);

	//: Browse Master Destructor
	virtual ~CL_BrowseMaster();

//! Attributes:
public:

//! Operations:
public:

//! Implementation:
private:
	CL_BrowseMaster_Generic *impl;
};

#endif
