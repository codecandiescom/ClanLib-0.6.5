
#ifndef header_netobject_server_generic
#define header_netobject_server_generic

#include "API/signals.h"
#include <map>
class CL_NetComputer;
class CL_NetPacket;
class CL_NetGroup;
class CL_NetObject_Controller_Generic;

class CL_NetObject_Server_Generic
{
//! Construction:
public:
	CL_NetObject_Server_Generic(CL_NetObject_Controller_Generic *controller);

	~CL_NetObject_Server_Generic();

//! Attributes:
public:
	int obj_id;

	CL_NetObject_Controller_Generic *controller;

	std::map< int, CL_Signal_v2<const CL_NetComputer &, CL_NetPacket &> > sig_received_message;

//! Operations:
public:
	void receive(int msg_type, const CL_NetComputer &from, CL_NetPacket &packet);

	void send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message);
	void send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message);

	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}

//! Implementation:
private:
	int ref_count;
};

#endif
