/*
	$Id: treenode_generic.cpp,v 1.3 2002/03/02 18:27:32 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "../API/GUI/treenode.h"
#include "treenode_generic.h"
#include "../API/GUI/treeview.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeNode_Generic::CL_TreeNode_Generic(CL_TreeNode *self, CL_TreeView *root)
{
	treenode = self;
	
	this->root = root;
	component = NULL;
	delete_component = false;
	collapsed = false;
	selected = false;
	userdata = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_TreeNode_Generic::set_collapsed(bool new_collapsed)
{
	collapsed = new_collapsed;

	root->sig_selection_changed()(*treenode);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeNode_Generic::on_child_click(const CL_Key &key)
{
	set_collapsed(!collapsed);
}
