/*
	$Id: netobject_server.h,v 1.1 2002/11/02 19:46:00 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="NetObjects"
//! header=network.h

#ifndef header_netobject_server
#define header_netobject_server

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <string>
#include "../../signals.h"

class CL_NetObject_Server_Generic;
class CL_NetPacket;
class CL_NetObject_Controller;
class CL_NetComputer;
class CL_NetGroup;

//: Network replication object, server version.
//- <p>The netobject is a message dispatcher for objects being replicated over the network.</p>
//- <p>It provides an easy system for a server object to send messages to replicated clients
//- on remote computers, as well as getting answers back.</p>
class CL_NetObject_Server
{
//! Construction:
public:
	//: Construct a new netobject, assigning it an unique handle that identifies it over the
	//: network.
	CL_NetObject_Server(CL_NetObject_Controller *controller);

	//: Copy constructor.
	CL_NetObject_Server(const CL_NetObject_Server &copy);

	//: NetObject destructor.
	virtual ~CL_NetObject_Server();

//! Attributes:
public:
	//: Returns the netobject handle that identifies it over the network.
	int get_obj_id() const;

//! Operations:
public:
	//: Send a message to client objects.
	void send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message);

	//: Send a message to client object.
	void send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message);

//! Signals:
public:
	//: Signal emitted when receiving messages of the specified type from a client.
	CL_Signal_v2<const CL_NetComputer &, CL_NetPacket &> &sig_received_message(int msg_type);

//! Implementation:
private:
	CL_NetObject_Server_Generic *impl;
};

#endif
