/*
	$Id: button_generic.cpp,v 1.35 2002/02/04 20:31:45 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "button_generic.h"
#include "API/GUI/component_options.h"
#include "API/Display/Display/surface.h"
#include "API/Display/Input/inputdevice.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Button_Generic::CL_Button_Generic(
	CL_Button *self,
	const std::string &text,
	bool toggle_mode)
:
	button(self),
	surface_up(NULL),
	surface_down(NULL),
	surface_highlighted(NULL),
	surface_disabled(NULL),
	text(text),
	toggled(false),
	toggle_mode(toggle_mode)
{
	slots.connect(button->sig_set_options(), this, &CL_Button_Generic::set_options);
	slots.connect(button->sig_input_down(), this, &CL_Button_Generic::on_input_down);
	slots.connect(button->sig_input_up(), this, &CL_Button_Generic::on_input_up);
	slots.connect(button->sig_mouse_enter(), this, &CL_Button_Generic::on_mouse_enter);
	slots.connect(button->sig_mouse_leave(), this, &CL_Button_Generic::on_mouse_leave);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Button_Generic::set_options(const CL_ComponentOptions &options)
{
	if(options.exists("text"))
		text = options.get_value("text");

	if(options.exists("toggled"))
		toggled = options.get_value_as_bool("toggled");

	if(options.exists("togglemode"))
		toggle_mode = options.get_value_as_bool("togglemode");
}

void CL_Button_Generic::on_input_down(CL_InputDevice *device, const CL_Key &key)
{
	if(button->is_mouse_captured())
		return;

	if(key.id != CL_MOUSE_LEFTBUTTON && key.id != CL_KEY_SPACE)
		return;

	toggled = !toggled;

	sig_pressed();
	sig_toggled(toggled);
	
	button->capture_mouse();
}

void CL_Button_Generic::on_input_up(CL_InputDevice *device, const CL_Key &key)
{
	if(button->is_mouse_captured() == false)
		return;

	if(key.id != CL_MOUSE_LEFTBUTTON && key.id != CL_KEY_SPACE)
		return;

	sig_released();

	if (button->has_mouse_over())
	{
		if(toggle_mode == false)
		{
			toggled = !toggled;
			sig_toggled(toggled);
		}
				
		sig_clicked();
	}
		
	button->release_mouse();
}

void CL_Button_Generic::on_mouse_enter()
{
	if(button->is_mouse_captured())
	{
		toggled = !toggled;
		sig_pressed();
	}
}

void CL_Button_Generic::on_mouse_leave()
{
	if(button->is_mouse_captured())
	{
		toggled = !toggled;
		sig_released();
	}
}
