/*
	$Id: netmessage.h,v 1.11 2001/09/22 15:52:17 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Network component header file.
*/

//! clanNetwork="NetSessions"
//! header=network.h

#ifndef header_netmessage
#define header_netmessage

#include <string>
#include "netcomputer.h"

//: CL_NetMessage is used to contain messages sent across the network.
class CL_NetMessage
{
public:
//! Construction:
	//: Creates a net message with no contents. (default constructor)
	CL_NetMessage()
	{
	}

	//: Copy constructor.
	CL_NetMessage(const CL_NetMessage &copy)
	{
		data = copy.data;
		from = copy.from;
	}

	//: <p>Constructs a Net message using the data given. Please notice that it	
	//: doesn't duplicate the data, but only points its data pointer to the 
	//: data given.</p>
	//- data - Pointer to the data to be contained by the message.
	//- size - Size of the data.
	CL_NetMessage(const void *data, int size)
	{
		this->data.append((char *) data, size);
	}

	//: NetMessage Constructor
	CL_NetMessage(const std::string &data)
	{
		this->data = data;
	}

//! Attributes:
	//: Packet data.
	std::string data;

	//: <p>Computer the message was sent from. You don't have to fill in this
	//: if you just want to send a message - it is only used when receiving from
	//: a netchannel.</p>
	CL_NetComputer from;
};

#endif
