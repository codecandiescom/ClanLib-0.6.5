/*
	$Id: resourcetype_string.cpp,v 1.2 2001/04/10 14:24:30 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		String resource type

*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcetype_string.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_String construction:

CL_ResourceData_String::CL_ResourceData_String(CL_Resource &resource)
: CL_ResourceData(resource)
{
	value = resource.get_location();

	resource.attach_data("string", this);
}
