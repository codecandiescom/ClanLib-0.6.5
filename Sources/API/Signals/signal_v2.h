/*
	$Id: signal_v2.h,v 1.11 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_signal_v2
#define header_signal_v2

#ifdef WIN32
#pragma warning ( disable : 4786 )
#endif

#include "slot.h"
#include "slot_v2.h"
#include <list>

template <class PARAM1, class PARAM2>
//: Signal V2 Class
class CL_Signal_v2
{
public:
	typedef CL_Slot_v2<PARAM1, PARAM2> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

//! Construction:
public:
	//: Signal V2 Constructor
	CL_Signal_v2()
	{
	}

	//: Signal V2 Destructor
	~CL_Signal_v2()
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
	void operator() (PARAM1 param1, PARAM2 param2)
	{
		call(param1, param2);
	}

	//: Call
	void call(PARAM1 param1, PARAM2 param2)
	{
		std::list<SlotIterator> remove_slots;

		// call slots connected to signal:
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
			
			slot->call(param1, param2);
		}

		// remove all slots no longer connected to any CL_Slot.
		typename std::list<SlotIterator>::iterator remove_it;
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
		slots.push_back(slot);
		return CL_Slot(slot);
	}

	//: Connect callback function slot.
	CL_Slot connect(void (*callback)(PARAM1, PARAM2))
	{
		return connect(new CL_FunctionSlot_v2<PARAM1, PARAM2>(callback));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CL_Slot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2))
	{
		return connect(new CL_MethodSlot_v2<CallbackClass, PARAM1, PARAM2>(cb_class, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CL_Slot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2, UserData),
		UserData user_data)
	{
		return connect(new CL_UserDataMethodSlot_v2<CallbackClass, PARAM1, PARAM2, UserData>(cb_class, callback, user_data));
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
