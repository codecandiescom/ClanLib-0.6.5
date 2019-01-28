/*
	$Id: popupmenu_default.cpp,v 1.27 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "popupmenu_default.h"
#include "API/Display/Display/display.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_PopupMenu_Default::CL_PopupMenu_Default(
	CL_PopupMenu *popupmenu,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(popupmenu), popupmenu(popupmenu)
{
	this->style = style;

	slot_paint = popupmenu->sig_paint().connect(
		this, &CL_PopupMenu_Default::on_paint);

	slot_paint_node = popupmenu->sig_paint_node().connect(
		this, &CL_PopupMenu_Default::on_paint_node);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_PopupMenu_Default::on_paint()
{
	bool focus = popupmenu->has_child(popupmenu->get_focus());

	int width = popupmenu->get_width();
	int height = popupmenu->get_height();
		
	style->draw_box(0, 0, width, height, GUICOLOR_BRIGHT_SHADE, GUICOLOR_DARK_SHADE);
	style->draw_box(1, 1, width - 1, height - 1, GUICOLOR_BRIGHT_SHADE, GUICOLOR_DARK_SHADE);
	style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_WINDOW_NORMAL);
}

void CL_PopupMenu_Default::on_paint_node(CL_MenuNode *node, CL_Point &point)
{
	CL_Component *component = node->get_component();
	
	int height = component->get_height();
	int mid = (height) / 2;

	// Draw arrow showing this item has a submenu
	if(node->has_children())
	{
	}

	// Size and place child component
	component->set_position(point.x, point.y);

	// Draw component
	CL_Display::push_translate_offset(point.x, point.y);
	component->sig_paint()();
	if(node->is_selected())
		CL_Display::fill_rect(0, 0, component->get_width(), height, 0.5f, 0.5f, 1.0f, 0.3f);
	CL_Display::pop_translate_offset();

	// Move down for next component
	point.y += height;
}
