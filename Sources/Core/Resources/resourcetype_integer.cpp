/*
	$Id: resourcetype_integer.cpp,v 1.2 2001/04/10 14:24:30 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Integer resource type

*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcetype_integer.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Integer construction:

CL_ResourceData_Integer::CL_ResourceData_Integer(CL_Resource &resource)
: CL_ResourceData(resource)
{
	value = CL_String(resource.get_location()).get_as_int();

	resource.attach_data("integer", this);
}
