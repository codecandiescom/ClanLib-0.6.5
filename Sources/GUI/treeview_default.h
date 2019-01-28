/*
	$Id: treeview_default.h,v 1.5 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_treeview_default
#define header_treeview_default

#include "API/GUI/treeview.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_TreeView_Default : public CL_ComponentStyle
{
public:
	CL_TreeView_Default(
		CL_TreeView *listbox,
		CL_StyleManager_Default *style);
	
	virtual ~CL_TreeView_Default() {};

private:
	CL_Slot slot_paint;
	CL_Slot slot_paint_node;

	void on_paint();
	void on_paint_node(CL_TreeNode *node, CL_Point &point);

	CL_TreeView *treeview;

	CL_StyleManager_Default *style;
};

#endif
