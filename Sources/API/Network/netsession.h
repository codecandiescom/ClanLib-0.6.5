/*
	$Id: netsession.h,v 1.15 2001/10/10 11:40:52 sphair Exp $

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

#include <string>
#include "../signals.h"

class CL_NetSession_Generic;

#define ACCESS_CHANNEL_READ 1
#define ACCESS_CHANNEL_WRITE 2

class CL_NetComputer;
class CL_NetGroup;
class CL_NetMessage;
class CL_ServerInfo;
class CL_EventTrigger;

//: CL_NetSession is the interface to communcation with a network session/game.
class CL_NetSession
{
//! Construction:
public:
	//: <p>Create a netsession server on the specified port. Only netsession clients
	//: with same application ID will be able to connect to the server.</p>
	CL_NetSession(
		const std::string &app_id,
		int port);

	//: Create a netsession client and connect to a server.
	CL_NetSession(
		const std::string &app_id,
		const std::string &hostname,
		int port);

	//: <p>Connect to a server coming from a server info structure. Usually used together
	//: with master browse servers and LAN broadcast browsing.</p>
	CL_NetSession(
		const CL_ServerInfo &server);

	//: Copy constructor.
	CL_NetSession(const CL_NetSession &copy);

	//: Net Session destructor
	virtual ~CL_NetSession();

//! Attributes:
public:
	//: Event trigger that triggers when the netsession object changes (on incoming data).
	CL_EventTrigger *get_trigger();

	//: The server of the network game.
	CL_NetComputer &get_server();

	//: A group of all the computers in the netsession.
	CL_NetGroup &get_all();
	
	//: Returns true if a message is available on the specified channel.
	bool peek(int channel) const;

	//: Returns the access status flags on the channel.
	int access_status(int channel) const;

	//: Returns true if the ACCESS_CHANNEL_WRITE flag is set on channel.
	bool is_writable(int channel) const;

	//: Returns true if the ACCESS_CHANNEL_READ flag is set on channel.
	bool is_readable(int channel) const;

//! Operations:
public:
	//: <p>Makes a copy of the current session. Since sessions are reference counted,
	//: this just means you will have two handles for the same session.</p>
	void operator =(const CL_NetSession &session);

	//: <p>Receives a message from the specified channel. The call is blocking, so
	//: don't call it if you aren't completly sure it will arrive (unless you set
	//: a timeout).</p>
	//: Note, if you don't want to poll, use the signals below.
	//- channel - Channel to look for a message.
	//- timeout - Timeout, -1 means block forever.
	//- Returns - The message received from the specified channel.
	CL_NetMessage receive(int channel, int timeout = -1);

	//: Sends a message to a group of computers at netchannel 'dest_channel'.
	//- dest - The group of computers to receive the message.
	//- dest_channel - Netchannel the message is sent to.
	//- message - Data in the message sent. All the data from 'message' is duplicated before this function returns.
	//- reliable - Arrival reliability. True guarantees arrival (use TCP/Pony express), false doesn't (use UDP/US Mail).
	void send(
		const int dest_channel,
		const CL_NetGroup &dest,
		const CL_NetMessage &message,
		bool reliable = true);

	//: <p>Returns the first (if more) computer that has left the game, or NULL if
	//: none. The next time the function is called, it returns the next computer,
	//: etc.</p>
	//: <p>CL_NetComputer instances are reference counted. A computer wont be forgotten
	//: until all instances of it has been destroyed. If a computer rejoins the server
	//: while there still exist a netcomputer instance, it will be recognized and
	//: cause a 'rejoin' event instead of a normal join event.</p>
	//: Note, if you don't want to poll, use the signals below.
	CL_NetComputer receive_computer_leave();

	//: <p>Returns the first (if more) computer that is joining the game, or throws an exception
	//: if none joined. The next time the function is called, it returns the next computer,
	//: etc.</p>
	//: Note, if you don't want to poll, use the signals below.
	CL_NetComputer receive_computer_join();

	//: <p>Returns the first (if more) computer that is re-joining the game, or throws an exception
	//: if none joined. The next time the function is called, it returns the next computer,
	//: etc.</p>
	//: <p>A re-joining computer is simply a computer joining the game, that still
	//: havn't had its CL_NetComputer reference deleted. This provides a way to
	//: recognize a computer that was in the game.</p>
	//: Note, if you don't want to poll, use the signals below.
	CL_NetComputer receive_computer_rejoin();

	//: Returns true if the session was closed.
	//: Note, if you don't want to poll, use the signals below.
	bool receive_session_closed();

	//: <p>Returns the first netchannel that has had its access changed. Returns -1 when no
	//: more channels have had their access changed.</p>
	//: This function only works on a client netsession.
	//: Note, if you don't want to poll, use the signals below.
	int receive_access_changed();

	//: Sets the channel access flags on the channel for the computers specified.
	//: This function only has effect on a server netsession.
	void set_access(
		int channel,
		const CL_NetGroup &group,
		int access_rights = ACCESS_CHANNEL_READ | ACCESS_CHANNEL_WRITE);

//! Signals:
public:
	//: <p>This signal is emitted when a message is received, and which
	//: wasn't routed to the sig_receive_message signal.</p>
	//- Returns - CL_NetMessage - The message received from the specified channel.
	//- Returns - int - The channel the message was received in.
	CL_Signal_v2<CL_NetMessage &, int> &sig_receive_message_default();

	//: This signal is emitted when a computer has left the game.
	//: <p>CL_NetComputer instances are reference counted. A computer wont be forgotten
	//: until all instances of it has been destroyed. If a computer rejoins the server
	//: while there still exist a netcomputer instance, it will be recognized and
	//: cause a 'rejoin' event instead of a normal join event.</p>
	CL_Signal_v1<CL_NetComputer> &sig_computer_leave();

	//: This signal is emitted when a computer is joining the game.
	CL_Signal_v1<CL_NetComputer> &sig_computer_join();

	//: This signal is emitted when a computer is re-joining the game.
	//: <p>A re-joining computer is simply a computer joining the game, that still
	//: havn't had its CL_NetComputer reference deleted. This provides a way to
	//: recognize a computer that was in the game.</p>
	CL_Signal_v1<CL_NetComputer> &sig_computer_rejoin();

	//: This signal is emitted when a netchannel had its access changed.
	//: This function only works on a client netsession.
	CL_Signal_v1<int> &sig_access_changed();

	//: This signal is emitted when the session was closed.
	CL_Signal_v0 &sig_session_closed();

//! Implementation:
public:
	CL_NetSession(class CL_NetSession_Generic *impl);
	CL_NetSession_Generic *impl;
};

#endif
