/*
	$Id: component_generic.cpp,v 1.45 2002/01/30 13:17:16 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Display/Display/display.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"
#include "API/GUI/component_style.h"
#include "API/GUI/component_options.h"
#include "component_generic.h"

CL_Component_Generic::CL_Component_Generic(
	CL_Component *_owner,
	const CL_Rect &pos,
	CL_Component *_parent,
	CL_StyleManager *style)
:
	owner(_owner),
	parent(_parent),
	style_manager(style),
	position(pos),
	gui(NULL),
	visible(true),
	enabled(true),
	popup(false),
	removeflag(false),
	tab_id(-1)
{
	if (style_manager == NULL && parent)
		style_manager = parent->get_style_manager();

	slot_set_options = sig_set_options.connect(
		this, &CL_Component_Generic::set_options);
	slot_begin_paint = sig_begin_paint.connect(
		this, &CL_Component_Generic::begin_paint);
	slot_paint_children = sig_paint_children.connect(
		this, &CL_Component_Generic::paint_children);
	slot_end_paint = sig_end_paint.connect(
		this, &CL_Component_Generic::end_paint);
}

CL_Component_Generic::~CL_Component_Generic()
{
	for (
		std::list<CL_ComponentStyle *>::iterator itStyle = styles.begin();
		itStyle != styles.end();
		itStyle++)
	{
		delete (*itStyle);
	}

	for (
		std::list<CL_Component *>::iterator it = delete_children.begin();
		it != delete_children.end();
		it++)
	{
		delete (*it);
	}
}

void CL_Component_Generic::set_options(const CL_ComponentOptions &options)
{
	int x = 0, y = 0, width = 0, height = 0;

	if (options.exists("x"))      x = options.get_value_as_int("x");
	if (options.exists("y"))      y = options.get_value_as_int("y");
	if (options.exists("width"))  width = options.get_value_as_int("width");
	if (options.exists("height")) height = options.get_value_as_int("height");

	owner->set_position(CL_Rect(x, y, x + width, y + height));

	if (options.exists("visible")) owner->show(options.get_value_as_bool("visible"));
	if (options.exists("enabled")) owner->enable(options.get_value_as_bool("enabled"));

	if (options.exists("tab_id")) owner->set_tab_id(options.get_value_as_int("tab_id"));
}

void CL_Component_Generic::find_preferred_size()
{
//	if(position.x2 == 0 && position.y2 == 0)
//	if(position.x1 == position.x2 || position.y1 == position.y2)
	{
		CL_Point size;
		owner->sig_get_preferred_size()(size);
		owner->set_size(size.x, size.y);
	}
}

void CL_Component_Generic::begin_paint()
{
	CL_Display::push_translate_offset(position.x1, position.y1);
	CL_Rect r = owner->get_screen_rect();
	CL_Display::push_clip_rect(CL_ClipRect(r.x1, r.y1, r.x2, r.y2));
}

void CL_Component_Generic::paint_children()
{
	// we draw the children back to front because then all non-focus input
	// will be routed to the topmost window.
	for (
		std::list<CL_Component *>::iterator it = children.end();
		it != children.begin();
		)
	{
		--it;

		if((*it)->is_visible()) (*it)->update();
	}
}

void CL_Component_Generic::end_paint()
{
	CL_Display::pop_clip_rect();
	CL_Display::pop_translate_offset();
}
