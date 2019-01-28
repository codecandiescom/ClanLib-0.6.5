/*
	$Id: popupmenu_default.h,v 1.18 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_popupmenu_default
#define header_popupmenu_default

#include "API/GUI/popupmenu.h"
#include "API/GUI/stylemanager_default.h"
#include "API/GUI/component_style.h"

class CL_PopupMenu_Default : public CL_ComponentStyle
{
public:
	CL_PopupMenu_Default(
		CL_PopupMenu *listbox,
		CL_StyleManager_Default *style);
	
	virtual ~CL_PopupMenu_Default() {};

private:
	CL_Slot slot_paint;
	CL_Slot slot_paint_node;

	void on_paint();
	void on_paint_node(CL_MenuNode *node, CL_Point &point);

	CL_PopupMenu *popupmenu;

	CL_StyleManager_Default *style;
};

#endif
