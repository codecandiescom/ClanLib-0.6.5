/*
	$Id: setupttf.cpp,v 1.7 2002/01/22 10:14:05 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include <API/TTF/setupttf.h>
#include "Display/Font/resourcetype_ttf.h"

static CL_ResourceType_TTF *restype_ttf = NULL;

static int ref_count = 0;

void CL_SetupTTF::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	restype_ttf = new CL_ResourceType_TTF();

	if (register_resources_only) return;
}

void CL_SetupTTF::deinit()
{
	ref_count--;
	if (ref_count > 0) return;

	if (restype_ttf) delete restype_ttf;

	restype_ttf = NULL;
}
