/*
	$Id: treeview_generic.cpp,v 1.12 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "treeview_generic.h"
#include "API/GUI/component_options.h"
#include "API/GUI/gui_manager.h"
#include "API/Display/Display/display.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView_Generic::CL_TreeView_Generic(CL_TreeView *self)
: treeview(self)
{
	// Create client area (which is the main treeview)
	client_area = new CL_Component(treeview);

	slot_paint_children = client_area->sig_paint_children().connect_virtual(
		this, &CL_TreeView_Generic::on_paint_children);
	slot_resize = treeview->sig_resize().connect(
		this, &CL_TreeView_Generic::on_resize);

	on_resize(treeview->get_width(), treeview->get_height());
}

CL_TreeView_Generic::~CL_TreeView_Generic()
{
	delete client_area;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeView_Generic::on_paint_children(CL_SlotParent_v0 &super)
{
	CL_Point point;
	treeview->draw_nodes(point);
}

void CL_TreeView_Generic::on_resize(int old_width, int old_height)
{
	int width = treeview->get_width();
	int height = treeview->get_height();

	CL_Rect rect(4, 4, 4 + width - 8, 4 + height - 8);
	client_area->set_position(rect);
}
