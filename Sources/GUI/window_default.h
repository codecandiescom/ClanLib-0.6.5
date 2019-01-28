/*
	$Id: window_default.h,v 1.14 2001/12/28 22:41:50 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_window_default
#define header_window_default

#include "API/GUI/window.h"
#include "API/GUI/button.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/layout_manager.h"
#include "API/GUI/component_style.h"
#include "API/GUI/component_move_handler.h"
#include "API/GUI/component_resize_handler.h"

class CL_Font;

class CL_Window_Default : public CL_ComponentStyle
{
public:
	CL_Window_Default(
		CL_Window *window,
		CL_StyleManager_Default *style);

	virtual ~CL_Window_Default();

private:
	CL_Slot slot_paint;
	CL_Slot slot_close;
	CL_Slot slot_resize;

	void on_paint();
	void on_close();
	void on_resize(int old_width, int old_height);

	CL_Window *window;
	CL_Button *button_close;

	CL_ComponentMoveHandler *move_handler;
	CL_ComponentResizeHandler *resize_handler;

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Font *font;
	CL_Font *font_disabled;

	CL_LayoutManager layout;

	int titlebar_height;
};

#endif
