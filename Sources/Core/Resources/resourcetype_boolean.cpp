/*
	$Id: resourcetype_boolean.cpp,v 1.2 2001/04/10 14:24:30 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Boolean resource type

*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcetype_boolean.h"
#include "API/Core/Resources/resource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Boolean construction:

CL_ResourceData_Boolean::CL_ResourceData_Boolean(CL_Resource &resource)
: CL_ResourceData(resource)
{
	const std::string &location = resource.get_location();

	if (location == "true" || location == "1")
	{
		value = true;
	}
	else if (location == "false" || location == "0")
	{
		value = false;
	}
	else
	{
		throw CL_Error(std::string("Boolean resource has invalid value: ") + location);
	}

	resource.attach_data("boolean", this);
}
