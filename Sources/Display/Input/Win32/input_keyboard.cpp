/*
	$Id: input_keyboard.cpp,v 1.7 2002/02/20 10:53:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <windows.h>
#include "API/Core/System/cl_assert.h"
#include "input_keyboard.h"

/*************************
		CL_Keyboard_Win32
*************************/

CL_Keyboard_Win32::CL_Keyboard_Win32()
{
	buttons = new CL_InputButton_Keyboard_Win32*[CL_NUM_KEYS];
	for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;

	memset(keys_down, 0, 256);

	CL_System_Win32::add_listener(this);
}

CL_Keyboard_Win32::~CL_Keyboard_Win32()
{
	CL_System_Win32::remove_listener(this);

	for (int i=0; i<CL_NUM_KEYS; i++) delete buttons[i];
	delete[] buttons;
}

void CL_Keyboard_Win32::keep_alive()
{
	GetKeyboardState(keys_down);
}

bool CL_Keyboard_Win32::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool keydown = false;
	switch (uMsg)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			keydown = true;
			break;
		
		case WM_KEYUP:
		case WM_SYSKEYUP:
			keydown = false;
			break;

		default:
			return false;
	}

	static bool lctrl_down = false;
	static bool rctrl_down = false;
	static bool lshift_down = false;
	static bool rshift_down = false;
	static bool alt_down = false;
	static bool altgr_down = false;

	int key = -1;
	switch ((int) wParam)
	{
	case 'A': key = CL_KEY_A; break;
	case 'B': key = CL_KEY_B; break;
	case 'C': key = CL_KEY_C; break;
	case 'D': key = CL_KEY_D; break;
	case 'E': key = CL_KEY_E; break;
	case 'F': key = CL_KEY_F; break;
	case 'G': key = CL_KEY_G; break;
	case 'H': key = CL_KEY_H; break;
	case 'I': key = CL_KEY_I; break;
	case 'J': key = CL_KEY_J; break;
	case 'K': key = CL_KEY_K; break;
	case 'L': key = CL_KEY_L; break;
	case 'M': key = CL_KEY_M; break;
	case 'N': key = CL_KEY_N; break;
	case 'O': key = CL_KEY_O; break;
	case 'P': key = CL_KEY_P; break;
	case 'Q': key = CL_KEY_Q; break;
	case 'R': key = CL_KEY_R; break;
	case 'S': key = CL_KEY_S; break;
	case 'T': key = CL_KEY_T; break;
	case 'U': key = CL_KEY_U; break;
	case 'V': key = CL_KEY_V; break;
	case 'W': key = CL_KEY_W; break;
	case 'X': key = CL_KEY_X; break;
	case 'Y': key = CL_KEY_Y; break;
	case 'Z': key = CL_KEY_Z; break;
	case '0': key = CL_KEY_0; break;
	case '1': key = CL_KEY_1; break;
	case '2': key = CL_KEY_2; break;
	case '3': key = CL_KEY_3; break;
	case '4': key = CL_KEY_4; break;
	case '5': key = CL_KEY_5; break;
	case '6': key = CL_KEY_6; break;
	case '7': key = CL_KEY_7; break;
	case '8': key = CL_KEY_8; break;
	case '9': key = CL_KEY_9; break;
	case VK_NUMPAD0: key = CL_KEY_KP_0; break;
	case VK_NUMPAD1: key = CL_KEY_KP_1; break;
	case VK_NUMPAD2: key = CL_KEY_KP_2; break;
	case VK_NUMPAD3: key = CL_KEY_KP_3; break;
	case VK_NUMPAD4: key = CL_KEY_KP_4; break;
	case VK_NUMPAD5: key = CL_KEY_KP_5; break;
	case VK_NUMPAD6: key = CL_KEY_KP_6; break;
	case VK_NUMPAD7: key = CL_KEY_KP_7; break;
	case VK_NUMPAD8: key = CL_KEY_KP_8; break;
	case VK_NUMPAD9: key = CL_KEY_KP_9; break;
	case VK_F1: key = CL_KEY_F1; break;
	case VK_F2: key = CL_KEY_F2; break;
	case VK_F3: key = CL_KEY_F3; break;
	case VK_F4: key = CL_KEY_F4; break;
	case VK_F5: key = CL_KEY_F5; break;
	case VK_F6: key = CL_KEY_F6; break;
	case VK_F7: key = CL_KEY_F7; break;
	case VK_F8: key = CL_KEY_F8; break;
	case VK_F9: key = CL_KEY_F9; break;
	case VK_F10: key = CL_KEY_F10; break;
	case VK_F11: key = CL_KEY_F11; break;
	case VK_F12: key = CL_KEY_F12; break;
	case VK_ESCAPE: key = CL_KEY_ESCAPE; break;
	case VK_LEFT: key = CL_KEY_LEFT; break;
	case VK_RIGHT: key = CL_KEY_RIGHT; break;
	case VK_UP: key = CL_KEY_UP; break;
	case VK_DOWN: key = CL_KEY_DOWN; break;
	case VK_CONTROL:
		if (keydown)
		{
			int scancode = (lParam & 0xff0000) >> 16;
			bool extended_key = (lParam & 0x1000000) != 0;

			if (!lctrl_down && scancode == 29 && !extended_key)
			{
				key = CL_KEY_LCTRL;
				lctrl_down = true;
			}
			else if (!rctrl_down && scancode == 29 && extended_key)
			{
				key = CL_KEY_RCTRL;
				rctrl_down = true;
			}
		}
		else
		{
			int scancode = (lParam & 0xff0000) >> 16;
			bool extended_key = (lParam & 0x1000000) != 0;

			if (lctrl_down && scancode == 29 && !extended_key) 
			{
				key = CL_KEY_LCTRL;
				lctrl_down = false;
			}
			else if (rctrl_down && scancode == 29 && extended_key) 
			{
				key = CL_KEY_RCTRL;
				rctrl_down = false;
			}
		}
		break;
	case VK_SHIFT:
		if (keydown)
		{
			int scancode = (lParam & 0xff0000) >> 16;

			if (!lshift_down && scancode == 42)
			{
				key = CL_KEY_LSHIFT;
				lshift_down = true;
			}
			else if (!rshift_down && scancode == 54)
			{
				key = CL_KEY_RSHIFT;
				rshift_down = true;
			}
		}
		else
		{
			int scancode = (lParam & 0xff0000) >> 16;

			if (lshift_down && scancode == 42) 
			{
				key = CL_KEY_LSHIFT;
				lshift_down = false;
			}
			else if (rshift_down && scancode == 54)
			{
				key = CL_KEY_RSHIFT;
				rshift_down = false;
			}
		}
		break;
	case VK_MENU:
		if (keydown)
		{
			int scancode = (lParam & 0xff0000) >> 16;
			bool extended_key = (lParam & 0x1000000) != 0;

			if (!alt_down && scancode == 56 && !extended_key) 
			{
				key = CL_KEY_ALT;
				alt_down = true;
			}
			else if (!altgr_down && scancode == 56 && extended_key)
			{
				key = CL_KEY_ALTGR;
				altgr_down = true;
			}
		}
		else
		{
			int scancode = (lParam & 0xff0000) >> 16;
			bool extended_key = (lParam & 0x1000000) != 0;

			if (alt_down && scancode == 56 && !extended_key) 
			{
				key = CL_KEY_ALT;
				alt_down = false;
			}
			else if (altgr_down && scancode == 56 && extended_key)
			{
				key = CL_KEY_ALTGR;
				altgr_down = false;
			}
		}
		break;
	case VK_TAB: key = CL_KEY_TAB; break;
	case VK_RETURN: key = CL_KEY_ENTER; break;
	case VK_SPACE: key = CL_KEY_SPACE; break;
	case VK_BACK: key = CL_KEY_BACKSPACE; break;
	case VK_INSERT: key = CL_KEY_INSERT; break;
	case VK_DELETE: key = CL_KEY_DELETE; break;
	case VK_HOME: key = CL_KEY_HOME; break;
	case VK_END: key = CL_KEY_END; break;
	case VK_PRIOR: key = CL_KEY_PAGEUP; break;
	case VK_NEXT: key = CL_KEY_PAGEDOWN; break;
	case VK_CAPITAL: key = CL_KEY_CAPSLOCK; break;
	case VK_NUMLOCK: key = CL_KEY_NUMLOCK; break;
	case VK_SCROLL: key = CL_KEY_SCRLOCK; break;
	case VK_PRINT: key = CL_KEY_PRINT; break;
	case VK_PAUSE: key = CL_KEY_PAUSE; break;
	case VK_DIVIDE: key = CL_KEY_KP_DIV; break;
	case VK_MULTIPLY: key = CL_KEY_KP_MULT; break;
	case VK_ADD: key = CL_KEY_KP_PLUS; break;
	case VK_SUBTRACT: key = CL_KEY_KP_MINUS; break;
	case VK_DECIMAL: key = CL_KEY_KP_DECIMAL; break;
	}
	
	CL_Key k;
	k.id = key;
	char buf[4];
	k.ascii = -1;
	int result = ToAscii((UINT) wParam, MapVirtualKey((UINT) wParam, 0), keys_down, (LPWORD) buf, 0);
	if (result > 0)
	{
		k.ascii = buf[result-1];
	}
	k.state = keydown ? CL_Key::Pressed : CL_Key::Released;

	if (keydown) CL_Input::sig_button_press()(this, k);
	else CL_Input::sig_button_release()(this, k);

	return true;
}

CL_InputButton *CL_Keyboard_Win32::map_button(int button_num)
{
	int key = -1;
	switch (button_num)
	{
	case CL_KEY_A: key = 'A'; break;
	case CL_KEY_B: key = 'B'; break;
	case CL_KEY_C: key = 'C'; break;
	case CL_KEY_D: key = 'D'; break;
	case CL_KEY_E: key = 'E'; break;
	case CL_KEY_F: key = 'F'; break;
	case CL_KEY_G: key = 'G'; break;
	case CL_KEY_H: key = 'H'; break;
	case CL_KEY_I: key = 'I'; break;
	case CL_KEY_J: key = 'J'; break;
	case CL_KEY_K: key = 'K'; break;
	case CL_KEY_L: key = 'L'; break;
	case CL_KEY_M: key = 'M'; break;
	case CL_KEY_N: key = 'N'; break;
	case CL_KEY_O: key = 'O'; break;
	case CL_KEY_P: key = 'P'; break;
	case CL_KEY_Q: key = 'Q'; break;
	case CL_KEY_R: key = 'R'; break;
	case CL_KEY_S: key = 'S'; break;
	case CL_KEY_T: key = 'T'; break;
	case CL_KEY_U: key = 'U'; break;
	case CL_KEY_V: key = 'V'; break;
	case CL_KEY_W: key = 'W'; break;
	case CL_KEY_X: key = 'X'; break;
	case CL_KEY_Y: key = 'Y'; break;
	case CL_KEY_Z: key = 'Z'; break;
	case CL_KEY_0: key = '0'; break;
	case CL_KEY_1: key = '1'; break;
	case CL_KEY_2: key = '2'; break;
	case CL_KEY_3: key = '3'; break;
	case CL_KEY_4: key = '4'; break;
	case CL_KEY_5: key = '5'; break;
	case CL_KEY_6: key = '6'; break;
	case CL_KEY_7: key = '7'; break;
	case CL_KEY_8: key = '8'; break;
	case CL_KEY_9: key = '9'; break;
	case CL_KEY_KP_0: key = VK_NUMPAD0; break;
	case CL_KEY_KP_1: key = VK_NUMPAD1; break;
	case CL_KEY_KP_2: key = VK_NUMPAD2; break;
	case CL_KEY_KP_3: key = VK_NUMPAD3; break;
	case CL_KEY_KP_4: key = VK_NUMPAD4; break;
	case CL_KEY_KP_5: key = VK_NUMPAD5; break;
	case CL_KEY_KP_6: key = VK_NUMPAD6; break;
	case CL_KEY_KP_7: key = VK_NUMPAD7; break;
	case CL_KEY_KP_8: key = VK_NUMPAD8; break;
	case CL_KEY_KP_9: key = VK_NUMPAD9; break;
	case CL_KEY_F1: key = VK_F1; break;
	case CL_KEY_F2: key = VK_F2; break;
	case CL_KEY_F3: key = VK_F3; break;
	case CL_KEY_F4: key = VK_F4; break;
	case CL_KEY_F5: key = VK_F5; break;
	case CL_KEY_F6: key = VK_F6; break;
	case CL_KEY_F7: key = VK_F7; break;
	case CL_KEY_F8: key = VK_F8; break;
	case CL_KEY_F9: key = VK_F9; break;
	case CL_KEY_F10: key = VK_F10; break;
	case CL_KEY_F11: key = VK_F11; break;
	case CL_KEY_F12: key = VK_F12; break;
	case CL_KEY_ESCAPE: key = VK_ESCAPE; break;
	case CL_KEY_LEFT: key = VK_LEFT; break;
	case CL_KEY_RIGHT: key = VK_RIGHT; break;
	case CL_KEY_UP: key = VK_UP; break;
	case CL_KEY_DOWN: key = VK_DOWN; break;
	case CL_KEY_LCTRL: key = VK_LCONTROL; break;
	case CL_KEY_RCTRL: key = VK_RCONTROL; break;
	case CL_KEY_LSHIFT: key = VK_LSHIFT; break;
	case CL_KEY_RSHIFT: key = VK_RSHIFT; break;
	case CL_KEY_ALT: key = VK_LMENU; break;
	case CL_KEY_ALTGR: key = VK_RMENU; break;
	case CL_KEY_TAB: key = VK_TAB; break;
	case CL_KEY_ENTER: key = VK_RETURN; break;
	case CL_KEY_SPACE: key = VK_SPACE; break;
	case CL_KEY_BACKSPACE: key = VK_BACK; break;
	case CL_KEY_INSERT: key = VK_INSERT; break;
	case CL_KEY_DELETE: key = VK_DELETE; break;
	case CL_KEY_HOME: key = VK_HOME; break;
	case CL_KEY_END: key = VK_END; break;
	case CL_KEY_PAGEUP: key = VK_PRIOR; break;
	case CL_KEY_PAGEDOWN: key = VK_NEXT; break;
	case CL_KEY_CAPSLOCK: key = VK_CAPITAL; break;
	case CL_KEY_NUMLOCK: key = VK_NUMLOCK; break;
	case CL_KEY_SCRLOCK: key = VK_SCROLL; break;
	case CL_KEY_PRINT: key = VK_PRINT; break;
	case CL_KEY_PAUSE: key = VK_PAUSE; break;
	case CL_KEY_KP_DIV: key = VK_DIVIDE; break;
	case CL_KEY_KP_MULT: key = VK_MULTIPLY; break;
	case CL_KEY_KP_MINUS: key = VK_SUBTRACT; break;
	case CL_KEY_KP_PLUS: key = VK_ADD; break;
	case CL_KEY_KP_ENTER: key = VK_RETURN; break;
	case CL_KEY_KP_DECIMAL: key = VK_DECIMAL; break;
	default:
		return NULL;
	}

	if (key != -1)
		buttons[button_num] =
			new CL_InputButton_Keyboard_Win32(key, keys_down);

	return buttons[button_num];
}

char *CL_Keyboard_Win32::get_name() const
{
	return "Standard keyboard";
}

CL_InputDevice::InputDeviceType CL_Keyboard_Win32::get_type() const
{
	return CL_InputDevice::type_keyboard;
}

int CL_Keyboard_Win32::get_num_buttons() const
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_Keyboard_Win32::get_button(int button_num)
{
	if (buttons[button_num] != NULL) return buttons[button_num];

	return map_button(button_num);
}

int CL_Keyboard_Win32::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_Keyboard_Win32::get_axis(int axis_num)
{
	return NULL;
}

int CL_Keyboard_Win32::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_Keyboard_Win32::get_hat(int hat_num)
{
	return NULL;
}

int CL_Keyboard_Win32::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_Keyboard_Win32::get_cursor(int cursor_num)
{
	return NULL;
}

/************************************
		CL_InputButton_Keyboard_Win32
************************************/

CL_InputButton_Keyboard_Win32::CL_InputButton_Keyboard_Win32(
	int _key, unsigned char *_keys_down)
{
	key = _key;
	keys_down = _keys_down;
}

CL_InputButton_Keyboard_Win32::~CL_InputButton_Keyboard_Win32()
{
}

bool CL_InputButton_Keyboard_Win32::is_pressed()
{
	// possible alternative way to get key state, doesn't require the array
	//if (GetAsyncKeyState(key)&0x8000) return true;
	if (keys_down[key]&128) return true;
	return false;
}
