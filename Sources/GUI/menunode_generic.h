/*
	$Id: menunode_generic.h,v 1.1 2002/03/01 22:19:09 sphair Exp $

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

#ifndef header_menunode_generic
#define header_menunode_generic

//: MenuNode
class CL_MenuNode_Generic
{
public:
	CL_MenuNode_Generic(CL_MenuNode *self, CL_PopupMenu *root);
	
	void set_collapsed(bool new_collapsed);
		
	CL_PopupMenu *root;
	CL_Component *component;
	bool delete_component;

	std::list<CL_MenuNode *> children;

	CL_SlotContainer slots;

	void *userdata;

	bool collapsed;
	bool selected;

	void on_child_click(const CL_Key &key);

	CL_Signal_v1<CL_Point &> sig_paint_node;
	
private:
	CL_MenuNode *menunode;
};

#endif
