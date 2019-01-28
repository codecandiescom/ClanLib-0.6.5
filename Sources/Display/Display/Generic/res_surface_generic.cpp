/*
	$Id: res_surface_generic.cpp,v 1.3 2001/04/25 15:15:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Display/Display/res_surface.h>
#include "resource_surface_generic.h"

CL_ResourceSource_Surface::CL_ResourceSource_Surface()
{
	surface_sources.push_back(this);
}

CL_ResourceSource_Surface::~CL_ResourceSource_Surface()
{
	surface_sources.remove(this);
}
