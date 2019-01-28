/*
	$Id: subchannel_generic.cpp,v 1.3 2001/03/15 12:14:47 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "Core/precomp.h"
#include "subchannel_generic.h"

CL_SubChannel_Generic::CL_SubChannel_Generic(int _netchannel)
: ref(0), netchannel(_netchannel)
{
}

CL_SubChannel_Generic::~CL_SubChannel_Generic()
{
}

int CL_SubChannel_Generic::add_ref()
{
	return ++ref;
}

int CL_SubChannel_Generic::release_ref()
{
	return --ref;
}

CL_Slot CL_SubChannel_Generic::connect(int subchannel, CL_Slot_v1<const class CL_NetMessage &> *slot)
{
	return subchannels[subchannel].connect(slot);
}

void CL_SubChannel_Generic::disconnect(int subchannel, CL_Slot &slot)
{
	subchannels[subchannel].disconnect(slot);
}
