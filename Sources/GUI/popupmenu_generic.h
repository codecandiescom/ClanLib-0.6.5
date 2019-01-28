/*
	$Id: popupmenu_generic.h,v 1.10 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_popupmenu_generic
#define header_popupmenu_generic

#include "API/GUI/popupmenu.h"

class CL_PopupMenu_Generic
{
// Construction:
public:
	CL_PopupMenu_Generic(CL_PopupMenu *self);
	~CL_PopupMenu_Generic();

	CL_PopupMenu *popupmenu;

// Attributes:
public:
	CL_Component *client_area;

// Signals:
public:
	CL_Signal_v1<const CL_MenuNode &> sig_selection_changed;
	CL_Signal_v2<CL_MenuNode *, CL_Point &> sig_paint_node;
	CL_Signal_v0 sig_cancelled;
	
// Slots:
private:
	CL_Slot slot_paint_children;
	CL_Slot slot_resize;
	CL_Slot slot_lost_focus;

// Callbacks:
private:
	void on_paint_children(CL_SlotParent_v0 &super);
	void on_resize(int old_width, int old_height);
	void on_lost_focus();
};

#endif
