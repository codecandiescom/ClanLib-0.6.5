/*
	$Id: netobject_channel.cpp,v 1.3 2001/03/15 12:14:47 mbn Exp $

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

#include "API/Network/netobject_channel.h"
#include "netobject_channel_generic.h"

CL_NetObjectChannel::CL_NetObjectChannel(const CL_NetObjectChannel &copy)
: impl(copy.impl)
{
	impl->add_ref();
}

CL_NetObjectChannel::CL_NetObjectChannel(class CL_NetSession *netgame, int channel)
: impl(NULL)
{
	impl = new CL_NetObjectChannel_Generic(netgame, channel, this);
	impl->add_ref();
}

CL_NetObjectChannel::~CL_NetObjectChannel()
{
	if (impl != NULL && impl->release_ref() == 0) delete impl;
}

void CL_NetObjectChannel::begin_sync(const CL_NetGroup *group)
{
	impl->begin_sync(group);
}

void CL_NetObjectChannel::end_sync(const CL_NetGroup *group)
{
	impl->end_sync(group);
}

bool CL_NetObjectChannel::wait_sync(int timeout)
{
	return impl->wait_sync(timeout);
}

CL_Signal_v3<const CL_NetObject &, int, const std::string &> &CL_NetObjectChannel::sig_create_object()
{
	return impl->sig_create_object;
}

