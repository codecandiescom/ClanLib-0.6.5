/*
	$Id: generic_surfaceprovider.cpp,v 1.1 2001/03/06 15:09:22 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Generic surfaceprovider class implementing reference counting.

*/

#include "Core/precomp.h"
#include "API/Display/SurfaceProviders/generic_surfaceprovider.h"
#include "API/Core/System/cl_assert.h"

CL_SurfaceProvider_Generic::CL_SurfaceProvider_Generic()
{
	ref_count = 0;
}

void CL_SurfaceProvider_Generic::lock()
{
	ref_count++;
	if (ref_count == 1) perform_lock();
}

void CL_SurfaceProvider_Generic::unlock()
{
	cl_assert(ref_count > 0);

	ref_count--;
	if (ref_count == 0) perform_unlock();
}
