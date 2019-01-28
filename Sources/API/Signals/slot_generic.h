/*
	$Id: slot_generic.h,v 1.8 2001/09/22 15:52:17 plasmoid Exp $

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

#ifndef header_slot_generic
#define header_slot_generic

//: Slot Generic Class
class CL_Slot_Generic
{
//! Construction:
public:
	//: Slot Generic Constructor
	CL_Slot_Generic() : slot_ref_count(0), signal_ref_count(0) { return; }

	//: Slot Generic Destructor
	virtual ~CL_Slot_Generic() { return; }

//! Attributes:
public:
	//: Get slot ref
	int get_slot_ref() const { return slot_ref_count; }

	//: Get signal ref
	int get_signal_ref() const { return signal_ref_count; }

//! Operations:
public:
	//: Add slot ref
	void add_slot_ref() { slot_ref_count++; }

	//: Release slot ref
	void release_slot_ref() { slot_ref_count--; check_delete(); }

	//: Add signal ref
	void add_signal_ref() { signal_ref_count++; }

	//: Release signal ref
	void release_signal_ref() { signal_ref_count--; check_delete(); }

//! Implementation:
private:
	void check_delete()
	{
		if (slot_ref_count == 0 && signal_ref_count == 0) delete this;
	}

	int slot_ref_count;

	int signal_ref_count;
};

#endif
