/*
	$Id: input_dx.cpp,v 1.4 2002/03/18 06:31:56 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"

#include "input_dx.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_joystick.h"

#include <dinput.h>

LPDIRECTINPUT CL_Input_DX::dinput = NULL;

CL_Input_DX::CL_Input_DX()
{
	HRESULT err = DirectInputCreate(CL_System_Win32::hInstance, 0x0300, &dinput, NULL);
	cl_assert(err == DI_OK);

	CL_Input::keyboards.push_back(new CL_Keyboard_Win32);
	CL_Input::pointers.push_back(new CL_Mouse_Win32);

	int num_devs = joyGetNumDevs();
	for (int i=0; i<num_devs; i++)
	{
		JOYCAPS joycaps;
		JOYINFO joyinfo; 
		MMRESULT err = joyGetPos(i, &joyinfo);
		if (err != JOYERR_UNPLUGGED) 
		{
			err = joyGetDevCaps(i, &joycaps, sizeof(joycaps));
			if (err == JOYERR_NOERROR) 
				CL_Input::joysticks.push_back(new CL_Joystick_Win32(i));
		}
	}
}

CL_Input_DX::~CL_Input_DX()
{
	dinput->Release();

	for (std::vector<CL_Keyboard*>::iterator k_it=CL_Input::keyboards.begin();
	k_it!=CL_Input::keyboards.end();k_it++)
	{
		delete *k_it;
	}

	for (std::vector<CL_InputDevice*>::iterator m_it=CL_Input::pointers.begin();
	m_it!=CL_Input::pointers.end();m_it++)
	{
		delete *m_it;
	}

	for (std::vector<CL_InputDevice*>::iterator j_it=CL_Input::joysticks.begin();
	j_it!=CL_Input::joysticks.end();j_it++)
	{
		delete *j_it;
	}
}
