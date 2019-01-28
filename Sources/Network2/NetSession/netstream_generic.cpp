
#include "netstream_generic.h"
#include "netsession_generic.h"
#include "netcomputer_generic.h"
#include "API/Network2/NetSession/netcomputer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic construction:

CL_NetStream_Generic::CL_NetStream_Generic(
	const std::string &netstream,
	CL_NetComputer &dest,
	CL_NetSession_Generic *session)
: session(session), computer(dest.impl), channel_id(0), closed(false), ref_count(0)
{
	CL_MutexSection mutex_section(&computer->mutex);
	computer->add_ref();

	channel_id = computer->send_stream_connect(netstream, this);
}

CL_NetStream_Generic::CL_NetStream_Generic(
	int channel_id,
	CL_NetComputer_Generic *dest,
	CL_NetSession_Generic *session)
: session(session), computer(dest), channel_id(channel_id), closed(false), ref_count(0)
{
	CL_MutexSection mutex_section(&computer->mutex);
	computer->add_ref();
}

CL_NetStream_Generic::~CL_NetStream_Generic()
{
	if (!closed) computer->send_stream_close(channel_id);

	CL_MutexSection mutex_section(&computer->mutex);
	computer->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic operations:

void CL_NetStream_Generic::add_ref()
{
	CL_MutexSection mutex_section(&mutex);
	ref_count++;
}

int CL_NetStream_Generic::release_ref()
{
	CL_MutexSection mutex_section(&mutex);
	ref_count--;
	if (ref_count == 0)
	{
		mutex_section.leave();
		delete this;
		return 0;
	}
	return ref_count;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic implementation:
