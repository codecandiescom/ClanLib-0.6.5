/*
	$Id: netsession_generic.h,v 1.6 2001/05/01 17:11:40 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netsession_generic
#define header_netsession_generic

class CL_NetComputer;
class CL_NetGroup;
class CL_ConnectionProvider;

#include "API/Network/netmessage.h"
#include "API/Core/System/event_trigger.h"

class CL_NetSession_Generic
{
// Construction:
public:
	CL_NetSession_Generic(CL_ConnectionProvider *provider);

	virtual ~CL_NetSession_Generic();

// Attributes:
public:
	enum PacketTypes
	{
		Packet_Hello,
		Packet_Hello_ToYouToo,
		Packet_NetChannel_Message_ToClient,
		Packet_NetChannel_Message_ToServer,
		Packet_NetChannel_AccessChange
	};

	CL_ConnectionProvider *provider;

	virtual CL_NetComputer &get_server() =0;

	virtual CL_NetGroup &get_all() =0;

	virtual bool peek(int channel) const=0;

	virtual int access_status(int channel) const=0;

	virtual bool is_writable(int channel) const=0;

	virtual bool is_readable(int channel) const=0;

	virtual CL_EventTrigger *get_trigger() { return &trigger; }

// Operations:
public:
	void add_ref();

	void release_ref();

	virtual void keep_alive()=0;

	virtual CL_NetMessage receive(int channel, int timeout=-1)=0;

	virtual void send(
		const int dest_channel,
		const CL_NetGroup &dest,
		const CL_NetMessage &message,
		bool reliable = true)=0;

	virtual CL_NetComputer receive_computer_leave()=0;

	virtual CL_NetComputer receive_computer_join()=0;

	virtual CL_NetComputer receive_computer_rejoin()=0;

	virtual bool receive_session_closed()=0;

	// Client side only:
	virtual int receive_access_changed()=0;

	// Server side only:
	virtual void set_access(
		int channel,
		const CL_NetGroup &group,
		int access_rights)=0;

// Signals:
public:
	CL_Signal_v1<CL_NetMessage &> sig_receive_message(int netchannel);
	CL_Signal_v2<CL_NetMessage &, int> sig_receive_message_default;
	CL_Signal_v1<CL_NetComputer> sig_computer_leave;
	CL_Signal_v1<CL_NetComputer> sig_computer_join;
	CL_Signal_v1<CL_NetComputer> sig_computer_rejoin;
	CL_Signal_v1<int> sig_access_changed;
	CL_Signal_v0 sig_session_closed;

protected:
	CL_EventTrigger trigger;

// Implementation:
private:
	int ref_count;
};

#endif
