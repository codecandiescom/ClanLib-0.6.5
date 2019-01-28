/*
	$Id: treeitem.cpp,v 1.1 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include "treeitem_generic.h"
#include "component_generic.h"
#include "API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem::CL_TreeItem(
	CL_TreeNode *node,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_TreeItem_Generic(this, node, "");
	get_style_manager()->connect_styles("treeview_item", this);
	find_preferred_size();
}

CL_TreeItem::CL_TreeItem(
	CL_TreeNode *node,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_TreeItem_Generic(this, node, text);
	get_style_manager()->connect_styles("treeview_item", this);
	find_preferred_size();
}

CL_TreeItem::~CL_TreeItem()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_TreeItem::get_text() const
{
	return impl->text;
}

CL_TreeNode *CL_TreeItem::get_node() const
{
	return impl->node;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_TreeItem::set_text(const std::string &text)
{
	impl->text = text;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v0 &CL_TreeItem::sig_clicked()
{
	return impl->sig_clicked;
}

