/*
	$Id: slot_v2.h,v 1.9 2001/12/16 18:17:45 mbn Exp $

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

#ifndef header_slot_v2
#define header_slot_v2

#include "slot_generic.h"

template <class PARAM1, class PARAM2>
//: Slot V2 Class
class CL_Slot_v2 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2)=0;
};

template <class PARAM1, class PARAM2>
//: FunctionSlot V2 Class
class CL_FunctionSlot_v2 : public CL_Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2);

	//: FunctionSlot V2 Constructor
	CL_FunctionSlot_v2(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2) { callback(param1, param2); }

private:
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2>
//: Methane Slot V2 class
class CL_MethodSlot_v2 : public CL_Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2);

	//: Methane Slot V2 constructor
	CL_MethodSlot_v2(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2) { (cb_class->*callback)(param1, param2); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class UserData>
//: User Data Methane Slot V2 Class
class CL_UserDataMethodSlot_v2 : public CL_Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, UserData);

	//: User Data Methane Slot V2 Constructor
	CL_UserDataMethodSlot_v2(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2) { (cb_class->*callback)(param1, param2, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
