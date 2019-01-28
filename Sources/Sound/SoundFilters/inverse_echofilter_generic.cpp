/*
	$Id: inverse_echofilter_generic.cpp,v 1.2 2002/05/20 12:57:06 mbn Exp $

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

#include "inverse_echofilter_generic.h"
#include <memory>
#ifdef BORLAND
#include <memory.h>
#endif

CL_InverseEchoFilter::CL_InverseEchoFilter(int buffer_size)
{
	impl = new CL_InverseEchoFilter_Generic;
	impl->buffer_size = buffer_size;
	impl->pos = 0;

	impl->buffer = new int[buffer_size*2];
	memset(impl->buffer, 0, sizeof(int)*buffer_size*2);
}

CL_InverseEchoFilter::~CL_InverseEchoFilter()
{
	delete[] impl->buffer;
	delete impl;
}

void CL_InverseEchoFilter::filter(int *data, int size)
{
	int *buffer = impl->buffer;
	int &pos = impl->pos;
	int &buffer_size = impl->buffer_size;

	int delay = buffer_size / 4;
	for (int i=0; i<size*2; i++)
	{
		buffer[pos] = data[i];

		int res = 0;
		for (int j=0; j<4; j++)
		{
			int p = pos+delay*j;
			if (p > buffer_size) p -= buffer_size;

			res += buffer[p]*2 / (5-j);
		}

		data[i] = res;

		pos++;
		if (pos == buffer_size) pos = 0;
	}
}
