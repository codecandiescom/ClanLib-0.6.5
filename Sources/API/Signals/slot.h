/*
	$Id: slot.h,v 1.15 2001/09/22 15:52:17 plasmoid Exp $

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

#ifndef header_slot
#define header_slot

#include "slot_generic.h"
#include <stdlib.h>

//: Slot class
class CL_Slot
{
//! Construction:
public:
	//: Create an unconnected slot.
	CL_Slot()
	: impl(NULL)
	{
	}

	//: Copy a slot.
	CL_Slot(const CL_Slot &copy)
	: impl(copy.impl)
	{
		if (impl != NULL) impl->add_slot_ref();
	}

	//: Slot destructor
	~CL_Slot()
	{
		if (impl != NULL) impl->release_slot_ref();
	}

//! Operators:
	//: Slot copy operator
	void operator =(const CL_Slot &copy)
	{
		if (impl) impl->release_slot_ref();
		impl = copy.impl;
		if (impl) impl->add_slot_ref();
	}

//! Implementation:
public:
	//: Slot constructor
	CL_Slot(CL_Slot_Generic *impl)
	: impl(impl)
	{
		if (impl != NULL) impl->add_slot_ref();
	}

	//: Slot Generic Implementation
	CL_Slot_Generic *impl;
};

#endif
