/*
	$Id: setupnetwork_unix.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $
	
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network2/setupnetwork.h"

static int ref_count = 0;

/////////////////////////////////////////////////////////////////////////////
// Initialize network.

void CL_SetupNetwork::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;
}

/////////////////////////////////////////////////////////////////////////////
// Deinitialize network.

void CL_SetupNetwork::deinit()
{
	ref_count--;
	if (ref_count > 0) return;
}
