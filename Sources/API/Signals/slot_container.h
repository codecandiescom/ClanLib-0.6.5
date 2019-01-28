//! clanSignal="System"
//! header=signals.h

#ifndef header_slot_container
#define header_slot_container

#include "slot.h"
#include "signal_v0.h"
#include "signal_v1.h"
#include "signal_v2.h"
#include "signal_v3.h"
#include "signal_v4.h"
#include "signal_v5.h"
#include <list>

//: Slot Generic Container
// Convience signal/slot connector class.
// The slot container class is a convience class. It simplifies the
// connection between signals and slots by keeping all connections made
// until the entire class is destroyed. This saves the developer from having
// to keep a CL_Slot reference around for each connection needed.
// In order to use this class, simply inherit from it and call
// connect_signal() for each function you want hooked up.
// See the login_view.cpp/h in the CTalk example for an example of how to use
// this class.
class CL_SlotContainer
{
public:
//! Operations:
	//: Connect a slot to a CL_Signal_v0 signal.
	template<class SigClass, class Class>
	void connect(SigClass &sig, Class *self, void(Class::*func)())
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v0 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v1 signal.
	template<class SigClass, class Class, class Param1>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v1 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v2 signal.
	template<class SigClass, class Class, class Param1, class Param2>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v2 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v3 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v3 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v4 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v4 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v5 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v5 signal with an user data parameter
	// passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

private:
	std::list<CL_Slot> slots;
};

#endif
