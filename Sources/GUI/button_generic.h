/*
	$Id: button_generic.h,v 1.20 2002/02/03 18:03:30 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_button_generic
#define header_button_generic

#include "API/GUI/button.h"

class CL_Button_Generic
{
public:
	CL_Button_Generic(
		CL_Button *self,
		const std::string &text,
		bool toggle_mode);

	~CL_Button_Generic() {};

	CL_Button *button;

	void set_options(const CL_ComponentOptions &options);
	void on_input_down(CL_InputDevice *device, const CL_Key &key);
	void on_input_up(CL_InputDevice *device, const CL_Key &key);
	void on_mouse_enter();
	void on_mouse_leave();

	CL_Surface *surface_up;
	CL_Surface *surface_down;
	CL_Surface *surface_highlighted;
	CL_Surface *surface_disabled;

	std::string text;
	bool toggled;
	bool toggle_mode;

	CL_Signal_v1<bool> sig_toggled;
	CL_Signal_v0 sig_clicked;
	CL_Signal_v0 sig_pressed;
	CL_Signal_v0 sig_released;

	CL_SlotContainer slots;
};  

#endif
