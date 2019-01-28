/*
	$Id: netsession.h,v 1.1 2002/11/02 19:46:01 mbn Exp $

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

#ifndef header_netsession
#define header_netsession

#include "../../signals.h"
#include <string>
class CL_NetStream;
class CL_NetPacket;
class CL_NetComputer;
class CL_NetSession_Generic;
class CL_IPAddress;
class CL_NetGroup;

//: Net session class.
class CL_NetSession
{
//! Construction:
public:
	//: Constructs a netsession.
	//param app_id: Application identifier. This must match any remote
	//param app_id: netsession connecting to/from this netsession.
	CL_NetSession(const std::string &app_id);

	CL_NetSession(const CL_NetSession &copy);

	virtual ~CL_NetSession();

//! Attributes:
public:
	//: Returns a list of all computers connected.
	CL_NetGroup &get_all();

//! Operations:
public:
	//: Start listening for connections.
	void start_listen(const std::string &listen_port);

	//: Stop listening for connections and disconnect everyone.
	void stop_listen();

	//: Connect to remote netsession.
	CL_NetComputer connect(const CL_IPAddress &address);

	//: Connect to remote netsession asyncronously.
	//- <p>The DNS lookup and connect is done in a worker thread, thus not
	//- blocking the calling thread. If the connection fails,
	//- CL_NetSession::sig_disconnected() is emitted.</p>
	CL_NetComputer connect_async(const std::string &hostname, const std::string &port);

	//: Enable or disable debug output.
	//- <p>This will output the netchannels each netpacket arrive in.</p>
	void show_debug(bool enable = true);

//! Signals:
public:
	//: Signal invoked when a computer connects to the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_connected();

	//: Signal invoked when a computer re-connects to the netsession server.
	//- <p>This happens when a CL_NetComputer instance has still not been destroyed
	//- since its last connect (allows you to recognize players dropping out, so
	//- they dont loose their score when they reconnect).</p>
	CL_Signal_v1<CL_NetComputer &> &sig_computer_reconnected();

	//: Signal invoked when a computer disconnects from the netsession server.
	CL_Signal_v1<CL_NetComputer &> &sig_computer_disconnected();

	//: Signal invoked when someone makes a connection to the specified
	//: netstream.
	CL_Signal_v1<CL_NetStream &> &sig_netstream_connect(const std::string &netstream);

	//: Signal invoked when someone sends a netpacket to the specified packet channel.
	CL_Signal_v2<CL_NetPacket &, CL_NetComputer &> &sig_netpacket_receive(const std::string &packet_channel);

//! Implementation:
public:
	CL_NetSession(CL_NetSession_Generic *impl);
private:
	CL_NetSession_Generic *impl;

	friend class CL_NetStream;
};

#endif
