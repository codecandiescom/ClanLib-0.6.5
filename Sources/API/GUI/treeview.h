/*
	$Id: treeview.h,v 1.18 2002/03/01 22:19:08 sphair Exp $

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

#ifndef header_treeview
#define header_treeview

#include "component.h"
#include "treenode.h"

class CL_TreeView_Generic;

//: TreeView component
class CL_TreeView : public CL_Component, public CL_TreeNode
{
//! Construction:
public:
	//: TreeView Constructor
	CL_TreeView(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: TreeView Constructor
	CL_TreeView(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: TreeView Destructor
	virtual ~CL_TreeView();

//! Attributes:
public:
	//: Returns the client area of the component.
	CL_Component *get_client_area() const;

//! Signals:
public:
	//: This signal is emitted when the selection set of a tree changes.
	CL_Signal_v1<const CL_TreeNode &> &sig_selection_changed();

	//: Draws the node (used by themes).
	CL_Signal_v2<CL_TreeNode *, CL_Point &> &sig_paint_node();

//! Implementation:
private:
	CL_TreeView(const CL_TreeView &copy) : CL_Component(NULL, NULL), CL_TreeNode(NULL) { return; } // disallow copy construction.
	CL_TreeView_Generic *impl;
};

#endif
