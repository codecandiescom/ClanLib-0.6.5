/*
	$Id: netpacket.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_netpacket
#define header_netpacket

#include "inputsource_netpacket.h"
#include "outputsource_netpacket.h"
#include <string>
#include <list>

class CL_NetComputer;
class CL_NetPacket_Generic;

//: Net packet class.
class CL_NetPacket
{
//! Construction:
public:
	//: Constructs a packet.
	//param data, size: Initial data for packet.
	CL_NetPacket();

	CL_NetPacket(const void *data, int size);

	CL_NetPacket(const std::string &data);

	CL_NetPacket(const CL_NetPacket &copy);

	// Destructor.
	~CL_NetPacket();

//! Attributes:
public:
	//: Returns a pointer to netpacket data.
	unsigned char *get_data();

	const unsigned char *get_data() const;

	//: Return size of netpacket data.
	int get_size() const;

	//: Input source interface for netpacket.
	CL_InputSource_NetPacket input;

	//: Output source interface for netpacket.
	CL_OutputSource_NetPacket output;

//! Operations:
public:
	//: Resize data. Does not destroy the old data in the packet.
	void resize(int new_size);

	// Copy assignment operator.
	CL_NetPacket &operator =(const CL_NetPacket &copy);

//! Implementation:
private:
	friend class CL_InputSource_NetPacket;
	friend class CL_OutputSource_NetPacket;
	friend class CL_NetComputer;

	CL_NetPacket_Generic *impl;
};

#endif
