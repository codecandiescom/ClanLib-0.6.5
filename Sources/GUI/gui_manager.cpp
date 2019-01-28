/*
	$Id: gui_manager.cpp,v 1.17 2002/01/30 13:17:16 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <cstdio>
#include "API/Display/Display/display.h"
#include "API/GUI/gui_manager.h"
#include "gui_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Construction:

CL_GUIManager::CL_GUIManager(CL_StyleManager *style)
:
	CL_Component(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()), NULL, style)
{
	impl = new CL_GUIManager_Generic(this);
}

CL_GUIManager::CL_GUIManager(CL_Component *parent, CL_StyleManager *style)
:
	CL_Component(CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()), NULL, style)
{
	impl = new CL_GUIManager_Generic(this, parent);
}

CL_GUIManager::~CL_GUIManager()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Attributes:

CL_Component *CL_GUIManager::get_focus()
{
	return impl->get_focus();
}

CL_Component *CL_GUIManager::get_capture()
{
	return impl->get_capture();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager Operations:

void CL_GUIManager::set_focus(CL_Component *component)
{
	impl->set_focus(component);
}

void CL_GUIManager::run()
{
	impl->run();
}

void CL_GUIManager::show()
{
	impl->show();
}

void CL_GUIManager::quit()
{
	impl->quit();
}

void CL_GUIManager::enable_input()
{
	impl->enable_input();
}

void CL_GUIManager::disable_input()
{
	impl->disable_input();
}

void CL_GUIManager::gui_capture_mouse(CL_Component *component)
{
	impl->gui_capture_mouse(component);
}

void CL_GUIManager::gui_release_mouse()
{
	impl->gui_release_mouse();
}
