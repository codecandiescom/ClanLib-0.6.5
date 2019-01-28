/*
	$Id: slot_v5.h,v 1.9 2001/12/16 18:17:45 mbn Exp $

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

#ifndef header_slot_v5
#define header_slot_v5

#include "slot_generic.h"

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: Slot V5 Class
class CL_Slot_v5 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)=0;
};

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: FunctionSlot V5 Class
class CL_FunctionSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: FunctionSlot V5 Constructor
	CL_FunctionSlot_v5(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { callback(param1, param2, param3, param4, param5); }

private:
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
//: Methane Slot V5 class
class CL_MethodSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: Methane Slot V5 constructor
	CL_MethodSlot_v5(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class UserData>
//: User Data Methane Slot V5 Class
class CL_UserDataMethodSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData);

	//: User Data Methane Slot V5 Constructor
	CL_UserDataMethodSlot_v5(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
