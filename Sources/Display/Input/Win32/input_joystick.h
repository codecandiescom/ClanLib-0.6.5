/*
	$Id: input_joystick.h,v 1.2 2001/11/30 22:38:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_input_joystick
#define header_input_joystick

#include "API/Display/Input/input.h"
#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Display/Input/inputhat.h"
#include "API/Display/Input/inputdevice.h"
#include "API/Core/System/keep_alive.h"

class CL_InputButton_Joystick : public CL_InputButton
{
protected:
	bool button_down;

public:
	CL_InputButton_Joystick() { button_down = false; }
	virtual bool is_pressed() { return button_down; }
	void set_state(bool new_state) { button_down = new_state; }
};

class CL_InputAxis_Joystick : public CL_InputAxis
{
protected:
	float pos;

public:
	CL_InputAxis_Joystick() { pos = 0; }
	virtual float get_pos() { return pos; }
	void set_pos(float new_pos) { pos = new_pos; }
};

class CL_InputHat_Joystick : public CL_InputHat
{
protected:
	float dir;

public:
	CL_InputHat_Joystick() { dir = -1; }
	virtual float get_direction() { return dir; }
	void set_dir(float new_dir) { dir = new_dir; }
};

class CL_Joystick_Win32 : public CL_InputDevice, public CL_KeepAlive
{
protected:
	int joystick_id;							// Joystick identifier
	JOYCAPS joycaps;							// Struct describing capabilities of the joystick

	CL_InputAxis_Joystick axes[6];
	CL_InputButton_Joystick buttons[32];
	CL_InputHat_Joystick hat;

	float calc_pos(int pos, int min, int max);

public:
	CL_Joystick_Win32(int joystick_id);
	virtual ~CL_Joystick_Win32();

	// from CL_InputDevice:
	virtual char *get_name() const;

	virtual InputDeviceType get_type() const;

	virtual int get_num_buttons() const;
	virtual CL_InputButton *get_button(int button_num);

	virtual int get_num_axes() const;
	virtual CL_InputAxis *get_axis(int axis_num);

	virtual int get_num_hats() const;
	virtual CL_InputHat *get_hat(int hat_num);

	virtual int get_num_cursors() const;
	virtual CL_InputCursor *get_cursor(int cursor_num);

	virtual void keep_alive();
};

#endif
