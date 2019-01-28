/*
	$Id: listbox.cpp,v 1.53 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/listbox.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "listbox_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ListBox::CL_ListBox(CL_Component *parent, CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ListBox_Generic(this);
	get_style_manager()->connect_styles("listbox", this);
	find_preferred_size();
}

CL_ListBox::CL_ListBox(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ListBox_Generic(this);
	get_style_manager()->connect_styles("listbox", this);
}

CL_ListBox::CL_ListBox(
	const CL_Rect &pos,
	int max_visible_items,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ListBox_Generic(this, max_visible_items);
	get_style_manager()->connect_styles("listbox", this);
}

CL_ListBox::~CL_ListBox()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_ListBox::get_client_area() const
{
	return impl->client_area;
}

int CL_ListBox::get_count() const
{
	return impl->get_count();
}

std::vector<CL_ListItem> &CL_ListBox::get_items() const
{
	return impl->get_items();
}

std::vector<std::string> CL_ListBox::get_selected_items() const
{
	return impl->get_selected_items();
}

const std::string &CL_ListBox::get_current_text() const
{
	return impl->get_current_text();
}

const std::string &CL_ListBox::get_text(int index) const
{
	return impl->get_text(index);
}

int CL_ListBox::get_current_item() const
{
	return impl->get_current_item();
}

bool CL_ListBox::is_selected(int index) const
{
	return impl->is_selected(index);
}

bool CL_ListBox::is_multi_selection() const
{
	return impl->is_multi_selection();
}

int CL_ListBox::get_item_height() const
{
	return impl->get_item_height();
}

int CL_ListBox::get_max_visible_items() const
{
	return impl->get_max_visible_items();
}

int CL_ListBox::get_top_item() const
{
	return impl->get_top_item();
}

bool CL_ListBox::has_scrollbar() const
{
	return impl->has_scrollbar();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_ListBox::insert_item(const std::string &text, int index)
{
	return impl->insert_item(text, index);
}

void CL_ListBox::remove_item(int index)
{
	impl->remove_item(index);
}

void CL_ListBox::change_item(const std::string &text, int index)
{
	impl->change_item(text, index);
}

void CL_ListBox::set_current_item(int index)
{
	impl->set_current_item(index);
}

void CL_ListBox::set_selected(int index, bool select)
{
	impl->set_selected(index, select);
}

void CL_ListBox::clear_selection()
{
	impl->clear_selection();
}

void CL_ListBox::select_all(bool select)
{
	impl->select_all(select);
}

void CL_ListBox::invert_selection()
{
	impl->invert_selection();
}

void CL_ListBox::set_multi_selection(bool enable)
{
	impl->set_multi_selection(enable);
}

void CL_ListBox::set_item_height(int height)
{
	impl->set_item_height(height);
}

void CL_ListBox::set_max_visible_items(int count)
{
	impl->set_max_visible_items(count);
}

void CL_ListBox::set_top_item(int index)
{
	impl->set_top_item(index, true);
}

void CL_ListBox::sort(bool ascending)
{
	impl->sort(ascending);
}

void CL_ListBox::clear()
{
	impl->clear();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<int> &CL_ListBox::sig_highlighted()
{
	return impl->sig_highlighted;
}

CL_Signal_v1<int> &CL_ListBox::sig_activated()
{
	return impl->sig_activated;
}

CL_Signal_v0 &CL_ListBox::sig_selection_changed()
{
	return impl->sig_selection_changed;
}
