/*
	$Id: gui_manager_generic.h,v 1.33 2002/01/30 13:17:16 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_guimanager_generic
#define header_guimanager_generic

#include "API/GUI/gui_manager.h"

class CL_InputDevice;
class CL_Key;

class CL_GUIManager_Generic
{
// Construction:
public:
	CL_GUIManager_Generic(CL_GUIManager *owner, CL_Component *parent = NULL);
	~CL_GUIManager_Generic();

// Attributes:
public:
	CL_Component *get_focus();
	CL_Component *get_capture();

// Operations:
public:
	void run();
	void show();
	void quit();

	void enable_input();
	void disable_input();
	
	void gui_capture_mouse(CL_Component *component);
	void gui_release_mouse();

	void set_focus(CL_Component *component);
	
// Implementation:
private:
	void on_button_press(CL_InputDevice *, const CL_Key&);
	void on_button_release(CL_InputDevice *, const CL_Key&);
	void on_mouse_move(int, int);
	CL_Component *find_focus_component(CL_InputDevice *device, const CL_Key &key);
	void do_mouse_enter_leave(int x, int y, CL_Component *new_focus);
	CL_Component *tab_next();
	CL_Component *tab_previous();
	int find_highest_tab_id(const CL_Component *component);

	CL_Slot slot_button_press;
	CL_Slot slot_button_release;
	CL_Slot slot_mouse_move;

	volatile bool quit_run;
	CL_GUIManager *owner;
	CL_GUIManager *parent;
	bool input_enabled;

	int current_tab_id;
	CL_Component *comp_focus;
	CL_Component *comp_capture_mouse;
	CL_Component *comp_mouse_over;
};

#endif
