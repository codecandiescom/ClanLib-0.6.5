/*
	$Id: layout_manager_generic.cpp,v 1.10 2002/01/16 19:06:54 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "layout_manager_generic.h"
#include "API/GUI/component.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_LayoutManager_Generic::CL_LayoutManager_Generic(CL_LayoutManager *self)
:
	layoutmanager(self)
{
}

CL_LayoutManager_Generic::~CL_LayoutManager_Generic()
{
	std::list<CL_LayoutManager_Item *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		delete *it;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_LayoutManager_Generic::add_resize_position(
	CL_Component *destination, CL_LayoutManager::ELayoutPosition destination_position,
	CL_Component *source, CL_LayoutManager::ELayoutPosition source_position)
{
	CL_LayoutManager_Item *item = new CL_LayoutManager_Item;

	item->dest = destination;
	item->dest_position = destination_position;

	item->source = source;
	item->source_position = source_position;

	item->slot_resize = source->sig_resize().connect(
		this, &CL_LayoutManager_Generic::on_resize, item);
//	item->slot_move = source->sig_move().connect(
//		this, &CL_LayoutManager_Generic::on_move, item);

	items.push_back(item);
}

void CL_LayoutManager_Generic::add_resize_bottomleft(
	CL_Component *destination, CL_Component *source)
{
	add_resize_position(destination, CL_LayoutManager::x2, source, CL_LayoutManager::x2);
	add_resize_position(destination, CL_LayoutManager::y2, source, CL_LayoutManager::y2);
}

void CL_LayoutManager_Generic::add_resize_left(
	CL_Component *destination, CL_Component *source)
{
	add_resize_position(destination, CL_LayoutManager::x2, source, CL_LayoutManager::x2);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_LayoutManager_Generic::on_resize(int old_width, int old_height, CL_LayoutManager_Item *item)
{
	int diff_width = item->source->get_width() - old_width;
	int diff_height = item->source->get_height() - old_height;

	CL_Rect rect = item->dest->get_position();

	int diff=0;

	if(item->source_position == CL_LayoutManager::y2)
		diff = diff_height;
	if(item->source_position == CL_LayoutManager::x2)
		diff = diff_width;
	if(item->source_position == CL_LayoutManager::y1)
		diff = 0;
	if(item->source_position == CL_LayoutManager::x1)
		diff = 0;
				
	if(item->dest_position == CL_LayoutManager::x1)
		rect.x1 += diff;
	if(item->dest_position == CL_LayoutManager::y1)
		rect.y1 += diff;
	if(item->dest_position == CL_LayoutManager::x2)
		rect.x2 += diff;
	if(item->dest_position == CL_LayoutManager::y2)
		rect.y2 += diff;

	item->dest->set_position(rect);
}

void CL_LayoutManager_Generic::on_move(int old_x, int old_y, CL_LayoutManager_Item *item)
{
	throw CL_Error("CL_LayoutManager_Generic::on_move not implemented yet");
}
