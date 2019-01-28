/*
	$Id: listbox_generic.cpp,v 1.57 2002/07/07 11:44:09 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"

#include <algorithm>

#include "listbox_generic.h"
#include "API/GUI/listbox.h"
#include "API/GUI/listitem.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"
#include "API/Core/System/cl_assert.h"

const static std::string blank("");

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ListBox_Generic::CL_ListBox_Generic(CL_ListBox *self, int max_visible_items)
: max_visible_items(max_visible_items), listbox(self)
{
	item_height = 12;
	scroll_offset = 0;
	visible_scrollbar = true;
	multi_selection = false;

	if(max_visible_items != -1)
		listbox->set_height(max_visible_items * item_height + 8);
	else
		max_visible_items = (listbox->get_height() - 4) / item_height;

	// Create client area (which is the main listbox)
	CL_Rect rect(4, 4, 4 + listbox->get_width() - 8, 4 + listbox->get_height() - 8);
	client_area = new CL_Component(rect, listbox);

	// Create a scrollbar
	rect = CL_Rect(listbox->get_width() - 19, 3, listbox->get_width() - 3, listbox->get_height() - 3);
	int range = get_count() - max_visible_items;
	scrollbar = new CL_ScrollBar(rect, 0, range, false, listbox);
	update_scrollbar();

	layout.add_resize_position(
		client_area, CL_LayoutManager::x2, listbox, CL_LayoutManager::x2);
	layout.add_resize_position(
		client_area, CL_LayoutManager::y2, listbox, CL_LayoutManager::y2);
	layout.add_resize_position(
		scrollbar, CL_LayoutManager::x2, listbox, CL_LayoutManager::x2);
	layout.add_resize_position(
		scrollbar, CL_LayoutManager::y2, listbox, CL_LayoutManager::y2);

	slot_set_options = listbox->sig_set_options().connect(
		this, &CL_ListBox_Generic::on_set_options);
	slot_mouse_down = client_area->sig_mouse_down().connect(
		this, &CL_ListBox_Generic::on_mouse_down);
	slot_child_add = listbox->sig_child_add().connect(
		this, &CL_ListBox_Generic::on_child_add);
	slot_child_remove = listbox->sig_child_remove().connect(
		this, &CL_ListBox_Generic::on_child_remove);
	slot_scroll_change = scrollbar->sig_value_changed().connect(
		this, &CL_ListBox_Generic::on_scroll_change);
	slot_resize = scrollbar->sig_resize().connect(
		this, &CL_ListBox_Generic::on_resize);
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ListBox_Generic::get_count() const
{
	return items.size();
}

std::vector<CL_ListItem> &CL_ListBox_Generic::get_items()
{
	return items;
}

std::vector<std::string> CL_ListBox_Generic::get_selected_items()
{
	std::vector<std::string> retval;
	std::vector<CL_ListItem>::iterator it;
	
	if(items.empty())
		return retval;
	
	for(it = items.begin(); it != items.end(); ++it)
		if((*it).selected)
			retval.push_back( (*it).str );
	
	return retval;
}

const std::string &CL_ListBox_Generic::get_text(int index) const
{
	int pos = 0;
	std::vector<CL_ListItem>::const_iterator it;
	for(it = items.begin(); it != items.end(); it++, pos++)
		if(pos == index)
			return (*it).str;
	
	return blank;
}

const std::string &CL_ListBox_Generic::get_current_text() const
{
	std::vector<CL_ListItem>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		if((*it).selected)
			return (*it).str;

	return blank;
}

int CL_ListBox_Generic::get_current_item() const
{
	int pos = 0;
	std::vector<CL_ListItem>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it, ++pos)
		if((*it).selected)
			return pos;

	return -1;
}

bool CL_ListBox_Generic::is_selected(int index) const
{
	return (items[index].selected);
}

int CL_ListBox_Generic::get_item_height() const
{
	return item_height;
}

int CL_ListBox_Generic::get_max_visible_items() const
{
	return max_visible_items;
}

int CL_ListBox_Generic::get_top_item() const
{
	return scroll_offset;
}

bool CL_ListBox_Generic::is_multi_selection() const
{
	return multi_selection;
}

bool CL_ListBox_Generic::has_scrollbar() const
{
	return visible_scrollbar;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_ListBox_Generic::insert_item(const std::string &item, int index)
{
	if(index < 0)
		items.push_back(CL_ListItem(item));
	else
		throw CL_Error("CL_ListBox::insert_item() using index is not implemented");

	update_scrollbar();

	return items.size() - 1;
}

void CL_ListBox_Generic::remove_item(int index)
{
	if(index < 0 || index > get_count())
		return;

	int pos = 0;
	std::vector<CL_ListItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it, ++pos)
	{
		if(pos == index)
		{
			items.erase(it);
			break;
		}
	}

	if(get_count() - scroll_offset < max_visible_items)
		scroll_offset = 0;

	update_scrollbar();
}

void CL_ListBox_Generic::change_item(const std::string &text, int index)
{
	items[index].str = text;
}

void CL_ListBox_Generic::set_current_item(int index)
{
	if(index < 0 || index >= get_count())
		return;

	std::vector<CL_ListItem>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		if((*it).selected)
			(*it).selected = false;
	items[index].selected = true;
	
	sig_activated(index);
	sig_selection_changed();
}

void CL_ListBox_Generic::clear_selection()
{
	std::vector<CL_ListItem>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it).selected = false;
}

void CL_ListBox_Generic::sort(bool ascending)
{
	std::sort(items.begin(), items.end());
}

void CL_ListBox_Generic::clear()
{
	items.clear();
	update_scrollbar();
}

void CL_ListBox_Generic::set_item_height(int new_item_height)
{
	item_height = new_item_height;

	max_visible_items = listbox->get_height() / item_height;
/*
	int height = listbox->get_height();
	listbox->set_height(max_visible_items * item_height + 8);
*/
}

void CL_ListBox_Generic::set_max_visible_items(int count)
{
	// TODO: Run some update here
	max_visible_items = count;

	listbox->set_height(max_visible_items * item_height + 8);
}

void CL_ListBox_Generic::set_top_item(int index, bool do_update_scrollbar)
{
	int maximum_offset = get_count() - get_max_visible_items();

	if (index < 0) index = 0;
	
	if (index >= maximum_offset)
		scroll_offset = maximum_offset;
	else
		scroll_offset = index;
	
	if(do_update_scrollbar)
		update_scrollbar();
}

void CL_ListBox_Generic::set_selected(int index, bool select)
{
	if(index < 0 || index >= get_count())
		return;

	items[index].selected = select;
}

void CL_ListBox_Generic::select_all(bool select)
{
	std::vector<CL_ListItem>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it).selected = select;
}

void CL_ListBox_Generic::invert_selection()
{
	std::vector<CL_ListItem>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it).selected = !((*it).selected);
}

void CL_ListBox_Generic::set_multi_selection(bool enable)
{
	multi_selection = enable;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ListBox_Generic::on_set_options(const CL_ComponentOptions &options)
{
	// Fetch items
	int item_count = options.count("item");
	for (int i = 0; i < item_count; i++)
		items.push_back(CL_ListItem(options.get_value("item", i)));

	// Calculate height if max_visible_items is used
	if(options.exists("max_visible_items"))
		max_visible_items = options.get_value_as_int("max_visible_items");
}

void CL_ListBox_Generic::on_mouse_down(const CL_Key &key)
{
	if(get_count() == 0)
		return;

	if(key.id == CL_MOUSE_WHEELUP)
	{
		set_top_item(get_top_item() - 5, true);
		return;
	}
	if(key.id == CL_MOUSE_WHEELDOWN)
	{
		set_top_item(get_top_item() + 5, true);
		return;
	}

	int index = (int)(key.y / item_height + scroll_offset);

	if(index < 0 || index >= get_count())
		return;

	if(is_multi_selection())
		items[index].selected = !items[index].selected;
	else
	{
		std::vector<CL_ListItem>::iterator it;
		for(it = items.begin(); it != items.end(); ++it)
			if((*it).selected)
				(*it).selected = false;
		items[index].selected = true;
	}

	sig_activated(index);
	sig_selection_changed();
}

void CL_ListBox_Generic::on_scroll_change(int new_offset)
{
	set_top_item(new_offset, false);
}

void CL_ListBox_Generic::on_child_add(CL_Component *child)
{
	CL_Rect rect_child = child->get_position();
	CL_Rect rect_clientarea = client_area->get_position();

	if(rect_child.x2 < rect_clientarea.get_width() / 2)
		rect_clientarea.x1 = rect_child.x2 + 3;			// Left side
	else
		rect_clientarea.x2 = rect_child.x1 - 3;			// Right side

//	if(rect_child.y2 < rect_clientarea.get_height() / 2)	// Top side
//		rect_clientarea.y1 = rect_child.y2;
//	else
//		rect_clientarea.y2 = rect_child.y1;

	client_area->set_position(rect_clientarea);
}

void CL_ListBox_Generic::on_child_remove(CL_Component *child)
{
	CL_Rect rect_child = child->get_position();
	CL_Rect rect_clientarea = client_area->get_position();

	if(rect_child.x2 < rect_clientarea.get_width() / 2)	
		rect_clientarea.x1 -= rect_child.get_width() + 2;	// Left side
	else
		rect_clientarea.x2 += rect_child.get_width() + 2;	// Right side

	client_area->set_position(rect_clientarea);
}

void CL_ListBox_Generic::on_resize(int old_width, int old_height)
{
	update_scrollbar();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ListBox_Generic::update_scrollbar()
{	
	if(get_count() > max_visible_items)
	{
		if(visible_scrollbar == false)
		{
			visible_scrollbar = true;
			listbox->add_child(scrollbar, true);
		}

		scrollbar->set_max_value(get_count() - max_visible_items);
		scrollbar->set_value(scroll_offset);
	}
	else
	{
		if(visible_scrollbar)
		{
			visible_scrollbar = false;
			scrollbar->set_value(0);
			listbox->remove_child(scrollbar);
		}
	}
}
