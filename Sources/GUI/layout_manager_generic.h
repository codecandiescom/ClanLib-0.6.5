/*
	$Id: layout_manager_generic.h,v 1.4 2001/09/08 19:12:51 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_layout_manager_generic
#define header_layout_manager_generic

#include "API/GUI/layout_manager.h"
#include "API/signals.h"

class CL_LayoutManager_Item
{
public:
	CL_Component *dest;
	CL_LayoutManager::ELayoutPosition dest_position;
	CL_Component *source;
	CL_LayoutManager::ELayoutPosition source_position;

	CL_Slot slot_resize;
	CL_Slot slot_move;
};

class CL_LayoutManager_Generic
{
// Construction:
public:
	CL_LayoutManager_Generic(CL_LayoutManager *self);

	virtual ~CL_LayoutManager_Generic();

// Attributes:
public:

// Operations:
public:
	void add_resize_position(
		CL_Component *destination, CL_LayoutManager::ELayoutPosition destination_position,
		CL_Component *source, CL_LayoutManager::ELayoutPosition source_position);

	void add_resize_bottomleft(CL_Component *destination, CL_Component *source);
	void add_resize_left(CL_Component *destination, CL_Component *source);

// Callbacks:
protected:
	void on_resize(int old_width, int old_height, CL_LayoutManager_Item *item);
	void on_move(int old_x, int old_y, CL_LayoutManager_Item *item);

// Implementation:
private:
	CL_LayoutManager *layoutmanager;

	std::list<CL_LayoutManager_Item *> items;
};

#endif
