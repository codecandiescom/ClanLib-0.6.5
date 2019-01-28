/*
	$Id: font_generic.cpp,v 1.4 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "font_generic.h"
#include <API/Core/Resources/resource_manager.h>

CL_Font_Generic::CL_Font_Generic()
{
	ref_count = 0;
}

CL_Font_Generic::~CL_Font_Generic()
{
}

int CL_Font_Generic::add_reference()
{
	return ++ref_count;
}

int CL_Font_Generic::release_reference()
{
	return --ref_count;
}
