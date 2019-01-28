/*
	$Id: treeitem_generic.cpp,v 1.2 2002/03/02 18:27:32 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "treeitem_generic.h"
#include "API/GUI/treeitem.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem_Generic::CL_TreeItem_Generic(
	CL_TreeItem *self,
	CL_TreeNode *node,
	const std::string &text)
:  text(text), node(node), item(self)
{
	slot_mouse_down = item->sig_mouse_down().connect(
		this, &CL_TreeItem_Generic::on_mouse_down);
	slot_set_options = item->sig_set_options().connect(
		this, &CL_TreeItem_Generic::on_set_options);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeItem_Generic::on_mouse_down(const CL_Key &key)
{
	item->sig_clicked()();
}

void CL_TreeItem_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("text"))
		text = options.get_value("text");
}
