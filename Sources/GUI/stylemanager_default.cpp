/*
	$Id: stylemanager_default.cpp,v 1.58 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include <cstdio>
#include <string>

#include "API/Display/Display/display.h"
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_manager.h"

#include "label_default.h"
#include "button_default.h"
#include "frame_default.h"
#include "image_default.h"
#include "inputbox_default.h"
#include "scrollbar_default.h"
#include "progressbar_default.h"
#include "checkbox_default.h"
#include "radiobutton_default.h"
#include "listbox_default.h"
#include "treeview_default.h"
#include "treeitem_default.h"
#include "popupmenu_default.h"
#include "window_default.h"

#define RGBCOL(x) (float(x)*(1/255.0f))

// Construction:

CL_StyleManager_Default::CL_StyleManager_Default(CL_ResourceManager *resources)
: CL_StyleManager(resources)
{
}

CL_StyleManager_Default::~CL_StyleManager_Default()
{
}

void CL_StyleManager_Default::get_color(
	GUIColor col,
	float &r,
	float &g,
	float &b,
	float &a)
{
	a = 1.0f;

	switch (col)
	{
	case GUICOLOR_SELECTED_OUTLINE:
		break;
	case GUICOLOR_DARK_OUTLINE:
		r = RGBCOL(128);
		g = RGBCOL(128);
		b = RGBCOL(128);
		break;
	case GUICOLOR_BRIGHT_SHADE:
		r = RGBCOL(255);
		g = RGBCOL(255);
		b = RGBCOL(255);
		break;
	case GUICOLOR_MEDIUM_SHADE:
		r = RGBCOL(211);
		g = RGBCOL(208);
		b = RGBCOL(200);
		break;
	case GUICOLOR_DARKER_SHADE:
		r = RGBCOL(65);
		g = RGBCOL(65);
		b = RGBCOL(65);
		break;
	case GUICOLOR_DARK_SHADE:
		r = RGBCOL(188);
		g = RGBCOL(190);
		b = RGBCOL(184);
		break;
	case GUICOLOR_DARK_OUTLINE_DISABLED:
		r = RGBCOL(192);
		g = RGBCOL(194);
		b = RGBCOL(189);
		break;
	case GUICOLOR_BRIGHT_SHADE_DISABLED:
		r = RGBCOL(243);
		g = RGBCOL(245);
		b = RGBCOL(241);
		break;
	case GUICOLOR_DARK_SHADE_DISABLED:
		r = RGBCOL(226);
		g = RGBCOL(227);
		b = RGBCOL(222);
		break;
	case GUICOLOR_WHITE:
		r = RGBCOL(255);
		g = RGBCOL(255);
		b = RGBCOL(255);
		break;
	case GUICOLOR_RED:
		r = RGBCOL(255);
		g = RGBCOL(55);
		b = RGBCOL(55);
		break;
	case GUICOLOR_CARET:
		r = RGBCOL(0);
		g = RGBCOL(0);
		b = RGBCOL(0);
		break;
	case GUICOLOR_PROGRESSBAR_FILLED:
	case GUICOLOR_SELECTION:
		r = RGBCOL(48);
		g = RGBCOL(105);
		b = RGBCOL(197);
		break;
	case GUICOLOR_PROGRESSBAR:
	case GUICOLOR_SCROLLBAR:
		r = RGBCOL(196);
		g = RGBCOL(197);
		b = RGBCOL(190);
		break;
	case GUICOLOR_BUTTON:
		r = RGBCOL(236);
		g = RGBCOL(238);
		b = RGBCOL(232);
		break;
	case GUICOLOR_FOCUS:
		r = RGBCOL(236);
		g = RGBCOL(238);
		b = RGBCOL(100);
		break;
	case GUICOLOR_BUTTON_DISABLED:
		r = RGBCOL(232);
		g = RGBCOL(235);
		b = RGBCOL(230);
		break;
	case GUICOLOR_BUTTON_TOGGLED:
		r = RGBCOL(230);
		g = RGBCOL(231);
		b = RGBCOL(225);
		break;
	case GUICOLOR_WINDOW_NORMAL:
		r = RGBCOL(235);
		g = RGBCOL(235);
		b = RGBCOL(227);
		break;
	case GUICOLOR_WINDOW_TITLEBAR:
		r = RGBCOL(40);
		g = RGBCOL(90);
		b = RGBCOL(150);
		break;
	case GUICOLOR_WINDOW_TITLEBAR_DISABLED:
		r = RGBCOL(40);
		g = RGBCOL(40);
		b = RGBCOL(40);
		break;
	}
}

void CL_StyleManager_Default::fill_rect(int x1, int y1, int x2, int y2, GUIColor col)
{
	float r = 1.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;

	get_color(col, r, g, b, a);

	CL_Display::fill_rect(x1, y1, x2, y2, r, g, b);
}

void CL_StyleManager_Default::draw_rect(int x1, int y1, int x2, int y2, GUIColor col)
{
	float r = 1.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;

	get_color(col, r, g, b, a);

	CL_Display::draw_rect(x1, y1, x2, y2, r, g, b);
}

void CL_StyleManager_Default::draw_line(int x1, int y1, int x2, int y2, GUIColor col)
{
	float r = 1.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;

	get_color(col, r, g, b, a);

	CL_Display::draw_line(x1, y1, x2, y2, r, g, b, a);
}

void CL_StyleManager_Default::draw_box(int x1, int y1, int x2, int y2, GUIColor topleft, GUIColor bottomright)
{
	float r = 1.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;

	get_color(topleft, r, g, b, a);

	CL_Display::draw_line(x1, y1, x2 - 1, y1, r, g, b, a);
	CL_Display::draw_line(x1, y1 + 1, x1, y2 - 1, r, g, b, a);
	
	r = 1.0f;
	g = 0.0f;
	b = 1.0f;
	a = 1.0f;

	get_color(bottomright, r, g, b, a);

	CL_Display::draw_line(x1 + 1, y2 - 1, x2 - 1, y2 - 1, r, g, b, a);
	CL_Display::draw_line(x2 - 1, y1 + 1, x2 - 1, y2 - 2, r, g, b, a);
}

// Overridables:

CL_Component *CL_StyleManager_Default::create_component(
	const std::string &type,
	CL_Component *parent)
{
	return CL_ComponentManager::create_component(type, parent, this);
}

void CL_StyleManager_Default::connect_styles(
	const std::string &type,
	CL_Component *owner)
{
	if (type == "label")
		owner->attach_style(new CL_Label_Default((CL_Label *) owner, this));

	else if (type == "button")
		owner->attach_style(new CL_Button_Default((CL_Button *) owner, this));

	else if (type == "frame")
		owner->attach_style(new CL_Frame_Default((CL_Frame *) owner, this));

	else if (type == "image")
		owner->attach_style(new CL_Image_Default((CL_Image *) owner, this));

	else if (type == "inputbox")
		owner->attach_style(new CL_InputBox_Default((CL_InputBox *) owner, this));

	else if (type == "scrollbar")
		owner->attach_style(new CL_ScrollBar_Default((CL_ScrollBar *) owner, this));

	else if (type == "popupmenu")
		owner->attach_style(new CL_PopupMenu_Default((CL_PopupMenu *) owner, this));

	else if (type == "progressbar")
		owner->attach_style(new CL_ProgressBar_Default((CL_ProgressBar *) owner, this));

	else if (type == "radiobutton")
		owner->attach_style(new CL_RadioButton_Default((CL_RadioButton *) owner, this));

	else if (type == "checkbox")
		owner->attach_style(new CL_CheckBox_Default((CL_CheckBox *) owner, this));

	else if (type == "listbox")
		owner->attach_style(new CL_ListBox_Default((CL_ListBox *) owner, this));

	else if (type == "treeview")
		owner->attach_style(new CL_TreeView_Default((CL_TreeView *) owner, this));

	else if (type == "treeview_item")
		owner->attach_style(new CL_TreeItem_Default((CL_TreeItem *) owner, this));

	else if (type == "window")
		owner->attach_style(new CL_Window_Default((CL_Window *) owner, this));
}
