/*
	$Id: setupnetwork_unix.cpp,v 1.5 2002/01/22 10:14:04 mbn Exp $
	
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network/setupnetwork.h"
#include "API/Core/System/cl_assert.h"
#include "connection_provider_unix.h"

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
