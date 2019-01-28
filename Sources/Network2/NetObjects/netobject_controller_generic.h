
#ifndef header_netobject_controller_generic
#define header_netobject_controller_generic

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include "API/Network2/NetSession/netcomputer.h"
#include "API/signals.h"
#include <string>
#include <map>
class CL_NetSession;
class CL_NetPacket;
class CL_NetObject_Client;
class CL_NetObject_Client_Generic;
class CL_NetObject_Server_Generic;

class CL_NetObject_Controller_Generic
{
//! Construction:
public:
	CL_NetObject_Controller_Generic(CL_NetSession *netsession, const std::string &channel);

	~CL_NetObject_Controller_Generic();

//! Attributes:
public:
	CL_Signal_v3<CL_NetObject_Client &, int, CL_NetPacket &> sig_create_object;

	std::map<int, CL_NetObject_Server_Generic *> server_objects;

	typedef std::pair<CL_NetComputer, int> ClientID;

	std::map<ClientID, CL_NetObject_Client_Generic *> client_objects;

	CL_NetSession *netsession;

	std::string channel;

	int obj_id_counter;

//! Operations:
public:
	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}

//! Implementation:
public:
	void on_received_netpacket(CL_NetPacket &packet, CL_NetComputer &from);

	CL_Slot slot_received_netpacket;

	int ref_count;
};

#endif
