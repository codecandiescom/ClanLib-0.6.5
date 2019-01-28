/*
	$Id: treeview_default.cpp,v 1.11 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "treeview_default.h"
#include "API/Display/Display/display.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView_Default::CL_TreeView_Default(
	CL_TreeView *_treeview,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(_treeview), treeview(_treeview)
{
	this->style = style;

	slot_paint = treeview->sig_paint().connect(
		this, &CL_TreeView_Default::on_paint);

	slot_paint_node = treeview->sig_paint_node().connect(
		this, &CL_TreeView_Default::on_paint_node);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeView_Default::on_paint()
{
	int width = treeview->get_width();
	int height = treeview->get_height();

	bool focus = treeview->has_child(treeview->get_focus());

	if(treeview->is_enabled() == false || focus == false)
	{
		// Fill
		style->fill_rect(1, 1, width - 1, height - 1, GUICOLOR_WHITE);

		// Outline
		style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);
	}
	else
	{
		// Fill
		style->fill_rect(2, 2, width - 2, height - 2, GUICOLOR_WHITE);

		// Outline
		style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

		// Shade box
		style->draw_box(1, 1, width - 1, height - 1, GUICOLOR_DARKER_SHADE, GUICOLOR_MEDIUM_SHADE);
	}
}

void CL_TreeView_Default::on_paint_node(CL_TreeNode *node, CL_Point &point)
{
	CL_Component *component = node->get_component();
	
	int height = component->get_height();
	int mid = (height) / 2;

	// Draw collapse box
	if(node->has_children())
	{
		// Box
		CL_Display::draw_rect(point.x, point.y + mid - 5, point.x + 9, point.y + mid + 4, 0.0f, 0.0f, 0.0f);

		// Horizontal line
		CL_Display::draw_line(point.x + 2, point.y + mid - 1, point.x + 6, point.y + mid - 1, 0.0f, 0.0f, 0.0f);

		// Vertical line
		if(node->is_collapsed())
			CL_Display::draw_line(point.x + 4, point.y + mid - 3, point.x + 4, point.y + mid + 1, 0.0f, 0.0f, 0.0f);
	}

	// Size and place child component
	component->set_position(point.x + 12, point.y);

	// Draw component
	CL_Display::push_translate_offset(point.x + 12, point.y);
	component->sig_paint()();
	if(node->is_selected())
		CL_Display::fill_rect(0, 0, component->get_width(), height, 0.5f, 0.5f, 1.0f, 0.3f);
	CL_Display::pop_translate_offset();

	// Move down for next component
	point.y += height;

	// If it has any children, adjust the x-position for the children
	if(node->is_collapsed() == false)
		point.x += 15;
}
