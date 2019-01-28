/*
	$Id: input_joystick.cpp,v 1.4 2001/12/22 17:03:34 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>
#include <windows.h>
#include "input_joystick.h"

/*****************************
		 CL_Joystick_Win32
*****************************/

CL_Joystick_Win32::CL_Joystick_Win32(int _joystick_id)
{
	joystick_id = _joystick_id;

	MMRESULT err=joyGetDevCaps(joystick_id, &joycaps, sizeof(joycaps));
	if (err != JOYERR_NOERROR)
	{
		std::cout << "ClanLib: Failed to initialize joystick #" << joystick_id+1 << std::endl;
		return;
	}
}

CL_Joystick_Win32::~CL_Joystick_Win32()
{
}

char *CL_Joystick_Win32::get_name() const
{
//	return joycaps.szOEMVxD;
	return (char *) joycaps.szPname;
}

CL_InputDevice::InputDeviceType CL_Joystick_Win32::get_type() const
{
	return CL_InputDevice::type_joystick;
}

int CL_Joystick_Win32::get_num_buttons() const
{
	return joycaps.wMaxButtons;
}

CL_InputButton *CL_Joystick_Win32::get_button(int button_num)
{
	return &buttons[button_num];
}

int CL_Joystick_Win32::get_num_axes() const
{
	return joycaps.wMaxAxes;
}

CL_InputAxis *CL_Joystick_Win32::get_axis(int axis_num)
{
	return &axes[axis_num];
}

int CL_Joystick_Win32::get_num_hats() const
{
	return 1;
}

CL_InputHat *CL_Joystick_Win32::get_hat(int /*hat_num*/)
{
	return &hat;
}

int CL_Joystick_Win32::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_Joystick_Win32::get_cursor(int /*cursor_num*/)
{
	return NULL;
}

void CL_Joystick_Win32::keep_alive()
{
	JOYINFOEX joyinfo;
	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL;

	MMRESULT err = joyGetPosEx(joystick_id, &joyinfo);
	if (err != JOYERR_NOERROR)
	{
		static bool message_printed = false;

		if (!message_printed)
		{
			std::cout << "ClanLib: Joystick polling failed (further messages suppressed)" << std::endl;
			message_printed = true;
		}
	}

	for (unsigned int button=0; button<joycaps.wMaxButtons; button++)
	{
		buttons[button].set_state(((joyinfo.dwButtons>>button)&1) ? true : false);
	}

	axes[0].set_pos(calc_pos(joyinfo.dwXpos, joycaps.wXmin, joycaps.wXmax));
	axes[1].set_pos(calc_pos(joyinfo.dwYpos, joycaps.wYmin, joycaps.wYmax));

	if (joycaps.wCaps&JOYCAPS_HASZ) axes[2].set_pos(calc_pos(joyinfo.dwZpos, joycaps.wZmin, joycaps.wZmax));
	if (joycaps.wCaps&JOYCAPS_HASR) axes[3].set_pos(calc_pos(joyinfo.dwRpos, joycaps.wRmin, joycaps.wRmax));
	if (joycaps.wCaps&JOYCAPS_HASU) axes[4].set_pos(calc_pos(joyinfo.dwUpos, joycaps.wUmin, joycaps.wUmax));
	if (joycaps.wCaps&JOYCAPS_HASV) axes[5].set_pos(calc_pos(joyinfo.dwVpos, joycaps.wVmin, joycaps.wVmax));

	if (joyinfo.dwPOV==JOY_POVCENTERED) hat.set_dir(-1);
	else hat.set_dir(float(joyinfo.dwPOV)/100);
}

float CL_Joystick_Win32::calc_pos(int pos, int min, int max)
{
	pos-=min;
	max-=min;

	int logical_pos=pos*13/max-6;

	if (logical_pos>0) logical_pos--;
	if (logical_pos<0) logical_pos++;

	if (logical_pos<-5) logical_pos=-5;
	if (logical_pos>5) logical_pos=5;

	return logical_pos/(float) 5;
}
