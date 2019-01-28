/*
	$Id: setupnetwork_win32.cpp,v 1.2 2002/11/02 19:57:05 mbn Exp $
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

#include "API/Network2/setupnetwork.h"
#include "API/Core/System/cl_assert.h"
#include <winsock2.h>

static ref_count = 0;

/////////////////////////////////////////////////////////////////////////////
// Initialize network.

void CL_SetupNetwork::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	if(register_resources_only == false)
	{
		WORD winsock_version = MAKEWORD( 2, 0 ); 
		WSADATA wsaData;
		int err = WSAStartup(winsock_version, &wsaData);

		// Failed to initialize winsockets. You got some weird ancient windows 95 or something?
		cl_assert(err == 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Deinitialize network.

void CL_SetupNetwork::deinit()
{
	ref_count--;
	if (ref_count > 0) return;
}
