/*
	$Id: scrollbar_default.cpp,v 1.43 2002/02/01 14:44:06 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "scrollbar_default.h"
#include "button_default.h"
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"

CL_ScrollBar_Default::CL_ScrollBar_Default(
	CL_ScrollBar *_scrollbar,
	CL_StyleManager_Default *style)
:
	CL_ComponentStyle(_scrollbar),
	timer_scroll(250),
	scrollbar(_scrollbar)
{
	this->style = style;
	resources = style->get_resources();

	CL_Component *client_area = scrollbar->get_client_area();

	button_increase = new CL_Button("", scrollbar, style);
	button_decrease = new CL_Button("", scrollbar, style);

	slots.connect(client_area->sig_paint(), this, &CL_ScrollBar_Default::on_paint);
	slots.connect(timer_scroll.sig_timer(), this, &CL_ScrollBar_Default::on_timer_scroll);

	slots.connect(button_increase->sig_pressed(), this, &CL_ScrollBar_Default::start_scroll, 1);
	slots.connect(button_decrease->sig_pressed(), this, &CL_ScrollBar_Default::start_scroll, -1);
	slots.connect(button_increase->sig_released(), this, &CL_ScrollBar_Default::stop_scroll);
	slots.connect(button_decrease->sig_released(), this, &CL_ScrollBar_Default::stop_scroll);
	slots.connect(scrollbar->sig_resize(), this, &CL_ScrollBar_Default::on_resize);

	on_resize(0, 0);
}

CL_ScrollBar_Default::~CL_ScrollBar_Default()
{
	delete button_increase;
	delete button_decrease;
}

void CL_ScrollBar_Default::start_scroll(int delta)
{
	scroll_delta = delta;
	timer_scroll.enable();
	on_timer_scroll();
}

void CL_ScrollBar_Default::stop_scroll()
{
	timer_scroll.disable();
}

void CL_ScrollBar_Default::on_resize(int old_width, int old_height)
{
	CL_Rect rect_decrease;
	CL_Rect rect_increase;
	CL_Rect rect_client;

	if (scrollbar->is_vertical())
	{
		int button_size = scrollbar->get_width();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(0, scrollbar->get_height() - button_size, button_size, scrollbar->get_height());
		rect_client = CL_Rect(0, button_size, button_size, scrollbar->get_height() - button_size);
	}
	else
	{
		int button_size = scrollbar->get_height();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(scrollbar->get_width() - button_size, 0, scrollbar->get_width(), button_size);
		rect_client = CL_Rect(button_size, 0, scrollbar->get_width() - button_size, button_size);
	}

	button_decrease->set_position(rect_decrease);
	button_increase->set_position(rect_increase);
	scrollbar->get_client_area()->set_position(rect_client);
}

void CL_ScrollBar_Default::on_timer_scroll()
{
	scrollbar->set_value(scrollbar->get_value() + scroll_delta);
}

void CL_ScrollBar_Default::on_paint()
{
	int width = scrollbar->get_client_area()->get_width();
	int height = scrollbar->get_client_area()->get_height();

	CL_Rect rect = scrollbar->get_slider_rect();

	// Background
	// TODO: Split this into two parts (at the sides of the slider)
	style->fill_rect(
		0, 0,
		width, height,
		GUICOLOR_SCROLLBAR);

	if(scrollbar->has_focus())
		style->draw_rect(
			0, 0,
			width, height,
			GUICOLOR_FOCUS);
		
	// Slider
	if(scrollbar->is_dragging_slider())
	{
		style->fill_rect(
			rect.x1 + 2,
			rect.y1 + 2,
			rect.x2 - 2,
			rect.y2 - 2,
			GUICOLOR_DARKER_SHADE);
	}
	else
	{
		style->fill_rect(
			rect.x1 + 2,
			rect.y1 + 2,
			rect.x2 - 2,
			rect.y2 - 2,
			GUICOLOR_WHITE);
	}

	style->draw_rect(
		rect.x1 + 1,
		rect.y1 + 1,
		rect.x2 - 1,
		rect.y2 - 1, 
		GUICOLOR_DARK_OUTLINE);
}
