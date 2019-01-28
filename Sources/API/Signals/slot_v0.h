/*
	$Id: slot_v0.h,v 1.11 2001/12/16 18:17:45 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSignal="System"
//! header=signals.h

#ifndef header_slot_v0
#define header_slot_v0

#include "slot_generic.h"
#include <list>

//: Slot V0 Class
class CL_Slot_v0 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(class CL_SlotParent_v0 &super)=0;
};

//: Slot Parent V0 Class
class CL_SlotParent_v0
{
public:
	typedef CL_Slot_v0 *Slot;
	typedef std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	
	//: Slot Parent V0 Constructor
	CL_SlotParent_v0(const SlotIterator &pos, const SlotIterator &end)
	: pos(pos), end(end)
	{
	}

//! Operations:
public:
	//: Operator
	void operator() ()
	{
		if (pos == end) return;

		SlotIterator next = pos;
		next++;

		CL_SlotParent_v0 super(next, end);
		(*pos)->call(super);
	}

//! Implementation:
private:
	SlotIterator pos, end;
};

//: FunctionSlot V0 Class
class CL_FunctionSlot_v0 : public CL_Slot_v0
{
public:
	typedef void (*Callback)();
	typedef void (*VirtualCallback)(CL_SlotParent_v0 &);

	//: FunctionSlot V0 Constructor
	CL_FunctionSlot_v0(Callback callback, VirtualCallback virtual_callback)
	: callback(callback), virtual_callback(virtual_callback) { return; }

	//: Call
	virtual void call(CL_SlotParent_v0 &super)
	{
		if (callback) { super(); callback(); }
		else virtual_callback(super);
	}

private:
	Callback callback;
	VirtualCallback virtual_callback;
};

template <class CallbackClass>
//: Method Slot V0 Class
class CL_MethodSlot_v0 : public CL_Slot_v0
{
public:
	typedef void (CallbackClass::*Callback)();
	typedef void (CallbackClass::*VirtualCallback)(CL_SlotParent_v0 &);

	//: Method Slot V0 Constructor
	CL_MethodSlot_v0(CallbackClass *cb_class, Callback callback, VirtualCallback virtual_callback)
	: cb_class(cb_class), callback(callback), virtual_callback(virtual_callback) { return; }

	//: Call
	virtual void call(CL_SlotParent_v0 &super)
	{
		if (callback) { super(); (cb_class->*callback)(); }
		else (cb_class->*virtual_callback)(super);
	}

private:
	CallbackClass *cb_class;
	Callback callback;
	VirtualCallback virtual_callback;
};

template <class CallbackClass, class UserData>
//: User Data Methane Slot V0 Class
class CL_UserDataMethodSlot_v0 : public CL_Slot_v0
{
public:
	typedef void (CallbackClass::*Callback)(UserData);
	typedef void (CallbackClass::*VirtualCallback)(CL_SlotParent_v0 &, UserData);

	//: User Data Methane Slot V0 Constructor
	CL_UserDataMethodSlot_v0(CallbackClass *cb_class, Callback callback, VirtualCallback virtual_callback, UserData user_data)
	: cb_class(cb_class), callback(callback), virtual_callback(virtual_callback), user_data(user_data) { return; }

	//: Call
	virtual void call(CL_SlotParent_v0 &super)
	{
		if (callback) { super(); (cb_class->*callback)(user_data); }
		else (cb_class->*virtual_callback)(super, user_data);
	}

private:
	CallbackClass *cb_class;
	Callback callback;
	VirtualCallback virtual_callback;
	UserData user_data;
};

#endif
