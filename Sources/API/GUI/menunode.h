/*
	$Id: menunode.h,v 1.2 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_menunode
#define header_menunode

#include <string>

class CL_Point;
class CL_PopupMenu;
class CL_Component;
class CL_MenuNode_Generic;

//: MenuNode	
class CL_MenuNode
{
//! Construction:
public:
	//: The menunode constructor.
	//: Not to be constructed by user - use insert_item() instead, which
	//: creates a CL_MenuNode.
	CL_MenuNode(CL_PopupMenu *popupmenu);
	
	virtual ~CL_MenuNode();

//! Attributes:
public:
	//: Returns true if node is selected
	bool is_selected() const;
	
	//: Return true if node is collapsed
	bool is_collapsed() const;

	//: Return true if node has sub-items
	bool has_children() const;

	//: Returns the attaches userdata, if any.
	void *get_userdata() const;

	//: Returns the CL_Component this node uses.
	CL_Component *get_component() const;

//! Operations:
public:
	//: Inserts an item at index. 
	//- If index is negative, text is inserted at the end of the list.
	//- Returns the node of the item.
	CL_MenuNode *insert_item(const std::string &text, int index = -1);

	//: Inserts an item (uses any type of component) at index. 
	//- If index is negative, text is inserted at the end of the list.
	//- Set delete_component to true if you want the component to be deleted when
	//- this node gets deleted.
	//- Returns the node of the item.
	CL_MenuNode *insert_item(CL_Component *component, bool delete_component = false, int index = -1);

	//: Sets/changes a new component for the node.
	//- Mostly used when add(CL_Component *) is used with a NULL parameter.
	//- Set delete_component to true if you want the component to be deleted when
	//- this node gets deleted.
	void set_component(CL_Component *component, bool delete_component = false);

	//: Collapse this node.
	void set_collapsed(bool collapse = true);
	
	//: Select this node.
	void set_selected(bool select = true);

	//: Select any of the children nodes.
	void set_selected(CL_MenuNode *node, bool select = true);

	//: Deselects current selected item.
	void clear_selection();

	//: Inverts the selection.
	//: Works only in Multi selection mode.
	void invert_selection();

	//: Deletes all items.
	void clear();

	//: Attaches userdata to node.
	void set_userdata(void *data);

	void draw_nodes(CL_Point &point);
	
//! Implementation:
private:
	CL_MenuNode(const CL_MenuNode &copy) { return; } // disallow copy construction.
	CL_MenuNode_Generic *impl;
};

#endif
