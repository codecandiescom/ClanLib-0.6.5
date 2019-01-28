/*
	$Id: subchannel.h,v 1.13 2001/10/10 11:40:52 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanNetwork="NetObjects"
//! header=network.h

#ifndef header_subchannel
#define header_subchannel

class CL_SubChannel_Generic;

#include "../signals.h"

//: Netchannel dispatcher.
//- Dispatches incoming messages on a netchannel to slots connected to the subchannel.
class CL_SubChannel
{
public:
//! Construction:
	//: Sub Channel Constructor
	CL_SubChannel(const CL_SubChannel &copy);

	//: Sub Channel Constructor
	CL_SubChannel(int netchannel);

	//: Sub Channel Destructor
	virtual ~CL_SubChannel();

//! Operations:
	//: Connect
	CL_Slot connect(int subchannel, CL_Slot_v1<const class CL_NetMessage &> *slot);

	//: DisConnect
	void disconnect(int subchannel, CL_Slot &slot);

//! Implementation:
public:
	//: Sub Channel Generic Implementation
	CL_SubChannel_Generic *impl;
};

#endif
