/*
	$Id: stylemanager_default.h,v 1.22 2002/02/28 11:38:16 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_stylemanager_default
#define header_stylemanager_default

#include "stylemanager.h"
#include "component_options.h"

class CL_ResourceManager;
class CL_Component;
class CL_StyleManager_Default_Generic;

//: GUIColor Enum
enum GUIColor
{
	GUICOLOR_SELECTED_OUTLINE,
	GUICOLOR_DARK_OUTLINE,
	GUICOLOR_DARK_OUTLINE_DISABLED,
	GUICOLOR_DARKER_SHADE,
	GUICOLOR_MEDIUM_SHADE,
	GUICOLOR_BRIGHT_SHADE,
	GUICOLOR_BRIGHT_SHADE_DISABLED,
	GUICOLOR_DARK_SHADE,
	GUICOLOR_DARK_SHADE_DISABLED,
	GUICOLOR_WHITE,
	GUICOLOR_RED,
	GUICOLOR_SELECTION,
	GUICOLOR_CARET,
	GUICOLOR_SCROLLBAR,
	GUICOLOR_FOCUS,
	GUICOLOR_BUTTON,
	GUICOLOR_BUTTON_DISABLED,
	GUICOLOR_BUTTON_TOGGLED,
	GUICOLOR_WINDOW_NORMAL,
	GUICOLOR_WINDOW_TITLEBAR,
	GUICOLOR_WINDOW_TITLEBAR_DISABLED,
	GUICOLOR_PROGRESSBAR,
	GUICOLOR_PROGRESSBAR_FILLED
};

//: Style manager that uses the default clanlib gui style.
class CL_StyleManager_Default : public CL_StyleManager
{
//! Construction:
public:
	//: Construct a default theme style manager using the specified resources.
	CL_StyleManager_Default(CL_ResourceManager *resources);

	//: Destructor.
	virtual ~CL_StyleManager_Default();

//! Attributes:
public:
	void get_color(GUIColor col, float &r, float &g, float &b, float &a);
	
//! Operations:
public:
	//: Fill rect
	void fill_rect(int x1, int y1, int x2, int y2, GUIColor col);

	//: Draw rect
	void draw_rect(int x1, int y1, int x2, int y2, GUIColor col);

	//: Draw line
	void draw_line(int x1, int y1, int x2, int y2, GUIColor col);

	//: Draw box
	void draw_box(int x1, int y1, int x2, int y2, GUIColor topleft, GUIColor bottomright);

//! Overrideables:
public:
	//: Create a component based on a type name.
	virtual CL_Component *create_component(
		const std::string &type,
		CL_Component *parent);

	//: Connect component styles to component.
	//: The 'type' parameter indicates what type the component is.
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner);

//! Implementation:
private:
	// Disallow copy contruction of style managers.
	CL_StyleManager_Default(const CL_StyleManager_Default &copy) : CL_StyleManager(NULL) { return; }

	CL_StyleManager_Default_Generic *impl;
};

#endif
