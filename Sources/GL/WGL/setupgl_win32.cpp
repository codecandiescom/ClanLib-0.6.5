/*
	$Id: setupgl_win32.cpp,v 1.4 2002/01/22 10:14:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <windows.h>
#include "Core/System/Win32/init_win32.h"
#include "API/GL/setupgl.h"
#include "displaycard_opengl_win32.h"
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/display.h"

class CL_OpenGL_Initializer : public CL_Win32EventListener
{
public:
	CL_OpenGL_Initializer();
	~CL_OpenGL_Initializer();
	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// ------------------------------------------------------------------------

CL_OpenGL_Initializer *opengl_initializer = NULL;

void CL_SetupGL::init(bool register_resources_only)
{
	if (register_resources_only) return;

	if (CL_Display::get_current_card() != 0)
	{
		cl_info(info_display, "You MUST call CL_SetupGL::init() before CL_SetupDisplay::init().");
		cl_assert(false);
	}
	opengl_initializer = new CL_OpenGL_Initializer;
}

void CL_SetupGL::deinit()
{
	if (opengl_initializer) delete opengl_initializer;
	opengl_initializer = NULL;
}

// ------------------------------------------------------------------------

extern bool global_uses_opengl;
extern void (*opengl_init_function)();

CL_OpenGL_Initializer::CL_OpenGL_Initializer()
{
	global_uses_opengl = true;
	CL_System_Win32::add_listener(this);
	opengl_init_function = CL_DisplayCard_OpenGL_Win32::add_display;
}

CL_OpenGL_Initializer::~CL_OpenGL_Initializer()
{
	CL_System_Win32::remove_listener(this);
}

bool CL_OpenGL_Initializer::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(CL_DisplayCard_OpenGL_Win32::context);
		return true;
	
	}
	return false;
}
