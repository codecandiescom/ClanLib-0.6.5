/*
	$Id: setupgui.cpp,v 1.31 2002/03/01 19:43:11 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/setupgui.h"
#include "API/GUI/component_manager.h"
#include "default_component_types.h"

void CL_SetupGUI::init(bool register_resources_only)
{
	if (register_resources_only) return;

	CL_ComponentManager::component_types["label"] = new CL_ComponentType_Label;
	CL_ComponentManager::component_types["button"] = new CL_ComponentType_Button;
	CL_ComponentManager::component_types["frame"] = new CL_ComponentType_Frame;
	CL_ComponentManager::component_types["inputbox"] = new CL_ComponentType_InputBox;
	CL_ComponentManager::component_types["image"] = new CL_ComponentType_Image;
	CL_ComponentManager::component_types["scrollbar"] = new CL_ComponentType_ScrollBar;
	CL_ComponentManager::component_types["progressbar"] = new CL_ComponentType_ProgressBar;
	CL_ComponentManager::component_types["checkbox"] = new CL_ComponentType_CheckBox;
	CL_ComponentManager::component_types["listbox"] = new CL_ComponentType_ListBox;
	CL_ComponentManager::component_types["filedialog"] = new CL_ComponentType_FileDialog;
	CL_ComponentManager::component_types["window"] = new CL_ComponentType_Window;
}

void CL_SetupGUI::deinit()
{
	delete CL_ComponentManager::component_types["label"];
	delete CL_ComponentManager::component_types["button"];
	delete CL_ComponentManager::component_types["frame"];
	delete CL_ComponentManager::component_types["inputbox"];
	delete CL_ComponentManager::component_types["image"];
	delete CL_ComponentManager::component_types["scrollbar"];
	delete CL_ComponentManager::component_types["progressbar"];
	delete CL_ComponentManager::component_types["checkbox"];
	delete CL_ComponentManager::component_types["listbox"];
	delete CL_ComponentManager::component_types["filedialog"];
	delete CL_ComponentManager::component_types["window"];
}
