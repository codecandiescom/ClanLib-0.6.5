/*
	$Id: echofilter_generic.cpp,v 1.1 2001/03/06 15:09:26 mbn Exp $

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

#include "echofilter_generic.h"
#include <memory.h>

CL_EchoFilter::CL_EchoFilter(int buffer_size, int shift_factor)
{
	impl = new CL_EchoFilter_Generic;
	impl->buffer_size = buffer_size;
	impl->shift_factor = shift_factor;
	impl->pos = 0;

	impl->buffer = new int[buffer_size*2];
	memset(impl->buffer, 0, sizeof(int)*buffer_size*2);
}

CL_EchoFilter::~CL_EchoFilter()
{
	delete[] impl->buffer;
	delete impl;
}

void CL_EchoFilter::filter(int *data, int size)
{
	int *buffer = impl->buffer;
	int &pos = impl->pos;
	int &buffer_size = impl->buffer_size;
	int &shift_factor = impl->shift_factor;

	for (int i=0; i<size*2; i++)
	{
		buffer[pos] /= shift_factor;
		buffer[pos] += data[i];
		data[i] = buffer[pos];
		pos++;
		if (pos == buffer_size) pos = 0;
	}
}
