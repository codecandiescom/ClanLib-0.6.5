/*
	$Id: netobject_controller.h,v 1.1 2002/11/02 19:46:00 mbn Exp $

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

#ifndef header_netobject_controller
#define header_netobject_controller

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <string>
#include "../../signals.h"
#include "../NetSession/netcomputer.h"
#include "../NetSession/netpacket.h"

class CL_NetObject_Controller_Generic;
class CL_NetGroup;
class CL_NetObject_Client;

//: Network object replication channel.
//- <p>Manages a netchannel with netobject replication data. It dispatches incoming
//- messages to the correct netobject.</p>
class CL_NetObject_Controller
{
//! Construction:
public:
	//: Constructs a netobject controller.
	CL_NetObject_Controller(class CL_NetSession *netsession, const std::string &channel);

	//: Copy constructor.
	CL_NetObject_Controller(const CL_NetObject_Controller &copy);

	//: NetObject Controller destructor.
	virtual ~CL_NetObject_Controller();

//! Attributes:
public:

//! Operations:
public:
	// Copy assignment operator.
	CL_NetObject_Controller &operator =(const CL_NetObject_Controller &copy);

//! Signals:
public:
	//: Signal that is called whenever the netobject channel receives a message for a netobject
	//: it doesnt know. The application should hook a slot into this signal and determine what
	//: to do with the message.
	//- <p>CL_Signal_v3<netobject, msgtype, message>.</p>
	//- <p>Signal parameters:
	//- CL_NetObject &netobj_handle - Handle identifying the netobject created.
	//- int msg_type - Message type of the message (the msg_type param used to send it with netobj.send()).
	//- CL_NetPacket &message - The message itself.</p>
	CL_Signal_v3<CL_NetObject_Client &, int, CL_NetPacket &> &sig_create_object();

//! Implementation:
public:
	//: NetObjectController implementation.
	CL_NetObject_Controller_Generic *impl;
};

#endif
