/*
	$Id: slot_v1.h,v 1.9 2001/12/16 18:17:45 mbn Exp $

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

#ifndef header_slot_v1
#define header_slot_v1

#include "slot_generic.h"

template <class PARAM1>
//: Slot V1 class
class CL_Slot_v1 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1)=0;
};

template <class PARAM1>
//: Funtion Slot V1 class
class CL_FunctionSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (*Callback)(PARAM1);

	//: Funtion Slot V1 constructor
	CL_FunctionSlot_v1(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1) { callback(param1); }

private:
	Callback callback;
};

template <class CallbackClass, class PARAM1>
//: Method Slot V1
class CL_MethodSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1);

	//: Method Slot V1 constructor
	CL_MethodSlot_v1(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

template <class CallbackClass, class PARAM1, class UserData>
//: User Data Method Slot V1 class
class CL_UserDataMethodSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, UserData);

	//: User Data Method Slot V1 constructor
	CL_UserDataMethodSlot_v1(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
