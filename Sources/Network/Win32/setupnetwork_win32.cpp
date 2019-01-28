/*
	$Id: setupnetwork_win32.cpp,v 1.5 2002/01/22 10:14:04 mbn Exp $
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "API/Network/setupnetwork.h"
#include "API/Core/System/cl_assert.h"
#include "connection_provider_win32.h"

static ref_count = 0;

/////////////////////////////////////////////////////////////////////////////
// Initialize network.

void CL_SetupNetwork::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	if (register_resources_only) return;

	WORD winsock_version = MAKEWORD( 2, 0 ); 
	WSADATA wsaData;
	int err = WSAStartup(winsock_version, &wsaData);

	// Failed to initialize winsockets. You got some weird ancient windows 95 or something?
	cl_assert(err == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Deinitialize network.

void CL_SetupNetwork::deinit()
{
	ref_count--;
	if (ref_count > 0) return;
}
