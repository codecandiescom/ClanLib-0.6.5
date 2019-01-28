/*
	$Id: input.cpp,v 1.8 2002/08/11 16:58:06 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/inputdevice.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/keyboard.h>
#include <API/Display/Input/mouse.h>

std::vector<CL_Keyboard*> CL_Input::keyboards;
std::vector<CL_InputDevice*> CL_Input::joysticks;
std::vector<CL_InputDevice*> CL_Input::pointers;

CL_Signal_v2<CL_InputDevice *, const CL_Key&> CL_Input::signal_button_press;
CL_Signal_v2<CL_InputDevice *, const CL_Key&> CL_Input::signal_button_release;
CL_Signal_v3<CL_InputDevice *, int, int> CL_Input::signal_mouse_move;
CL_Signal_v2<int, int> CL_Mouse::signal_move;
CL_Signal_v1<const CL_Key &> CL_Mouse::signal_button_press;
CL_Signal_v1<const CL_Key &> CL_Mouse::signal_button_release;

CL_Signal_v2<CL_InputDevice *, const CL_Key &> &CL_Input::sig_button_press()
{
	return CL_Input::signal_button_press;
}

CL_Signal_v2<CL_InputDevice *, const CL_Key &> &CL_Input::sig_button_release()
{
	return CL_Input::signal_button_release;
}

CL_Signal_v3<CL_InputDevice *, int, int> &CL_Input::sig_mouse_move()
{
	return CL_Input::signal_mouse_move;
}

CL_Signal_v2<int, int> &CL_Mouse::sig_move()
{
	return CL_Mouse::signal_move;
}

CL_Signal_v1<const CL_Key &> &CL_Mouse::sig_button_press()
{
	return CL_Mouse::signal_button_press;
}

CL_Signal_v1<const CL_Key &> &CL_Mouse::sig_button_release()
{
	return CL_Mouse::signal_button_release;
}

bool CL_Keyboard::get_keycode(int button_no)
{
	return CL_Input::keyboards[0]->get_button(button_no)->is_pressed();
}

int CL_Mouse::get_x()
{
	return (int) CL_Input::pointers[0]->get_cursor(0)->get_x();
}

int CL_Mouse::get_y()
{
	return (int) CL_Input::pointers[0]->get_cursor(0)->get_y();
}

void CL_Mouse::set_position(float x, float y)
{
	CL_Input::pointers[0]->get_cursor(0)->set_position(x, y);
}

bool CL_Mouse::left_pressed()
{
	return CL_Input::pointers[0]->get_button(0)->is_pressed();
}

bool CL_Mouse::middle_pressed()
{
	return CL_Input::pointers[0]->get_button(1)->is_pressed();
}

bool CL_Mouse::right_pressed()
{
	return CL_Input::pointers[0]->get_button(2)->is_pressed();
}
