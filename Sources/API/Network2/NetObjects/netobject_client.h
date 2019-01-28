/*
	$Id: netobject_client.h,v 1.1 2002/11/02 19:46:00 mbn Exp $

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

#ifndef header_netobject_client
#define header_netobject_client

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <string>
#include "../../signals.h"

class CL_NetObject_Client_Generic;
class CL_NetPacket;
class CL_NetObject_Controller_Generic;
class CL_NetComputer;

//: Network replication object, client version.
//- <p>The netobject is a message dispatcher for objects being replicated over the network.</p>
//- <p>It provides an easy system for a server object to send messages to replicated clients
//- on remote computers, as well as getting answers back.</p>
class CL_NetObject_Client
{
//! Construction:
public:
	//: NetObject client constructor.
	CL_NetObject_Client(int obj_id, const CL_NetComputer &server, CL_NetObject_Controller_Generic *controller);

	//: Copy constructor.
	CL_NetObject_Client(const CL_NetObject_Client &copy);

	//: NetObject destructor.
	virtual ~CL_NetObject_Client();

//! Attributes:
public:
	//: Returns the netobject handle that identifies it over the network.
	int get_obj_id() const;

	//: Returns the netcomputer handle to the server of this object.
	CL_NetComputer &get_server() const;

//! Operations:
public:
	//: Send a message to the server object.
	void send(int msg_type, const CL_NetPacket &message);

//! Signals:
public:
	//: Signal emitted when receiving messages of the specified type from the server.
	CL_Signal_v1<CL_NetPacket &> &sig_received_message(int msg_type);

//! Implementation:
private:
	CL_NetObject_Client_Generic *impl;
};

#endif
