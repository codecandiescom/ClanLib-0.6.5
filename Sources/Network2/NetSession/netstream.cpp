
#include "API/Network2/NetSession/netstream.h"
#include "API/Network2/NetSession/netsession.h"
#include "netstream_generic.h"
#include "netsession_generic.h"
#include "netcomputer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream construction:

CL_NetStream::CL_NetStream(
	const std::string &netstream,
	CL_NetComputer &dest)
: impl(new CL_NetStream_Generic(netstream, dest, dest.impl->netsession))
{
	input.netstream = impl;
	output.netstream = impl;
	if (impl) impl->add_ref();
}

CL_NetStream::CL_NetStream(const CL_NetStream &copy)
: impl(copy.impl)
{
	input.netstream = impl;
	output.netstream = impl;
	if (impl) impl->add_ref();
}

CL_NetStream::~CL_NetStream()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream attributes:

CL_EventTrigger *CL_NetStream::get_read_trigger()
{
	return &impl->read_trigger;
}

CL_EventTrigger *CL_NetStream::get_write_trigger()
{
	return &impl->write_trigger;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream operations:

CL_NetStream &CL_NetStream::operator =(const CL_NetStream &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();

	input.netstream = impl;
	output.netstream = impl;

	return *this;
}

int CL_NetStream::send(const void *data, int size)
{
	if (impl->closed) return 0;
	impl->computer->send_stream_message(impl->channel_id, std::string((const char *) data, size));
	return size;
}

#define cl_min(a, b) ((a < b) ? a : b)

int CL_NetStream::recv(void *data, int size)
{
	char *_data = (char *) data;
	int received = 0;
	while (true)
	{
		impl->read_trigger.wait();
		
		CL_MutexSection mutex_section(&impl->mutex);
		while (!impl->receive_queue.empty())
		{
			std::string &chunk = impl->receive_queue.front();
			int copy_size = cl_min(size-received, chunk.length());

			memcpy(_data + received, chunk.data(), copy_size);
			received += copy_size;

			if (copy_size == chunk.length()) impl->receive_queue.pop();
			else chunk = chunk.substr(copy_size);

			if (received == size) return received;
		}
		if (impl->closed) return received;
		impl->read_trigger.reset();
	}

	return received;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream implementation:

CL_NetStream::CL_NetStream(CL_NetStream_Generic *impl)
: impl(impl)
{
	input.netstream = impl;
	output.netstream = impl;
	if (impl) impl->add_ref();
}

