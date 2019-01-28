/*
	$Id: inputbox_default.h,v 1.22 2001/12/27 23:11:23 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_inputbox_default
#define header_inputbox_default

#include "API/GUI/inputbox.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_Font;

class CL_InputBox_Default : public CL_ComponentStyle
{
public:
	CL_InputBox_Default(
		CL_InputBox *inputbox,
		CL_StyleManager_Default *style);
	
	virtual ~CL_InputBox_Default();

private:
	CL_Slot slot_paint;
	void on_paint();

	CL_Slot slot_activity;
	void on_activity();

	CL_InputBox *inputbox;

	int get_mouse_position(int x, int y);

	CL_StyleManager_Default *style;
	CL_ResourceManager *resources;
	CL_Font *font;
	CL_Font *font_disabled;

	int character_offset;

	bool show_cursor;
	unsigned int cursor_blink_time;
};

#endif
