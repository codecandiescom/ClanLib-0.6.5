/*
	$Id: netobject.cpp,v 1.5 2001/11/12 20:32:32 mbn Exp $

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

#include "API/Network/netobject.h"
#include "API/Network/netobject_channel.h"
#include "netobject_generic.h"
#include "netobject_channel_generic.h"

CL_NetObject::CL_NetObject(const CL_NetObject &copy)
{
	impl = copy.impl;
	impl->add_ref();
}

CL_NetObject::CL_NetObject(CL_NetObjectChannel *obj_channel)
: impl(new CL_NetObject_Generic(obj_channel->impl))
{
	impl->add_ref();
}

CL_NetObject::CL_NetObject(int obj_id, CL_NetObjectChannel *obj_channel)
: impl(new CL_NetObject_Generic(obj_id, obj_channel->impl))
{
	impl->add_ref();
}

CL_NetObject::~CL_NetObject()
{
	if (impl != NULL && impl->release_ref() == 0) delete impl;
}

CL_Slot CL_NetObject::connect(
	int msg_type,
	CL_Slot_v1<CL_InputSource &> *slot)
{
	return impl->connect(msg_type, slot);
}

CL_Slot CL_NetObject::connect(
	int talkback_type,
	CL_Slot_v2<const CL_NetComputer &, class CL_InputSource &> *slot)
{
	return impl->connect(talkback_type, slot);
}

void CL_NetObject::send(int msg_type, const std::string &message)
{
	impl->send(msg_type, message);
}

void CL_NetObject::talkback(int talkback_type, const std::string &message)
{
	impl->talkback(talkback_type, message);
}

int CL_NetObject::get_obj_id() const
{
	return impl->get_obj_id();
}

bool CL_NetObject::is_server() const
{
	return impl->server_side;
}
