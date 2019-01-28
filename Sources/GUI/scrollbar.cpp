/*
	$Id: scrollbar.cpp,v 1.44 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "scrollbar_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ScrollBar construction:

CL_ScrollBar::CL_ScrollBar(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, false, false);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	int min,
	int max,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, min, max, 0, orientation, false);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, orientation, false);
	get_style_manager()->connect_styles("scrollbar", this);
	find_preferred_size();
}

CL_ScrollBar::CL_ScrollBar(
	const CL_Rect &pos,
	int min,
	int max,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, min, max, 0, orientation, false);
	get_style_manager()->connect_styles("scrollbar", this);
}

CL_ScrollBar::CL_ScrollBar(
	const CL_Rect &pos,
	bool orientation,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ScrollBar_Generic(this, 0, 100, 0, orientation, false);
	get_style_manager()->connect_styles("scrollbar", this);
}

CL_ScrollBar::~CL_ScrollBar()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_ScrollBar::get_client_area() const
{
	return impl->client_area;
}

bool CL_ScrollBar::is_vertical() const
{
	return impl->vertical;
}

bool CL_ScrollBar::is_tracking() const
{
	return impl->tracking;
}

int CL_ScrollBar::get_min_value() const
{
	return impl->min_value;
}

int CL_ScrollBar::get_max_value() const
{
	return impl->max_value;
}

int CL_ScrollBar::get_value() const
{
	return impl->cur_value;
}

int CL_ScrollBar::get_range() const
{
	return impl->get_range();
}

bool CL_ScrollBar::is_dragging_slider() const
{
	return impl->dragging;
}

CL_Rect &CL_ScrollBar::get_slider_rect() const
{
	return impl->rect_slider;
}

bool CL_ScrollBar::is_fixed_length_slider() const
{
	return impl->fixed_length;
}

int CL_ScrollBar::get_min_slider_length() const
{
	return impl->min_slider_length;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ScrollBar::set_vertical(bool enable)
{
	impl->set_vertical(enable);
}

void CL_ScrollBar::set_tracking(bool enable)
{
	impl->tracking = enable;
}

void CL_ScrollBar::set_range(int min_value, int max_value)
{
	impl->set_range(min_value, max_value);
}

void CL_ScrollBar::set_min_value(int value)
{
	impl->set_min_value(value);
}

void CL_ScrollBar::set_max_value(int value)
{
	impl->set_max_value(value);
}

void CL_ScrollBar::set_value(int value)
{
	impl->set_value(value);
}

void CL_ScrollBar::increase(int step)
{
	impl->set_value(impl->cur_value + step);
}

void CL_ScrollBar::decrease(int step)
{
	impl->set_value(impl->cur_value - step);
}

void CL_ScrollBar::set_fixed_length_mode(bool enable)
{
	impl->fixed_length = enable;
}

void CL_ScrollBar::set_slider_length(int length)
{
	impl->fixed_slider_length = length;
}

void CL_ScrollBar::set_min_slider_length(int length)
{
	impl->min_slider_length = length;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<int> &CL_ScrollBar::sig_value_changed()
{
	return impl->sig_value_changed;
}

CL_Signal_v0 &CL_ScrollBar::sig_slider_pressed()
{
	return impl->sig_slider_pressed;
}

CL_Signal_v1<int> &CL_ScrollBar::sig_slider_moved()
{
	return impl->sig_slider_moved;
}

CL_Signal_v0 &CL_ScrollBar::sig_slider_released()
{
	return impl->sig_slider_released;
}

