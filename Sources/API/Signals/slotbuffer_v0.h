/*
	$Id: slotbuffer_v0.h,v 1.9 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_slotbuffer_v0
#define header_slotbuffer_v0

#include "signal_v0.h"

//: Slot Buffer V0 Class
class CL_SlotBuffer_v0
{
//! Construction:
public:
	//: Slot Buffer V0 Constructor
	CL_SlotBuffer_v0() : count(0)
	{
	}

	//: Slot Buffer V0 Constructor
	CL_SlotBuffer_v0(CL_Signal_v0 &signal) : count(0)
	{
		connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return count > 0;
	}

	//: Size
	int size()
	{
		return count;
	}

//! Operations:
public:
	//: Connect
	void connect(CL_Signal_v0 &signal)
	{
		slot = signal.connect(this, &CL_SlotBuffer_v0::callback);
	}

	//: Disconnect
	void disconnect(CL_Signal_v0 &signal)
	{
		signal.disconnect(slot);
	}

//! Implementation:
private:
	void callback()
	{
		count++;
	}

	CL_Slot slot;
	int count;
};

#endif
