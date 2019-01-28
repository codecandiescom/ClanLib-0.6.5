/*
	$Id: slot_v4.h,v 1.9 2001/12/16 18:17:45 mbn Exp $

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

#ifndef header_slot_v4
#define header_slot_v4

#include "slot_generic.h"

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: Slot V4 Class
class CL_Slot_v4 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)=0;
};

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: FunctionSlot V4 Class
class CL_FunctionSlot_v4 : public CL_Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	//: FunctionSlot V4 Constructor
	CL_FunctionSlot_v4(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { callback(param1, param2, param3, param4); }

private:
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
//: Methane Slot V4 class
class CL_MethodSlot_v4 : public CL_Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	//: Methane Slot V4 constructor
	CL_MethodSlot_v4(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (cb_class->*callback)(param1, param2, param3, param4); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class UserData>
//: User Data Methane Slot V4 Class
class CL_UserDataMethodSlot_v4 : public CL_Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, UserData);

	//: User Data Methane Slot V4 Constructor
	CL_UserDataMethodSlot_v4(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (cb_class->*callback)(param1, param2, param3, param4, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
