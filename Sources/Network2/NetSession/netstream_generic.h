
#ifndef header_netstream_generic
#define header_netstream_generic

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include "API/Core/System/event_trigger.h"
#include "API/Core/System/mutex.h"
#include <queue>
#include <string>

class CL_NetComputer;
class CL_NetComputer_Generic;
class CL_NetSession_Generic;

class CL_NetStream_Generic
{
//! Construction:
public:
	//: Connect constructor.
	CL_NetStream_Generic(
		const std::string &netstream,
		CL_NetComputer &dest,
		CL_NetSession_Generic *session);

	//: Received constructor.
	CL_NetStream_Generic(
		int channel_id,
		CL_NetComputer_Generic *dest,
		CL_NetSession_Generic *session);

	~CL_NetStream_Generic();

//! Attributes:
public:
	//: Flag to be raised by netsession when all data has been written to a channel.
	CL_EventTrigger write_trigger;

	//: Flag to be raised by netsession when there is data to be read from the channel.
	CL_EventTrigger read_trigger;

	//: Receive buffer queue:
	std::queue<std::string> receive_queue;

	//: Syncronized access to member variables.
	CL_Mutex mutex;

	//: NetSession that stream is connected to.
	CL_NetSession_Generic *session;

	//: Computer the stream is communicating with.
	CL_NetComputer_Generic *computer;

	//: Stream channel identifier.
	int channel_id;
	
	//: True if connection was closed.
	bool closed;

	//: Returns the reference count of the netstream.
	int get_ref_count() const { return ref_count; }

//! Operations:
public:
	//: Increases the reference count.
	void add_ref();

	//: Decreases the reference count. If the count reaches zero, deletes this.
	int release_ref();

//! Implementation:
private:
	//: Reference counter.
	int ref_count;
};

#endif
