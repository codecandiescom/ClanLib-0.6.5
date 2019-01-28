/*
	$Id: scrollbar_generic.cpp,v 1.45 2002/03/02 18:42:25 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "scrollbar_generic.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/component.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/button.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/mouse.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:
/*
CL_ComponentOptions CL_ScrollBar_Generic::create_options(
	const CL_Rect &pos,
	int min,
	int max,
	int value,
	bool orientation,
	bool tracking)
{
	CL_ComponentOptions options;

	options.add_option("x", pos.x1);
	options.add_option("y", pos.y1);
	options.add_option("width", pos.get_width());
	options.add_option("height", pos.get_height());
	options.add_option("min", min);
	options.add_option("max", max);
	options.add_option("value", value);
	options.add_option("orientation", orientation);
	options.add_option("tracking", tracking);

	return options;
}
*/
CL_ScrollBar_Generic::CL_ScrollBar_Generic(
	CL_ScrollBar *self,
	int min,
	int max,
	int value,
	bool orientation,
	bool tracking)
:
	min_value(0),
	max_value(0),
	cur_value(0),
	tracking(tracking),
	fixed_length(false),
	dragging(false),
	scrollbar(self),
	capture_last_offset(0),
	timer_scroll(250),
	initialized(false)
{
	vertical = !orientation;
	set_min_value(min);
	set_max_value(max);
	set_value(value);

	if(vertical)
		min_slider_length = scrollbar->get_height() / 6;
	else
		min_slider_length = scrollbar->get_width() / 6;
	if(min_slider_length < 20)
		min_slider_length = 20;

	CL_Rect rect(0, 0, scrollbar->get_width(), scrollbar->get_height());
	client_area = new CL_Component(rect, scrollbar);

	slots.connect(scrollbar->sig_set_options(), this, &CL_ScrollBar_Generic::on_set_options);
	slots.connect(client_area->sig_mouse_move(), this, &CL_ScrollBar_Generic::on_mouse_move);
	slots.connect(client_area->sig_mouse_down(), this, &CL_ScrollBar_Generic::on_mouse_down);
	slots.connect(client_area->sig_mouse_up(), this, &CL_ScrollBar_Generic::on_mouse_up);
	slots.connect(client_area->sig_resize(), this, &CL_ScrollBar_Generic::on_client_resize);

	slots.connect(timer_scroll.sig_timer(), this, &CL_ScrollBar_Generic::on_timer_scroll);

	initialized = true;
}

CL_ScrollBar_Generic::~CL_ScrollBar_Generic()
{
	delete client_area;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ScrollBar_Generic::get_range() const
{
	return max_value - min_value + 1;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ScrollBar_Generic::set_vertical(bool enable)
{
	throw CL_Error("CL_ScrollBar_Generic::set_vertical() is not implemented");
}

void CL_ScrollBar_Generic::set_range(int new_min_value, int new_max_value)
{
	min_value = new_min_value;
	max_value = new_max_value;

	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value < min_value)
		cur_value = min_value;
	if(cur_value > max_value)
		cur_value = max_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_min_value(int value)
{
	min_value = value;
	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value < min_value)
		cur_value = min_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_max_value(int value)
{
	max_value = value;
	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value > max_value)
		cur_value = max_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_value(int value, bool using_slider)
{
	if(value < min_value) value = min_value;
	if(value > max_value) value = max_value;

	if(cur_value != value)
	{
		cur_value = value;

		if(using_slider)
			sig_slider_moved(cur_value);

		if((using_slider && tracking) || using_slider == false)
			sig_value_changed(cur_value);
	}

	if(using_slider == false)
		calculate_slider();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ScrollBar_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("orientation"))
	{
		CL_String s(options.get_value("orientation"));
		s.to_lower();
		if(s == "hor" || s == "horizontal" || s == "1")
			vertical = false;
		else if(s == "ver" || s == "vertical" || s == "0")
			vertical = true;
		else
			throw CL_Error("Invalid scrollbar orientation value");
	}
	else
		vertical = false;

	if(options.exists("min"))
		set_min_value(options.get_value_as_int("min"));
	else
		set_min_value(0);

	if(options.exists("max"))
		set_max_value(options.get_value_as_int("max"));
	else
		set_max_value(min_value);

	if(options.exists("value"))
		set_value(options.get_value_as_int("value"));
	else
		set_value(min_value);
}

void CL_ScrollBar_Generic::on_client_resize(int old_width, int old_height)
{
	calculate_slider();
}

void CL_ScrollBar_Generic::on_mouse_down(const CL_Key &key)
{
	if(key.id == CL_MOUSE_WHEELUP)
	{
		set_value(cur_value - 5);
		return;
	}
	if(key.id == CL_MOUSE_WHEELDOWN)
	{
		set_value(cur_value + 5);
		return;
	}
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
		if(dragging == false)
		{
			if(key.x > rect_slider.x1 && 
				key.y > rect_slider.y1 && 
				key.x < rect_slider.x2 && 
				key.y < rect_slider.y2)
			{
				dragging = true;
				client_area->capture_mouse();
				if(vertical) 
					capture_last_offset = (int) key.y;
				else
					capture_last_offset = (int) key.x;
				sig_slider_pressed();
			}
			else
			{
				int scrollbar_length;
				int slider_length;

				scroll_delta = 1;

				if(vertical)
				{
					scrollbar_length = client_area->get_height();
					slider_length = rect_slider.get_height();

					if(key.y < rect_slider.y1)
						scroll_delta = -1;
				}
				else
				{
					scrollbar_length = client_area->get_width();
					slider_length = rect_slider.get_width();

					if(key.x < rect_slider.x1)
						scroll_delta = -1;
				}

				float percent = slider_length / (float)(scrollbar_length - slider_length);
				scroll_delta *= (int)( get_range() * percent);

				on_timer_scroll();
				timer_scroll.enable();
				client_area->capture_mouse();
			}
		}
	}
}

void CL_ScrollBar_Generic::on_mouse_up(const CL_Key &key)
{
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
		if(dragging)
		{
			client_area->release_mouse();
			dragging = false;

			set_value(cur_value);

			if(tracking == false)
				sig_value_changed(cur_value);
			sig_slider_released();
		}
		else if(timer_scroll.is_enabled())
		{
			timer_scroll.disable();
			client_area->release_mouse();
		}
	}
}

void CL_ScrollBar_Generic::on_mouse_move(int x, int y)
{
	if(dragging == false)
		return;

	if(vertical)
	{
//		y -= client_area->get_position().y1;

		int delta = y - capture_last_offset;

		int scrollbar_length = client_area->get_height();
		int slider_length = rect_slider.get_height();

		rect_slider.y1 += delta;

		if(rect_slider.y1 < 0)
			rect_slider.y1 = 0;
		else if(rect_slider.y1 + slider_length > scrollbar_length)
			rect_slider.y1 = scrollbar_length - slider_length;
		else
			capture_last_offset = y;

		rect_slider.y2 = rect_slider.y1 + slider_length;

		float percent = (float)rect_slider.y1 / (scrollbar_length - slider_length);
		float value = min_value + (get_range() * percent);
		set_value((int)value, true);
	} 
	else
	{
//		x -= client_area->get_position().x1;

		int delta = x - capture_last_offset;

		int scrollbar_length = client_area->get_width();
		int slider_length = rect_slider.get_width();

		rect_slider.x1 += delta;

		if(rect_slider.x1 < 0)
			rect_slider.x1 = 0;
		else if(rect_slider.x1 + slider_length > scrollbar_length)
			rect_slider.x1 = scrollbar_length - slider_length;
		else capture_last_offset = x;

		rect_slider.x2 = rect_slider.x1 + slider_length;

		float percent = (float)rect_slider.x1 / (scrollbar_length - slider_length);
		float value = min_value + (get_range() * percent);
		set_value((int)value, true);
	}
}

void CL_ScrollBar_Generic::on_timer_scroll()
{
	set_value(cur_value + scroll_delta);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ScrollBar_Generic::calculate_slider()
{
	if(initialized == false)
		return;

	int slider_offset;
	int slider_length;

	if(fixed_length)
	{
		throw CL_Error("fixed-length sliders not implemented");
	}
	else {
		int scrollbar_length;
		if(vertical)
			scrollbar_length = scrollbar->get_client_area()->get_height();
		else
			scrollbar_length = scrollbar->get_client_area()->get_width();
		
		int range = get_range();

		slider_length = scrollbar_length / range;
		if(slider_length < min_slider_length)
			slider_length = min_slider_length;

		float y = 0.0f;
		if(range > 1)
		{
			int available_area = scrollbar_length - slider_length;
			y = (float)available_area / (range - 1);
		}

		slider_offset = (int) (y * (cur_value - min_value));
	}

	if(vertical)
	{
		rect_slider.y1 = slider_offset;
		rect_slider.y2 = slider_offset + slider_length;
		rect_slider.x1 = 0;
		rect_slider.x2 = scrollbar->get_client_area()->get_width();
	}
	else
	{
		rect_slider.x1 = slider_offset;
		rect_slider.x2 = slider_offset + slider_length;
		rect_slider.y1 = 0;
		rect_slider.y2 = scrollbar->get_client_area()->get_height();
	}
}


