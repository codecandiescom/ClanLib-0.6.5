/*
	$Id: button.cpp,v 1.61 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/button.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "button_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Button::CL_Button(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, std::string(), false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
}

CL_Button::CL_Button(
	const CL_Point &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const CL_Point &pos,
	const std::string &text,
	CL_Surface *surface_up,
	CL_Surface *surface_down,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	set_surface_up(surface_up);
	set_surface_down(surface_down);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::CL_Button(
	const CL_Point &pos,
	const std::string &text,
	CL_Surface *surface_up,
	CL_Surface *surface_down,
	CL_Surface *surface_disabled,
	CL_Surface *surface_highlighted,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(CL_Rect(pos.x, pos.y, 0, 0), parent, style), impl(0)
{
	impl = new CL_Button_Generic(this, text, false);
	set_surface_up(surface_up);
	set_surface_down(surface_down);
	set_surface_highlighted(surface_highlighted);
	set_surface_disabled(surface_disabled);
	get_style_manager()->connect_styles("button", this);
	find_preferred_size();
}

CL_Button::~CL_Button()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_Button::get_text() const
{
	return impl->text;
}

bool CL_Button::is_toggle_button() const
{
	return impl->toggle_mode;
}

bool CL_Button::is_down() const
{
	return impl->toggled;
}

CL_Surface *CL_Button::get_surface_up() const
{
	return impl->surface_up;
}

CL_Surface *CL_Button::get_surface_down() const
{
	return impl->surface_down;
}

CL_Surface *CL_Button::get_surface_highlighted() const
{
	return impl->surface_highlighted;
}

CL_Surface *CL_Button::get_surface_disabled() const
{
	return impl->surface_disabled;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Button::set_text(const std::string &text)
{
	impl->text = text;
}

void CL_Button::set_toggle_mode(bool toggle)
{
	impl->toggle_mode = toggle;
}

void CL_Button::set_down(bool enable)
{
	impl->toggled = enable;
}

void CL_Button::toggle()
{
	impl->toggled = !impl->toggled;
	impl->sig_toggled(impl->toggled);
}

void CL_Button::set_surface_up(CL_Surface *surface)
{
	impl->surface_up = surface;
}

void CL_Button::set_surface_down(CL_Surface *surface)
{
	impl->surface_down = surface;
}

void CL_Button::set_surface_highlighted(CL_Surface *surface)
{
	impl->surface_highlighted = surface;
}

void CL_Button::set_surface_disabled(CL_Surface *surface)
{
	impl->surface_disabled = surface;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<bool> &CL_Button::sig_toggled()
{
	return impl->sig_toggled;
}

CL_Signal_v0 &CL_Button::sig_clicked()
{
	return impl->sig_clicked;
}

CL_Signal_v0 &CL_Button::sig_pressed()
{
	return impl->sig_pressed;
}

CL_Signal_v0 &CL_Button::sig_released()
{
	return impl->sig_released;
}

