/*
	$Id: setupcore.cpp,v 1.19 2002/01/22 10:14:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <cstdlib>
#include "API/Core/Resources/resourcetype_boolean.h"
#include "API/Core/Resources/resourcetype_integer.h"
#include "API/Core/Resources/resourcetype_string.h"
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/Resources/resourcetype.h"
#include "API/Core/System/setupcore.h"

static CL_RegisterResourceType<CL_ResourceData_Boolean> *restype_boolean = NULL;
static CL_RegisterResourceType<CL_ResourceData_Integer> *restype_integer = NULL;
static CL_RegisterResourceType<CL_ResourceData_String> *restype_string = NULL;
static CL_RegisterResourceType<CL_ResourceData_Raw> *restype_raw = NULL;

void init_system(); // this function is implemented in init_PLATFORM.cpp
void deinit_system(); // this function is implemented in init_PLATFORM.cpp

static int ref_count = 0;

void CL_SetupCore::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	restype_boolean = new CL_RegisterResourceType<CL_ResourceData_Boolean>("boolean");
	restype_integer = new CL_RegisterResourceType<CL_ResourceData_Integer>("integer");
	restype_string = new CL_RegisterResourceType<CL_ResourceData_String>("string");
	restype_raw = new CL_RegisterResourceType<CL_ResourceData_Raw>("raw");

	if (register_resources_only) return;
	
	init_system();
}

void CL_SetupCore::deinit()
{
	// Since, CL_SetupCore::init() may be called more than once - Only call deinit at the final call to this function
	if (!ref_count) return;	// Do not allow ref_count to become negative (may occur if something calls deinit() without an init() )
	ref_count--;
	if (ref_count > 0) return;	// Wait until final call to deinit

	delete restype_boolean;
	delete restype_integer;
	delete restype_string;
	delete restype_raw;

	restype_boolean = NULL;
	restype_integer = NULL;
	restype_string = NULL;
	restype_raw = NULL;

	deinit_system();
}
