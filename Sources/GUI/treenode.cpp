/*
	$Id: treenode.cpp,v 1.2 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include "API/GUI/treeview.h"
#include "API/GUI/treeitem.h"
#include "API/GUI/treenode.h"
#include "treenode_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeNode::CL_TreeNode(CL_TreeView *root)
{
	impl = new CL_TreeNode_Generic(this, root);
}

CL_TreeNode::~CL_TreeNode()
{
	clear();
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

void *CL_TreeNode::get_userdata() const
{
	return impl->userdata;
}

CL_Component *CL_TreeNode::get_component() const
{
	return impl->component;
}

bool CL_TreeNode::is_selected() const
{
	return impl->selected;
}

bool CL_TreeNode::is_collapsed() const
{
	return impl->collapsed;
}

bool CL_TreeNode::has_children() const
{
	return (impl->children.empty() == false);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

CL_TreeNode *CL_TreeNode::insert_item(const std::string &text, int index)
{
	CL_TreeNode *node = new CL_TreeNode(impl->root);
	impl->children.push_back(node);

	CL_Component *new_component = new CL_TreeItem(node, text, impl->root->get_client_area());

	node->set_component(new_component, true);

	return node;
}

CL_TreeNode *CL_TreeNode::insert_item(CL_Component *new_component, bool delete_component, int index)
{
	CL_TreeNode *node = new CL_TreeNode(impl->root);
	impl->children.push_back(node);

	impl->root->get_client_area()->add_child(new_component);

	node->set_component(new_component, delete_component);

	return node;
}

void CL_TreeNode::set_component(CL_Component *component, bool delete_component)
{
	if(impl->component)
	{
		// TODO: Disconnect the signal:
		// impl->slots.connect(new_component->sig_mouse_up(), impl, &CL_TreeNode_Generic::on_child_click);

		if(impl->delete_component)
			delete impl->component;
	}

	impl->component = component;
	impl->slots.connect(component->sig_mouse_up(), impl, &CL_TreeNode_Generic::on_child_click);
	impl->delete_component = delete_component;
}

void CL_TreeNode::set_collapsed(bool collapsed)
{
	impl->set_collapsed(collapsed);
}
	
void CL_TreeNode::set_selected(bool select)
{
	impl->selected = select;
}

void CL_TreeNode::set_selected(CL_TreeNode *node, bool select)
{
	node->set_selected(select);
}

void CL_TreeNode::clear_selection()
{
	set_selected(false);

	std::list<CL_TreeNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		(*it)->clear_selection();
}

void CL_TreeNode::clear()
{
	std::list<CL_TreeNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		delete (*it);

	impl->children.clear();

	if(impl->component && impl->delete_component)
		delete impl->component;
}

void CL_TreeNode::set_userdata(void *data)
{
	impl->userdata = data;
}

void CL_TreeNode::draw_nodes(CL_Point &point)
{
	std::list<CL_TreeNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
	{
		int x = point.x;
		impl->root->sig_paint_node()(*it, point);
		if((*it)->is_collapsed() == false)
			(*it)->draw_nodes(point);
		point.x = x;
	}
}
