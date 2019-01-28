/*
	$Id: setupgl_glx.cpp,v 1.5 2002/01/22 10:14:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/GL/setupgl.h"
#include "Display/System/Unix/setupdisplay_unix.h"
#include "glx_target.h"
#include <vector>

// defined in clanDisplay. It is the list of display targets that we can use:
extern std::vector<CL_DisplayTarget*> targets;

static CL_Force_DispTarget *force = 0;
void CL_SetupGL::init(bool register_resources_only)
{
	if (register_resources_only) return;

	force = new CL_Force_DispTarget("glx");
	targets.push_back(new CL_GLXTarget);
}

void CL_SetupGL::deinit()
{
	if (force) delete force;
	force = 0;
}
