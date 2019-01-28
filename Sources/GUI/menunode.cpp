/*
	$Id: menunode.cpp,v 1.2 2002/03/16 17:29:39 plasmoid Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include "API/GUI/component.h"
#include "API/GUI/popupmenu.h"
#include "API/GUI/menunode.h"
#include "menunode_generic.h"

#include "API/GUI/label.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuNode::CL_MenuNode(CL_PopupMenu *root)
{
	impl = new CL_MenuNode_Generic(this, root);
}

CL_MenuNode::~CL_MenuNode()
{
	clear();
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

void *CL_MenuNode::get_userdata() const
{
	return impl->userdata;
}

CL_Component *CL_MenuNode::get_component() const
{
	return impl->component;
}

bool CL_MenuNode::is_selected() const
{
	return impl->selected;
}

bool CL_MenuNode::is_collapsed() const
{
	return impl->collapsed;
}

bool CL_MenuNode::has_children() const
{
	return (impl->children.empty() == false);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

CL_MenuNode *CL_MenuNode::insert_item(const std::string &text, int index)
{
	CL_MenuNode *node = new CL_MenuNode(impl->root);
	impl->children.push_back(node);

	CL_Component *new_component = new CL_Label(text, impl->root->get_client_area());

	node->set_component(new_component, true);

	return node;
}

CL_MenuNode *CL_MenuNode::insert_item(CL_Component *new_component, bool delete_component, int index)
{
	CL_MenuNode *node = new CL_MenuNode(impl->root);
	impl->children.push_back(node);

	impl->root->get_client_area()->add_child(new_component);

	node->set_component(new_component, delete_component);

	return node;
}

void CL_MenuNode::set_component(CL_Component *component, bool delete_component)
{
	if(impl->component)
	{
		// TODO: Disconnect the signal:
		// impl->slots.connect(new_component->sig_mouse_up(), impl, &CL_MenuNode_Generic::on_child_click);

		if(impl->delete_component)
			delete impl->component;
	}

	impl->component = component;
	impl->slots.connect(component->sig_mouse_up(), impl, &CL_MenuNode_Generic::on_child_click);
	impl->delete_component = delete_component;
}

void CL_MenuNode::set_collapsed(bool collapsed)
{
	impl->set_collapsed(collapsed);
}
	
void CL_MenuNode::set_selected(bool select)
{
	impl->selected = select;
}

void CL_MenuNode::set_selected(CL_MenuNode *node, bool select)
{
	node->set_selected(select);
}

void CL_MenuNode::clear_selection()
{
	set_selected(false);

	std::list<CL_MenuNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		(*it)->clear_selection();
}

void CL_MenuNode::clear()
{
	std::list<CL_MenuNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		delete (*it);

	impl->children.clear();

	if(impl->component && impl->delete_component)
		delete impl->component;
}

void CL_MenuNode::set_userdata(void *data)
{
	impl->userdata = data;
}

void CL_MenuNode::draw_nodes(CL_Point &point)
{
	std::list<CL_MenuNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
	{
		int x = point.x;
		impl->root->sig_paint_node()(*it, point);
		if((*it)->is_collapsed() == false)
			(*it)->draw_nodes(point);
		point.x = x;
	}
}
