/*
	$Id: subchannel_generic.h,v 1.2 2001/03/15 12:14:47 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_subchannel_generic
#define header_subchannel_generic

#include <map>
#include "../../API/signals.h"

class CL_NetMessage;

class CL_SubChannel_Generic
{
public:
	CL_SubChannel_Generic(int netchannel);
	~CL_SubChannel_Generic();

	int add_ref();
	int release_ref();

	CL_Slot connect(int subchannel, CL_Slot_v1<const CL_NetMessage &> *slot);
	void disconnect(int subchannel, CL_Slot &slot);

	int ref;
	int netchannel;
	std::map<int, CL_Signal_v1<const CL_NetMessage &> > subchannels;
};

#endif
