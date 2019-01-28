/*
	$Id: treenode_generic.h,v 1.3 2002/03/02 18:27:32 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_treenode_generic
#define header_treenode_generic

#include <list>
#include "../API/Signals/slot_container.h"
#include "../API/Core/Math/point.h"
#include "../API/Display/Input/key.h"

class CL_TreeNode;

//: TreeView node
class CL_TreeNode_Generic
{
public:
	CL_TreeNode_Generic(CL_TreeNode *self, CL_TreeView *root);
	
	void set_collapsed(bool new_collapsed);
		
	CL_TreeView *root;
	CL_Component *component;
	bool delete_component;

	std::list<CL_TreeNode *> children;

	CL_SlotContainer slots;

	void *userdata;

	bool collapsed;
	bool selected;

	void on_child_click(const CL_Key &key);

	CL_Signal_v1<CL_Point &> sig_paint_node;
	
private:
	CL_TreeNode *treenode;
};

#endif
