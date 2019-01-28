/*
	$Id: popupmenu_generic.cpp,v 1.14 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "popupmenu_generic.h"
#include "API/GUI/component_options.h"
#include "API/GUI/gui_manager.h"
#include "API/Display/Display/display.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_PopupMenu_Generic::CL_PopupMenu_Generic(CL_PopupMenu *self)
: popupmenu(self)
{
	// Create client area (which is the main popupmenu)
	client_area = new CL_Component(popupmenu);

	slot_paint_children = client_area->sig_paint_children().connect_virtual(
		this, &CL_PopupMenu_Generic::on_paint_children);
	slot_resize = popupmenu->sig_resize().connect(
		this, &CL_PopupMenu_Generic::on_resize);
	slot_lost_focus = popupmenu->sig_lost_focus().connect(
		this, &CL_PopupMenu_Generic::on_lost_focus);

	on_resize(popupmenu->get_width(), popupmenu->get_height());
}

CL_PopupMenu_Generic::~CL_PopupMenu_Generic()
{
	delete client_area;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_PopupMenu_Generic::on_paint_children(CL_SlotParent_v0 &super)
{
	CL_Point point;
	popupmenu->draw_nodes(point);
}

void CL_PopupMenu_Generic::on_resize(int old_width, int old_height)
{
	int width = popupmenu->get_width();
	int height = popupmenu->get_height();

	CL_Rect rect(4, 4, 4 + width - 8, 4 + height - 8);
	client_area->set_position(rect);
}

void CL_PopupMenu_Generic::on_lost_focus()
{
	sig_cancelled();

//	popupmenu->close();
}
