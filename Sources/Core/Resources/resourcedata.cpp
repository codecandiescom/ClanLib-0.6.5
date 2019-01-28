/*
	$Id: resourcedata.cpp,v 1.2 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcedata.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Generic:

class CL_ResourceData_Generic
{
public:
	class CL_Resource_Generic *resource;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData construction:

CL_ResourceData::CL_ResourceData(CL_Resource &resource)
: impl(new CL_ResourceData_Generic)
{
	impl->resource = resource.impl;
}

CL_ResourceData::~CL_ResourceData()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData attributes:

CL_Resource CL_ResourceData::get_resource()
{
	return CL_Resource(impl->resource);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData implementation:
