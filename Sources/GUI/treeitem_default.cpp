/*
	$Id: treeitem_default.cpp,v 1.1 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include "API/Display/Font/font.h"
#include "treeitem_default.h"
#include "API/GUI/stylemanager_default.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem_Default::CL_TreeItem_Default(
	CL_TreeItem *treeview_item,
	CL_StyleManager_Default *style)
: CL_ComponentStyle(treeview_item), item(treeview_item)
{
	this->style = style;
	
	resources = style->get_resources();
	font = CL_Font::load("TreeViewItem/font", resources);
		
	slot_paint = item->sig_paint().connect(
		this, &CL_TreeItem_Default::on_paint);
	slot_get_preferred_size = item->sig_get_preferred_size().connect(
		this, &CL_TreeItem_Default::on_get_preferred_size);
}

CL_TreeItem_Default::~CL_TreeItem_Default()
{
	delete font;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeItem_Default::on_get_preferred_size(CL_Point &size)
{
	size.x = font->get_text_width(item->get_text()) + 8;
	size.y = font->get_height();
}

void CL_TreeItem_Default::on_paint()
{
	int height = item->get_height();
	int width = item->get_width();
	
	if(item->has_mouse_over())
		style->fill_rect(0, 0, width, height, GUICOLOR_SELECTION);
	else
		style->fill_rect(0, 0, width, height, GUICOLOR_WINDOW_NORMAL);

	int font_height = font->get_height();
	font->print_left(4, (height - font_height) / 2, item->get_text().c_str());
}
