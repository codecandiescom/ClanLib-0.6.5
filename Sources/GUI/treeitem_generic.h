/*
	$Id: treeitem_generic.h,v 1.1 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_treeitem_generic
#define header_treeitem_generic

#include "API/GUI/treeitem.h"

class CL_TreeNode;

class CL_TreeItem_Generic
{
// Construction:
public:
	CL_TreeItem_Generic(CL_TreeItem *self, CL_TreeNode *node, const std::string &text);
	~CL_TreeItem_Generic() {};

// Attributes:
public:
	std::string text;
	CL_TreeNode *node;

// Signals:
public:
	CL_Signal_v0 sig_clicked;

// Slots:
private:
	CL_Slot slot_mouse_down;
	CL_Slot slot_set_options;

// Callbacks:
private:
	void on_mouse_down(const CL_Key &key);
	void on_set_options(const CL_ComponentOptions &options);

// Implementation:
private:
	CL_TreeItem *item;
};

#endif
