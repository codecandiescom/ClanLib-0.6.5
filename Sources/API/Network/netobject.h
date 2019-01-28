/*
	$Id: netobject.h,v 1.17 2001/12/16 19:18:06 mbn Exp $

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

#ifndef header_netobject
#define header_netobject

#include <string>
#include "../signals.h"

class CL_NetObject_Generic;

//: Network replication object.
//- The netobject is a message dispatcher for objects being replicated over the network.
//- <p>It provides an easy system for a server object to send messages to replicated clients
//- on remote computers, as well as getting answers back.</p>
class CL_NetObject
{
//! Construction:
public:
	//: Copy constructor.
	CL_NetObject(const CL_NetObject &copy);

	//: <p>Construct a new netobject, assigning it an unique handle that identifies it over the
	//: network. Use this constructor to create a server side netobject.</p>
	CL_NetObject(class CL_NetObjectChannel *obj_channel);

	//: <p>Construct a netobject, assigning it the object handle obj_id. This is normally used
	//: to construct a client side netobject.</p>
	CL_NetObject(int obj_id, class CL_NetObjectChannel *obj_channel);

	//: NetObject destructor
	virtual ~CL_NetObject();

//! Attributes:
public:
	//: Returns the netobject handle that identifies it over the network.
	int get_obj_id() const;

	//: Returns true if the netobject is a server side object.
	bool is_server() const;

//! Operations:
public:
	//: Connect the slot to the specified message sub-type.
	CL_Slot connect(int msg_type, CL_Slot_v1<class CL_InputSource &> *slot);

	//: Connect the slot to the specified message sub-type.
	CL_Slot connect(int msg_type, void (*callback)(class CL_InputSource &))
	{
		return connect(msg_type, new CL_FunctionSlot_v1<class CL_InputSource &>(callback));
	}

	//: Connect the slot to the specified message sub-type.
	template <class CallbackClass>
	CL_Slot connect(
		int msg_type,
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(class CL_InputSource &))
	{
		return connect(msg_type, new CL_MethodSlot_v1<CallbackClass, class CL_InputSource &>(cb_class, callback));
	}

	//: Connect the slot to the specified message sub-type.
	template <class CallbackClass, class UserData>
	CL_Slot connect(
		int msg_type,
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(class CL_InputSource &, UserData),
		UserData user_data)
	{
		return connect(msg_type, new CL_UserDataMethodSlot_v1<CallbackClass, class CL_InputSource &, UserData>(cb_class, callback, user_data));
	}

	//: Connect the slot to the specified talkback message sub-type.
	CL_Slot connect(int talkback_type, CL_Slot_v2<const class CL_NetComputer &, class CL_InputSource &> *slot);

	//: Connect callback function slot.
	CL_Slot connect(int talkback_type, void (*callback)(const class CL_NetComputer &, class CL_InputSource &))
	{
		return connect(talkback_type, new CL_FunctionSlot_v2<const class CL_NetComputer &, class CL_InputSource &>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CL_Slot connect(
		int talkback_type,
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(const class CL_NetComputer &, class CL_InputSource &))
	{
		return connect(talkback_type, new CL_MethodSlot_v2<CallbackClass, const class CL_NetComputer &, class CL_InputSource &>(cb_class, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CL_Slot connect(
		int talkback_type,
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(const class CL_NetComputer &, class CL_InputSource &, UserData),
		UserData user_data)
	{
		return connect(talkback_type, new CL_UserDataMethodSlot_v2<CallbackClass, const class CL_NetComputer &, class CL_InputSource &, UserData>(cb_class, callback, user_data));
	}

	//: Send a message to the client objects.
	void send(int msg_type, const std::string &message);

	//: Send a message to the server objects.
	void talkback(int talkback_type, const std::string &message);

//! Implementation:
private:
	CL_NetObject_Generic *impl;
};

#endif
