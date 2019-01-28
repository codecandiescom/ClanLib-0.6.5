/*
	$Id: treeview_generic.h,v 1.10 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_treeview_generic
#define header_treeview_generic

#include "API/GUI/treeview.h"

class CL_TreeView_Generic
{
// Construction:
public:
	CL_TreeView_Generic(CL_TreeView *self);
	~CL_TreeView_Generic();

	CL_TreeView *treeview;

// Attributes:
public:
	CL_Component *client_area;

// Signals:
public:
	CL_Signal_v1<const CL_TreeNode &> sig_selection_changed;
	CL_Signal_v2<CL_TreeNode *, CL_Point &> sig_paint_node;

// Slots:
private:
	CL_Slot slot_paint_children;
	CL_Slot slot_resize;

// Callbacks:
private:
	void on_paint_children(CL_SlotParent_v0 &super);
	void on_resize(int old_width, int old_height);
};

#endif
