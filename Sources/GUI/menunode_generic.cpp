/*
	$Id: menunode_generic.cpp,v 1.2 2002/03/16 17:29:39 plasmoid Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/component.h"
#include "API/GUI/popupmenu.h"
#include "../API/GUI/menunode.h"
#include "menunode_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuNode_Generic::CL_MenuNode_Generic(CL_MenuNode *self, CL_PopupMenu *root)
{
	menunode = self;
	
	this->root = root;
	component = NULL;
	delete_component = false;
	collapsed = false;
	selected = false;
	userdata = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MenuNode_Generic::set_collapsed(bool new_collapsed)
{
	collapsed = new_collapsed;

	root->sig_selection_changed()(*menunode);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_MenuNode_Generic::on_child_click(const CL_Key &key)
{
/*	const std::list<CL_Component *> children = component->get_children();
	if(children.size())
	{
		CL_Component *submenu = *(children.begin());
		component->remove_child(submenu);
		popupmenu->get_gui_manager()->add_child(submenu);

		CL_Rect pos = popupmenu->get_screen_rect();
		CL_Rect comp_pos = component->get_screen_rect();
		submenu->set_position(pos.x1 + popupmenu->get_width(), comp_pos.y1);
		submenu->set_focus();
		submenu->show();
	}
	else
		popupmenu->close();
*/
}
