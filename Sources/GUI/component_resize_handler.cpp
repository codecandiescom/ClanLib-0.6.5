/*
	$Id: component_resize_handler.cpp,v 1.11 2002/02/04 20:31:45 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/component_resize_handler.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"

class CL_ComponentResizeHandler_Generic
{
public:
	CL_ComponentResizeHandler_Generic()
	: owner(NULL), client_area(NULL)
	{
	}

	void on_mouse_down(const CL_Key &key);
	void on_mouse_up(const CL_Key &key);
	void on_mouse_move(int x, int y);

	CL_Slot slot_mouse_down;
	CL_Slot slot_mouse_up;
	CL_Slot slot_mouse_move;

	bool is_active;
	CL_Point mouse_origin;

	CL_Component *owner;
	CL_Component *client_area;
};

CL_ComponentResizeHandler::CL_ComponentResizeHandler(
	const CL_Rect &resize_area,
	CL_Component *owner,
	CL_StyleManager *style)
:
	impl(new CL_ComponentResizeHandler_Generic)
{
	impl->owner = owner;
	impl->is_active = false;

	impl->client_area = new CL_Component(resize_area, impl->owner, style);

	impl->slot_mouse_down = impl->client_area->sig_mouse_down().connect(
		impl, &CL_ComponentResizeHandler_Generic::on_mouse_down);

	impl->slot_mouse_up = impl->client_area->sig_mouse_up().connect(
		impl, &CL_ComponentResizeHandler_Generic::on_mouse_up);

	impl->slot_mouse_move = impl->client_area->sig_mouse_move().connect(
		impl, &CL_ComponentResizeHandler_Generic::on_mouse_move);
}

CL_ComponentResizeHandler::~CL_ComponentResizeHandler()
{
	if (impl->client_area != NULL)
	{
		impl->owner->remove_child(impl->client_area);
		delete impl->client_area;
	}
	delete impl;
}

void CL_ComponentResizeHandler_Generic::on_mouse_down(const CL_Key &key)
{
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
		mouse_origin = CL_Point((int)key.x, (int)key.y);
		is_active = true;
		client_area->capture_mouse();
	}
}

void CL_ComponentResizeHandler_Generic::on_mouse_up(const CL_Key &key)
{
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
		is_active = false;
		client_area->release_mouse();
	}
}

void CL_ComponentResizeHandler_Generic::on_mouse_move(int x, int y)
{
	if(is_active == false)
		return;

	int delta_x = x - mouse_origin.x;
	int delta_y = y - mouse_origin.y;

	owner->set_size(owner->get_width() + delta_x, owner->get_height() + delta_y);

	CL_Rect rect = client_area->get_position();

	client_area->set_position(rect.x1 + delta_x, rect.y1 + delta_y);
}
