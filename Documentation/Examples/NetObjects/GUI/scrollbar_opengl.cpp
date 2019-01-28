#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "scrollbar_opengl.h"
#include "stylemanager_opengl.h"

CL_ScrollBar_OpenGL::CL_ScrollBar_OpenGL(
	CL_ScrollBar *_scrollbar,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(_scrollbar),
	timer_scroll(250),
	scrollbar(_scrollbar)
{
	this->style = style;
	resources = style->get_resources();

	CL_Rect rect_decrease;
	CL_Rect rect_increase;

	CL_Component *client_area = scrollbar->get_client_area();

	if (scrollbar->is_vertical())
	{
		int button_size = scrollbar->get_width();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(0, scrollbar->get_height() - button_size, button_size, scrollbar->get_height());
	}
	else
	{
		int button_size = scrollbar->get_height();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(scrollbar->get_width() - button_size, 0, scrollbar->get_width(), button_size);
	}

	button_increase = new CL_Button(rect_increase, "", scrollbar, style);
	button_decrease = new CL_Button(rect_decrease, "", scrollbar, style);

	layout.add_resize_position(button_decrease, CL_LayoutManager::x2, scrollbar, CL_LayoutManager::x2);

	layout.add_resize_position(button_increase, CL_LayoutManager::x2, scrollbar, CL_LayoutManager::x2);
	layout.add_resize_position(button_increase, CL_LayoutManager::y1, scrollbar, CL_LayoutManager::y2);
	layout.add_resize_position(button_increase, CL_LayoutManager::y2, scrollbar, CL_LayoutManager::y2);

	slot_paint = client_area->sig_paint().connect(
		this, &CL_ScrollBar_OpenGL::on_paint);

	slot_increase_pressed = button_increase->sig_pressed().connect(
		this, &CL_ScrollBar_OpenGL::start_scroll, 1);

	slot_decrease_pressed = button_decrease->sig_pressed().connect(
		this, &CL_ScrollBar_OpenGL::start_scroll, -1);

	slot_increase_released = button_increase->sig_released().connect(
		this, &CL_ScrollBar_OpenGL::stop_scroll);

	slot_decrease_released = button_decrease->sig_released().connect(
		this, &CL_ScrollBar_OpenGL::stop_scroll);

	slot_timer = timer_scroll.sig_timer().connect(
		this, &CL_ScrollBar_OpenGL::on_timer_scroll);
}

CL_ScrollBar_OpenGL::~CL_ScrollBar_OpenGL()
{
}

void CL_ScrollBar_OpenGL::start_scroll(int delta)
{
	scroll_delta = delta;
	timer_scroll.enable();
	on_timer_scroll();
}

void CL_ScrollBar_OpenGL::stop_scroll()
{
	timer_scroll.disable();
}

void CL_ScrollBar_OpenGL::on_timer_scroll()
{
	scrollbar->set_value(scrollbar->get_value() + scroll_delta);
}

void CL_ScrollBar_OpenGL::on_paint()
{
	int width = scrollbar->get_client_area()->get_width();
	int height = scrollbar->get_client_area()->get_height();

	CL_Rect rect = scrollbar->get_slider_rect();

	// Background
	CL_Display::fill_rect(
		0, 0,
		width, height,
		0.5f, 0.5f, 0.5f);

	// Slider
	if(scrollbar->is_dragging_slider())
	{
		CL_Display::fill_rect(
			rect.x1 + 2,
			rect.y1 + 2,
			rect.x2 - 2,
			rect.y2 - 2,
			0.7f, 0.7f, 0.7f);
	}
	else
	{
		CL_Display::fill_rect(
			rect.x1 + 2,
			rect.y1 + 2,
			rect.x2 - 2,
			rect.y2 - 2,
			0.9f, 0.9f, 0.9f);
	}

	CL_Display::draw_rect(
		rect.x1 + 1,
		rect.y1 + 1,
		rect.x2 - 1,
		rect.y2 - 1, 
		0.2f, 0.2f, 0.2f);
}
