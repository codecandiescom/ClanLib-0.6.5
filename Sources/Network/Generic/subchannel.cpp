/*
	$Id: subchannel.cpp,v 1.4 2001/03/17 07:29:34 vaidhy Exp $

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

#include "API/Network/subchannel.h"
#include "subchannel_generic.h"

CL_SubChannel::CL_SubChannel(const CL_SubChannel &copy)
: impl(copy.impl)
{
	impl->add_ref();
}

CL_SubChannel::CL_SubChannel(int netchannel)
: impl(new CL_SubChannel_Generic(netchannel))
{
	impl->add_ref();
}

CL_SubChannel::~CL_SubChannel()
{
	if (impl != NULL && impl->release_ref() == 0) delete impl;
}

CL_Slot CL_SubChannel::connect(int subchannel, CL_Slot_v1<const class CL_NetMessage &> *slot)
{
	return impl->connect(subchannel, slot);
}

void CL_SubChannel::disconnect(int subchannel, CL_Slot &slot)
{
	impl->disconnect(subchannel, slot);
}
