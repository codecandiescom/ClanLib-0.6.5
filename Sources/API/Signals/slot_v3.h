/*
	$Id: slot_v3.h,v 1.9 2001/12/16 18:17:45 mbn Exp $

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

#ifndef header_slot_v3
#define header_slot_v3

#include "slot_generic.h"

template <class PARAM1, class PARAM2, class PARAM3>
//: Slot V3 Class
class CL_Slot_v3 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3)=0;
};

template <class PARAM1, class PARAM2, class PARAM3>
//: FunctionSlot V3 Class
class CL_FunctionSlot_v3 : public CL_Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3);

	//: FunctionSlot V3 Constructor
	CL_FunctionSlot_v3(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { callback(param1, param2, param3); }

private:
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3>
//: Methane Slot V3 class
class CL_MethodSlot_v3 : public CL_Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3);

	//: Methane Slot V3 constructor
	CL_MethodSlot_v3(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (cb_class->*callback)(param1, param2, param3); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class UserData>
//: User Data Methane Slot V3 Class
class CL_UserDataMethodSlot_v3 : public CL_Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, UserData);

	//: User Data Methane Slot V3 Constructor
	CL_UserDataMethodSlot_v3(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (cb_class->*callback)(param1, param2, param3, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
