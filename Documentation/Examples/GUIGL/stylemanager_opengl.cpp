/*
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <ClanLib/Core/Resources/resource_manager.h>
#include <ClanLib/GUI/button.h>
#include <ClanLib/GUI/window.h>

#include "stylemanager_opengl.h"
#include "button_opengl.h"
#include "window_opengl.h"

// Construction:

CL_StyleManager_OpenGL::CL_StyleManager_OpenGL(CL_ResourceManager *resources)
: CL_StyleManager_Default(resources)
{
}

CL_StyleManager_OpenGL::~CL_StyleManager_OpenGL()
{
}

// Attributes:

// Overridables:

void CL_StyleManager_OpenGL::connect_styles(
	const std::string &type,
	CL_Component *owner)
{
	if (type == "button")
	{
		CL_Button *button = (CL_Button *) owner;
		button->attach_style(new CL_Button_OpenGL(button, this));
	}
	else if (type == "window")
	{
		CL_Window *window = (CL_Window *) owner;
		window->attach_style(new CL_Window_OpenGL(window, this));
	}
	else
	{
		CL_StyleManager_Default::connect_styles(type, owner);
	}
}
