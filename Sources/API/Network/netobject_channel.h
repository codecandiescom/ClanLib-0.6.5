/*
	$Id: netobject_channel.h,v 1.13 2001/10/10 11:40:52 sphair Exp $

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

#ifndef header_netobject_channel
#define header_netobject_channel

#include <string>
#include "../signals.h"

class CL_NetObjectChannel_Generic;

//: Network object replication channel.
//- <p>Manages a netchannel with netobject replication data. It dispatches incoming
//- messages to the correct netobject.</p>
class CL_NetObjectChannel
{
//! Construction:
public:
	//: Copy constructor.
	CL_NetObjectChannel(const CL_NetObjectChannel &copy);

	//: Constructs a netobject channel.
	CL_NetObjectChannel(class CL_NetSession *netgame, int channel);

	//: Net Object Channel Destructor
	virtual ~CL_NetObjectChannel();

//! Attributes:
public:
	//: CL_Signal_v3<netobject, msgtype, message>.
	//:
	//: <p>Signal that is called whenever the netobject channel receives a message for a netobject
	//: it doesnt know. The application should hook a slot into this signal and determine what
	//: to do with the message.</p>
	CL_Signal_v3<const class CL_NetObject &, int, const std::string &> &sig_create_object();

	//: <p>Sends a begin_sync message to the netchannel. This is used together with end_sync to
	//: mark a group of objects. The client can use wait_sync to wait for a full transfer of
	//: a list of objects.</p>
	void begin_sync(const class CL_NetGroup *group = NULL);

	//: <p>Sends a end_sync message to the netchannel. This is used together with end_sync to
	//: mark a group of objects. The client can use wait_sync to wait for a full transfer of
	//: a list of objects.</p>
	void end_sync(const class CL_NetGroup *group = NULL);

	//: <p>Waits for a synced group of objects to arrive. If the whole group havnt been received
	//: before the timeout, the function will return false.</p>
	bool wait_sync(int timeout = -1);

public:
	//: Net Object Channel Generic Implementation
	CL_NetObjectChannel_Generic *impl;
};

#endif
