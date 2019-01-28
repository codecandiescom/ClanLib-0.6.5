/*
	$Id: scrollbar_generic.h,v 1.26 2002/02/03 18:03:30 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_scrollbar_generic
#define header_scrollbar_generic

#include "API/Core/System/timer.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Input/key.h"
#include "API/signals.h"

class CL_ScrollBar;
class CL_Button;
class CL_InputDevice;
class CL_Component;
class CL_ComponentOptions;
class CL_StyleManager;

class CL_ScrollBar_Generic
{
// Construction:
public:
	CL_ScrollBar_Generic(
		CL_ScrollBar *self,
		int min,
		int max,
		int value,
		bool orientation,
		bool tracking);

	~CL_ScrollBar_Generic();

// Attributes:
public:
	int get_range() const;

	CL_Component *client_area;

	int min_value;
	int max_value;
	int cur_value;

	bool tracking;
	bool vertical;
	bool fixed_length;

	bool dragging;

	CL_Rect rect_slider;
	int min_slider_length;
	int fixed_slider_length;
			
// Operations:
public:
	void set_vertical(bool enable);
	void set_range(int min_value, int max_value);
	void set_min_value(int value);
	void set_max_value(int value);
	void set_value(int value, bool using_slider = false);

// Signals:
public:
	CL_Signal_v1<int> sig_value_changed;
	CL_Signal_v0 sig_slider_pressed;
	CL_Signal_v1<int> sig_slider_moved;
	CL_Signal_v0 sig_slider_released;

// Slots:
private:
	CL_SlotContainer slots;

// Callbacks:
private:
	void on_set_options(const CL_ComponentOptions &options);
	void on_child_add(CL_Component *child);
	void on_child_remove(CL_Component *child);
	void on_mouse_down(const CL_Key &key);
	void on_mouse_up(const CL_Key &key);
	void on_client_resize(int old_width, int old_height);
	void on_mouse_move(int x, int y);
	void on_timer_scroll();

// Implementation:
private:
	void calculate_slider();

	CL_ScrollBar *scrollbar;

	bool mouse_captured;
	int capture_last_offset;

	CL_Timer timer_scroll;
	int scroll_delta;

	bool initialized;
};

#endif
