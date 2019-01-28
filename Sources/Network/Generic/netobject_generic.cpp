/*
	$Id: netobject_generic.cpp,v 1.5 2001/11/12 20:32:32 mbn Exp $

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
#include "netobject_generic.h"
#include "netobject_channel_generic.h"

CL_NetObject_Generic::CL_NetObject_Generic(class CL_NetObjectChannel_Generic *_obj_channel)
: obj_channel(_obj_channel), obj_id(_obj_channel->id_counter++), server_side(true), ref(0)
{
	obj_channel->objects[obj_id] = this;
}

CL_NetObject_Generic::CL_NetObject_Generic(int _obj_id, class CL_NetObjectChannel_Generic *_obj_channel)
: obj_channel(_obj_channel), obj_id(_obj_id), server_side(false), ref(0)
{
	obj_channel->objects[obj_id] = this;
}

CL_NetObject_Generic::~CL_NetObject_Generic()
{
	obj_channel->objects.erase(obj_channel->objects.find(obj_id));
}

CL_Slot CL_NetObject_Generic::connect(int msg_type, CL_Slot_v1<CL_InputSource &> *slot)
{
	return msg_signals[msg_type].connect(slot);
}

CL_Slot CL_NetObject_Generic::connect(int talkback_type, CL_Slot_v2<const class CL_NetComputer &, class CL_InputSource &> *slot)
{
	return talkback_signals[talkback_type].connect(slot);
}

void CL_NetObject_Generic::send(int msg_type, const std::string &message)
{
	obj_channel->send(obj_id, msg_type, message);
}

void CL_NetObject_Generic::talkback(int talkback_type, const std::string &message)
{
	obj_channel->talkback(obj_id, talkback_type, message);
}

int CL_NetObject_Generic::add_ref()
{
	return ++ref;
}

int CL_NetObject_Generic::release_ref()
{
	return --ref;
}
