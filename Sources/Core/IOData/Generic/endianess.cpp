/*
	$Id: endianess.cpp,v 1.7 2001/09/15 14:54:50 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/IOData/cl_endian.h"

void CL_Endian::swap(void *data, int type_size, int total_times)
{
	if (type_size == 1) return;

	unsigned char *d = (unsigned char *) data;

	for (int j=0; j<total_times; j++)
	{
		for (int i=0; i<type_size/2; i++)
		{
			unsigned char a = d[i];
			d[i] = d[type_size-1-i];
			d[type_size-1-i] = a;
		}
		
		d += type_size;
	}
}

bool CL_Endian::is_system_big()
{
#ifdef USE_BIG_ENDIAN
	return true;
#else
	return false;
#endif
}

bool CL_Endian::is_system_64bit()
{
	if (sizeof(int*) == 8) return true;
	return false;
}
