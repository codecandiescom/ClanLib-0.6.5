/*
	$Id: component.h,v 1.93 2002/02/03 18:03:30 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component
#define header_component

#include <string>

#include "../signals.h"
#include "../Core/Math/rect.h"
#include "../Core/Math/point.h"
#include "../Display/Input/key.h"

class CL_GUIManager;
class CL_ComponentStyle;
class CL_InputDevice;
class CL_ComponentOptions;
class CL_StyleManager;
class CL_Component_Generic;

//: Component base class.
// The component class is the base class for any component/widget/control (or
// whatever you call it :)) in the GUI system. It is responsible for painting
// the component, painting of any child components of it, and it receives the
// input for a component.
class CL_Component
{
//! Construction:
public:
	//: Component Constructor
	CL_Component(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Component Constructor
	CL_Component(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Component Destructor
	virtual ~CL_Component();

//! Attributes:
public:
	//: Returns the parent component, or NULL if none.
	CL_Component *get_parent() const;

	//: Returns the root parent.
	CL_Component *get_root_parent();

	//: Returns the GUI manager that this component is attached to, or NULL if none.
	CL_GUIManager *get_gui_manager() const;

	//: Returns the style manager used by this component.
	CL_StyleManager *get_style_manager() const;

	//: Returns the width of the component.
	int get_width() const;

	//: Returns the height of the component.
	int get_height() const;

	//: Returns the components position in screen coordinates.
	CL_Rect get_screen_rect() const;

	//: Returns the position of this component.
	const CL_Rect &get_position() const;

	//: Returns true if the passed component is a child of this component.
	bool has_child(CL_Component *component) const;

	//: Returns the list of child components.
	const std::list<CL_Component *> &get_children() const;

	//: Returns the bounding rectangle of the component's children.
	CL_Rect get_children_rect();

	//: Returns the component at (pos_x, pos_y).
	//: This functions searches for children recursively.
	//: NULL is returned if the point is outside the component.
	//: The component itself is returned if the component has no child at the specified point.
	CL_Component *get_component_at(int pos_x, int pos_y);

	//: Returns the component that currently has the focus.
	CL_Component *get_focus() const;

	//: Returns true if component has the focus.
	bool has_focus() const;

	//: Returns true if the mouse is currently highlighting the component.
	bool has_mouse_over() const;

	//: Returns true if component is visible.
	bool is_visible() const;

	//: Returns true if component accepts user input.
	bool is_enabled() const;

	//: Returns true if component is a popup window.
	bool is_popup() const;

	//: Returns true if component has mouse captured.
	bool is_mouse_captured() const;

	//: I have no idea what this does... -- mbn 3. okt 2000.
	bool is_removeflag_set() const;

	//: Returns the current mouse position, relative to the component.
	CL_Point get_mouse_position() const;

	//: Returns the tab id of the component.
	int get_tab_id() const;

//! Operations:
public:
	//: Set the tab id of the component.
	void set_tab_id(int id);

	//: Sets the position (relative to its parent) and size of this component.
	void set_position(const CL_Rect &new_pos);

	//: Sets the position of this component (relative to its parent).
	void set_position(int new_x, int new_y);

	//: Sets the size of this component.
	void set_size(int new_width, int new_height);

	//: Changes the width of the component.
	void set_width(int width);

	//: Changes the height of the component.
	void set_height(int height);

	//: Sets the parent of this component.
	void set_parent(CL_Component *parent);

	//: Sets the GUI manager this component is attached to.
	void set_gui_manager(CL_GUIManager *gui);

	//: Changes the visibility flag.
	void show(bool show = true);

	//: Enable/Disable user input.
	void enable(bool enable = true);

	//: Change popup flag.
	void popup(bool popup);

	//: Give focus to this component.
	void set_focus();

	//: Raises this component to the top of the parent component's stack. 
	void raise();

	//: Lowers this component to the bottom of the parent component's stack.
	void lower();

	//: Capture the mouse on the currently attached GUI manager. All mouse input
	//: will be directed to this component until release_mouse() is called.
	void capture_mouse();

	//: Releases the mouse capture.
	void release_mouse();

	//: Calls sig_begin_paint(), then sig_paint(), and finally sig_end_paint().
	void update();

	//: Run the component as a modal component until it returns
	void run(CL_Component *parent = NULL);

	//: Quit the modal loop of the component
	void quit();

	//: "Closes" this component.
	//: Hides and detaches component from parent.
	void close();

	//: Clears the remove flag... dunno what the remove flag means. -- mbn 3. okt 2000
	void clear_removeflag();

	//: Adds the specified component as a children to this component.
	//: If 'delete_child' is true, the child will be deleted when the component is deleted.
	void add_child(CL_Component *child, bool delete_child = false);

	//: Removes the specified child from the component.
	void remove_child(CL_Component *child);

	//: Add all components in a component manager.
	void add_components(class CL_ComponentManager *component_manager);

	//: Attaches a component style to the component.
	void attach_style(CL_ComponentStyle *style);

	//: Removes a component style to the component.
	void detach_style(CL_ComponentStyle *style);

	//: Calculate a default size
	void find_preferred_size();

	//: Returns the component with the specified tab ID.
	CL_Component *find_tab_id(int tab_id);

//! Signals:
public:
	//: Update component with a new set of component options.
	CL_Signal_v1<const CL_ComponentOptions &> &sig_set_options();

	//: <p>Draws the component. Must be called in a
	//: begin_paint() / end_paint() session.</p>
	CL_Signal_v0 &sig_paint();

	//: <p>Called prior to component drawing. Sets up a cliprect and translocates
	//: coordinates accordingly to this component.</p>
	CL_Signal_v0 &sig_begin_paint();

	//: Called after component drawing, and draws all the children.
	CL_Signal_v0 &sig_paint_children();

	//: <p>Cleans up when component drawing is finished (cleans up cliprects and
	//: translocation stuff).</p>
	CL_Signal_v0 &sig_end_paint();

	//: Called when any inputdevice button is pressed (keyboard, mouse, joystick, etc).
	CL_Signal_v2<CL_InputDevice *, const CL_Key &> &sig_input_down();

	//: Called when any inputdevice button is released (keyboard, mouse, joystick, etc)
	CL_Signal_v2<CL_InputDevice *, const CL_Key &> &sig_input_up();

	//: Called when a key is pressed on keyboard.
	CL_Signal_v1<const CL_Key &> &sig_key_down();

	//: Called when a key is released on keyboard.
	CL_Signal_v1<const CL_Key &> &sig_key_up();

	//: Called when a mousebutton is clicked.
	//: The CL_Key contains the coordinates and which button was pressed
	CL_Signal_v1<const CL_Key &> &sig_mouse_down();

	//: Called when a mousebutton is released.
	CL_Signal_v1<const CL_Key &> &sig_mouse_up();

	//: Called when the mouse is moved.
	CL_Signal_v2<int, int> &sig_mouse_move();

	//: Called when mouse enters the component area.
	CL_Signal_v0 &sig_mouse_enter();

	//: Called when mouse leaves the component area.
	CL_Signal_v0 &sig_mouse_leave();

	//: Called when component gains focus.
	CL_Signal_v0 &sig_got_focus();

	//: Called when component lose focus.
	CL_Signal_v0 &sig_lost_focus();

	//: Called when component is resized.
	//: The two parameters are the OLD width and height of the component.
	CL_Signal_v2<int, int> &sig_resize();

	//: Called when component is moved.
	//: The two parameters are the OLD positions of the component.
	CL_Signal_v2<int, int> &sig_move();

	//: Called when a child component is about to be added to the component.
	CL_Signal_v1<CL_Component *> &sig_child_before_add();

	//: Called when a child component is added to the component.
	CL_Signal_v1<CL_Component *> &sig_child_add();

	//: Called when a child component is about to be removed from the component.
	CL_Signal_v1<CL_Component *> &sig_child_before_remove();
	
	//: Called when a child component is removed from the component.
	CL_Signal_v1<CL_Component *> &sig_child_remove();

	//: Called when needing to convert parent coordinates into coordinates used by this component.
	//: <p>This is used in 3D guis to convert from screen coordinates to gui coordinates.
	//: (project the screen cordinates down on the plane that the gui is laying on)</p>
	CL_Signal_v2<int &, int &> &sig_transform_coords();
	
	//: Called when needing to find the minimum size of the component.
	//: Normally used by the themes.
	CL_Signal_v1<CL_Point &> &sig_get_minimum_size();

	//: Called when needing to find the maximum size of the component.
	//: Normally used by the themes.
	CL_Signal_v1<CL_Point &> &sig_get_maximum_size();

	//: Called when needing to find the preferred size of the component.
	//: Normally used by the themes.
	CL_Signal_v1<CL_Point &> &sig_get_preferred_size();

//! Implementation:
protected:
	CL_Component_Generic *impl;

	friend class CL_Component_Generic;
};

#endif
