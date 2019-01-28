/*
	$Id: slotbuffer_v1.h,v 1.10 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_slotbuffer_v1
#define header_slotbuffer_v1

#include <vector>
#include "slot.h"

template <class PARAM1> class CL_Signal_v1;

template <class PARAM1>
//: SlotBuffer V1 Class
class CL_SlotBuffer_v1
{
public:
	struct Params
	{
		PARAM1 p1;
	};

//! Construction:
public:
	//: Slot Buffer V1 Constructor
	CL_SlotBuffer_v1()
	{
	}

	//: Slot Buffer V1 Constructor
	CL_SlotBuffer_v1(CL_Signal_v1<PARAM1> &signal)
	{
		connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return !params.empty();
	}

	//: Size
	int size()
	{
		return params.size();
	}

	//: Operator
	Params &operator[](int index)
	{
		return params[index];
	}

//! Operations:
public:
	//: Connect
	void connect(CL_Signal_v1<PARAM1> &signal)
	{
		slot = signal.connect(this, &CL_SlotBuffer_v1::callback);
	}

	//: Disconnect
	void disconnect(CL_Signal_v1<PARAM1> &signal)
	{
		signal.disconnect(slot);
	}

//! Implementation:
private:
	void callback(PARAM1 p1)
	{
		Params p;
		p.p1 = p1;

		params.push_back(p);
	}

	std::vector<Params> params;
	CL_Slot slot;
};

#endif
