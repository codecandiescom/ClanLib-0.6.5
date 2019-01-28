/*
	$Id: setupsound_generic.cpp,v 1.4 2001/09/08 19:24:21 japj Exp $

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

#include <cstdlib>
#include "Sound/Sound/Generic/resourcetype_sample.h"
#include "API/Core/Resources/resourcetype.h"
#include "setupsound_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound_Generic initialization:

static CL_RegisterResourceType<CL_ResourceData_Sample> *restype_sample = NULL;

void CL_SetupSound_Generic::init()
{
	restype_sample = new CL_RegisterResourceType<CL_ResourceData_Sample>("sample");
}

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound_Generic deinitialization:

void CL_SetupSound_Generic::deinit()
{
	delete restype_sample;
	restype_sample = NULL;
}
