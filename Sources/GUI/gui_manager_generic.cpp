/*
	$Id: gui_manager_generic.cpp,v 1.74 2002/03/02 18:42:25 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Display/Display/display.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/mouse.h"
#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputcursor.h"
#include "API/Display/Input/keyboard.h"
#include "API/Core/System/system.h"
#include "API/GUI/component.h"
#include "gui_manager_generic.h"
#include "component_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic construction:

CL_GUIManager_Generic::CL_GUIManager_Generic(CL_GUIManager *_owner, CL_Component *parent_component)
:
	owner(_owner),
	parent(NULL),
	input_enabled(true),
	current_tab_id(0),
	comp_focus(_owner),
	comp_capture_mouse(NULL),
	comp_mouse_over(NULL)
{
	owner->set_gui_manager(owner);

	if(parent_component)
	{
		parent = parent_component->get_gui_manager();
		if(parent)
			parent->disable_input();
	}

	slot_button_press = CL_Input::sig_button_press().connect(
		this, &CL_GUIManager_Generic::on_button_press);

	slot_button_release = CL_Input::sig_button_release().connect(
		this, &CL_GUIManager_Generic::on_button_release);

	slot_mouse_move = CL_Mouse::sig_move().connect(
		this, &CL_GUIManager_Generic::on_mouse_move);
}

CL_GUIManager_Generic::~CL_GUIManager_Generic()
{
	if (parent)
		parent->enable_input();

	disable_input();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic attributes:

CL_Component *CL_GUIManager_Generic::get_focus()
{
	return comp_focus;
}

CL_Component *CL_GUIManager_Generic::get_capture()
{
	return comp_capture_mouse;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic operations:

void CL_GUIManager_Generic::set_focus(CL_Component *component)
{
	if (comp_focus == component) return;

	CL_Component *old_comp_focus = comp_focus;

	comp_focus = component;
	old_comp_focus->sig_lost_focus()();
	comp_focus->sig_got_focus()();
}

void CL_GUIManager_Generic::quit()
{
	quit_run = true;
}

void CL_GUIManager_Generic::run()
{
	quit_run = false;
	while (!quit_run)
	{
		show();
			
		CL_Display::flip_display();
		CL_System::keep_alive();
		CL_System::sleep(10);
	}
}

void CL_GUIManager_Generic::show()
{
	if (parent)
		parent->show();

	owner->update();

/*
	// Remove all closed components
	bool new_focus = false;
	std::list<COMP_PAIR>::iterator it;
	for (it = components.begin(); it != components.end();)
	{
		CL_Component *cur_component = (*it).first;
		if(cur_component->is_removeflag_set())
		{
			it = components.erase(it);
			new_focus = true;
		}
		else
			it++;
	}
*/
}

void CL_GUIManager_Generic::enable_input()
{
	input_enabled = true;
}

void CL_GUIManager_Generic::disable_input()
{
	input_enabled = false;
}

void CL_GUIManager_Generic::gui_capture_mouse(CL_Component *component)
{
	comp_capture_mouse = component;
}

void CL_GUIManager_Generic::gui_release_mouse()
{
	comp_capture_mouse = NULL;
}

CL_Component *CL_GUIManager_Generic::find_focus_component(CL_InputDevice *device, const CL_Key &key)
{
	CL_Component *focus_component = comp_focus;

	if (comp_capture_mouse)
		focus_component = comp_capture_mouse;
	else if (device->get_type() == CL_InputDevice::type_mouse)
		focus_component = owner->get_component_at((int)(key.x - owner->get_position().x1),(int)( key.y - owner->get_position().y1));

	return focus_component;
}

void CL_GUIManager_Generic::on_button_press(
	CL_InputDevice *device,
	const CL_Key &key)
{
	if (!input_enabled) return;

	// Transform from screen coordinates to GUI coordinate.
	int mouse_x = (int) key.x;
	int mouse_y = (int) key.y;
	owner->sig_transform_coords()(mouse_x, mouse_y);

	CL_Key transformed_key = key;
	transformed_key.x = (float)mouse_x;
	transformed_key.y = (float)mouse_y;

	// Figure out what our focus component is (whom to send message):
	CL_Component *focus_component = find_focus_component(device, transformed_key);

	// If its a mouse click, make the clicked component get focus.
	if (device->get_type() == CL_InputDevice::type_mouse) set_focus(focus_component);

	// If its a tab click, pass on keyboard input to next component.
	if (key.id == CL_KEY_TAB)
	{
		if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT) || CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
			focus_component = tab_previous();
		else
			focus_component = tab_next();
	}

	// If component is disabled, do not send event:
	if (focus_component->is_enabled() == false || focus_component->is_visible() == false) return;

	// Make coordinates relative to focus component.
	transformed_key.x -= focus_component->get_screen_rect().x1;
	transformed_key.y -= focus_component->get_screen_rect().y1;

	// Send input down event:
	focus_component->sig_input_down()(device, transformed_key);

	// Send key down event:
	if(device->get_type() == CL_InputDevice::type_keyboard)
		focus_component->sig_key_down()(transformed_key);

	// Send mouse down event:
	if(device->get_type() == CL_InputDevice::type_mouse)
		focus_component->sig_mouse_down()(transformed_key);
}

void CL_GUIManager_Generic::on_button_release(
	CL_InputDevice *device,
	const CL_Key &key)
{
	if (!input_enabled) return;

	// Do not pass tab support down through the gui:
	if (key.id == CL_KEY_TAB) return;

	// Transform from screen coordinates to GUI coordinate.
	int mouse_x = (int) key.x;
	int mouse_y = (int) key.y;
	owner->sig_transform_coords()(mouse_x, mouse_y);

	CL_Key transformed_key = key;
	transformed_key.x = (float)mouse_x;
	transformed_key.y = (float)mouse_y;

	// Figure out what our focus component is (whom to send message):
	CL_Component *focus_component = find_focus_component(device, transformed_key);

	// Make coordinates relative to focus component.
	transformed_key.x -= focus_component->get_screen_rect().x1;
	transformed_key.y -= focus_component->get_screen_rect().y1;

	// Send input up event:
	focus_component->sig_input_up()(device, transformed_key);

	// Send key up event:
	if(device->get_type() == CL_InputDevice::type_keyboard)
		focus_component->sig_key_up()(transformed_key);

	// Send mouse up event:
	if(device->get_type() == CL_InputDevice::type_mouse)
		focus_component->sig_mouse_up()(transformed_key);
}

void CL_GUIManager_Generic::on_mouse_move(int x, int y)
{
	if (!input_enabled) return;

	owner->sig_transform_coords()(x, y);

	CL_Component *focus_component = comp_capture_mouse;
	if (!comp_capture_mouse)
	{
		focus_component = owner->get_component_at(
			x - owner->get_position().x1,
			y - owner->get_position().y1);
	}

	// Make coordinates relative to focus component.
	x -= focus_component->get_screen_rect().x1;
	y -= focus_component->get_screen_rect().y1;

	// Send mouse enter and mouse leave signals:
	do_mouse_enter_leave(x, y, focus_component);

	focus_component->sig_mouse_move()(x, y);
}

void CL_GUIManager_Generic::do_mouse_enter_leave(int x, int y, CL_Component *new_focus)
{
	CL_Component *old_focus = comp_mouse_over;
	if (x < 0 || y < 0 || x >= new_focus->get_width() || y >= new_focus->get_height()) new_focus = 0;

	// No signals needed if same component kept focus.
	if (old_focus == new_focus) return;

	if (old_focus) old_focus->sig_mouse_leave()();
	if (new_focus) new_focus->sig_mouse_enter()();

	comp_mouse_over = new_focus;
}

CL_Component *CL_GUIManager_Generic::tab_next()
{
	// First try to find component by tab id:
	CL_Component *tab_component = owner->find_tab_id(++current_tab_id);
	if (tab_component == 0) // reached end, start from beginning
	{
		current_tab_id = 0;
		tab_component = owner->find_tab_id(current_tab_id);
	}
	if (tab_component)
	{
		set_focus(tab_component);
		return comp_focus;
	}

	// No tab ids - fall back to tree walking:
	if (comp_focus->get_children().empty()) // tab to sibling or uncle
	{
		CL_Component *cur = comp_focus;

		while (true)
		{
			CL_Component *parent = cur->get_parent();
			if (parent == 0) break;

			// Search siblings:
			std::list<CL_Component *>::const_iterator it;
			for (it = parent->get_children().begin(); it != parent->get_children().end(); it++)
			{
				if (*it == cur)
				{
					it++;

					// no more siblings
					if (it == parent->get_children().end()) break;

					// give sibling focus
					set_focus(*it);
					return comp_focus;
				}
			}

			// Was last sibling, continue search in parent (uncles)
			cur = parent;
		}

		// if we are top node (no siblings), and got no children:
		if (cur == comp_focus) return comp_focus;

		// walked through all components. Restart at first component:
		set_focus(owner);
		return comp_focus;
	}
	else // tab to children
	{
		// give first child focus
		set_focus(comp_focus->get_children().front());
		return comp_focus;
	}
}

CL_Component *CL_GUIManager_Generic::tab_previous()
{
	// First try to find component by tab id:
	CL_Component *tab_component = owner->find_tab_id(--current_tab_id);
	if (current_tab_id == -1) // reached beginning, start from end
	{
		current_tab_id = find_highest_tab_id(owner);
		tab_component = owner->find_tab_id(current_tab_id);
	}
	if (tab_component)
	{
		set_focus(tab_component);
		return comp_focus;
	}

	// No tab ids - fall back to tree walking:
	CL_Component *cur = comp_focus;
	while (true)
	{
		CL_Component *parent = cur->get_parent();
		if (parent == 0) break;

		// Search siblings:
		std::list<CL_Component *>::const_iterator it;
		for (it = parent->get_children().begin(); it != parent->get_children().end(); it++)
		{
			if (*it == cur)
			{
				// no more siblings
				if (it == parent->get_children().begin()) break;

				it--;

				// give sibling focus
				set_focus(*it);
				return comp_focus;
			}
		}

		// Was last sibling, continue search in parent (uncles)
		cur = parent;
	}

	// walked through all components. Restart at end component:
	cur = owner;
	while (!cur->get_children().empty()) cur = cur->get_children().back();
	set_focus(cur);
	return comp_focus;
}

#define cl_max(a,b) ((a > b) ? a : b)

int CL_GUIManager_Generic::find_highest_tab_id(const CL_Component *component)
{
	int id = cl_max(0, component->get_tab_id());
	const std::list<CL_Component *> &children = component->get_children();
	std::list<CL_Component *>::const_iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		id = cl_max(id, find_highest_tab_id(*it));
	}
	return id;
}
