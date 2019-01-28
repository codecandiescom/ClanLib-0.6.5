/*
	$Id: resourcetype.cpp,v 1.2 2001/04/10 14:24:30 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Core/Resources/resourcetype.h>

std::list<CL_ResourceType*> CL_ResourceType::resource_types;

CL_ResourceType::CL_ResourceType(const std::string &type)
: type(type)
{
	resource_types.push_back(this);
}

CL_ResourceType::~CL_ResourceType()
{
	resource_types.remove(this);
}
