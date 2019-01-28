/*
	$Id: treeitem.h,v 1.2 2002/03/02 18:27:31 japj Exp $

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

#ifndef header_treeitem
#define header_treeitem

#include "component.h"

class CL_TreeItem_Generic;
class CL_TreeNode;

//: TreeView_Item component
class CL_TreeItem : public CL_Component
{
//! Construction:
public:
	//: TreeView Item Constructor
	CL_TreeItem(
		CL_TreeNode *node,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: TreeView Item Constructor
	CL_TreeItem(
		CL_TreeNode *node,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: TreeView Item Destructor
	virtual ~CL_TreeItem();

//! Attributes:
public:
	//: Returns the item label.
	//- Returns - The item label.
	const std::string &get_text() const;

	//: Returns the TreeView node this component is used in.
	CL_TreeNode *get_node() const;

//! Operations:
public:
	//: Sets the item label.
	//- text - The item label.
	void set_text(const std::string &text);

//! Signals:
public:
	//: This signal is emitted when the item is clicked on.
	CL_Signal_v0 &sig_clicked();

//! Implementation:
private:
	CL_TreeItem(const CL_TreeItem &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_TreeItem_Generic *impl;
};

#endif
