/*
	$Id: signal_v0.h,v 1.12 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_signal_v0
#define header_signal_v0

#ifdef WIN32
#pragma warning ( disable : 4786 )
#endif

#include "slot.h"
#include "slot_v0.h"
#include <list>

//: CL Signal v0 class
class CL_Signal_v0
{
public:
	typedef CL_Slot_v0 *Slot;
	typedef std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: CL Signal v0 constructor
	CL_Signal_v0()
	{
	}

	//: CL Signal v0 destructor
	~CL_Signal_v0()
	{
		for (SlotIterator slot_it = slots.begin(); slot_it != slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->release_signal_ref();
		}
	}

//! Operations:
public:
	//: Operator
	void operator() ()
	{
		call();
	}

	//: Call
	void call()
	{
		std::list<SlotIterator> remove_slots;

		// call slots connected to signal:
		CL_SlotParent_v0 super(slots.begin(), slots.end());
		super();

		// find slots to be removed:
		for (SlotIterator slot_it = slots.begin(); slot_it != slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			// skip slot if it has no more references left in application.
			// (make it pending for removal)
			if (slot->get_slot_ref() == 0)
			{
				remove_slots.push_back(slot_it);
				continue;
			}
		}

		// remove all slots no longer connected to any CL_Slot.
		std::list<SlotIterator>::iterator remove_it;
		for (remove_it = remove_slots.begin(); remove_it != remove_slots.end(); remove_it++)
		{
			Slot slot = **remove_it;
			slot->release_signal_ref();
			slots.erase(*remove_it);
		}
	}

	//: Connect slot.
	CL_Slot connect(Slot slot)
	{
		slot->add_signal_ref();
		slots.push_front(slot);
		return CL_Slot(slot);
	}

	//: Connect callback function slot.
	CL_Slot connect(void (*callback)())
	{
		return connect(new CL_FunctionSlot_v0(callback, 0));
	}

	//: Connect virtual callback function slot.
	CL_Slot connect_virtual(void (*virtual_callback)(CL_SlotParent_v0 &))
	{
		return connect(new CL_FunctionSlot_v0(0, virtual_callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CL_Slot connect(CallbackClass *cb_class, void (CallbackClass::*callback)())
	{
		return connect(new CL_MethodSlot_v0<CallbackClass>(cb_class, callback, 0));
	}

	//: Connect virtual member function slot.
	template <class CallbackClass>
	CL_Slot connect_virtual(CallbackClass *cb_class, void (CallbackClass::*virtual_callback)(CL_SlotParent_v0 &))
	{
		return connect(new CL_MethodSlot_v0<CallbackClass>(cb_class, 0, virtual_callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CL_Slot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(UserData),
		UserData user_data)
	{
		return connect(new CL_UserDataMethodSlot_v0<CallbackClass, UserData>(cb_class, callback, 0, user_data));
	}

	//: Create virtual member function with user data slot.
	template <class CallbackClass, class UserData>
	CL_Slot connect_virtual(
		CallbackClass *cb_class,
		void (CallbackClass::*virtual_callback)(CL_SlotParent_v0 &, UserData),
		UserData user_data)
	{
		return connect(new CL_UserDataMethodSlot_v0<CallbackClass, UserData>(cb_class, 0, virtual_callback, user_data));
	}

	//: Disconnect
	void disconnect(CL_Slot &disconnect_slot)
	{
		for (SlotIterator slot_it = slots.begin(); slot_it != slots.end();)
		{
			Slot slot = *slot_it;
			if (disconnect_slot.impl == slot)
			{
				slot->release_signal_ref();
				slot_it = slots.erase(slot_it);
			}
			else
				slot_it++;
		}
	}

//! Implementation:
private:
	std::list<Slot> slots;
};

#endif
