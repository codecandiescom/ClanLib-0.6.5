/*
	$Id: input_mouse.cpp,v 1.10 2002/05/17 08:47:58 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <windows.h>
#include "API/Display/Display/display.h"
#include "API/Display/Input/mouse.h"
#include "input_mouse.h"
#include "API/Core/System/cl_assert.h"
#include "Display/Display/DirectDraw/displaycard_win32compatible.h"

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL WM_MOUSELAST+1 
#endif

/******************************
	CL_Mouse_Win32
******************************/

#define NUM_MBUTTONS 3

CL_Mouse_Win32::CL_Mouse_Win32()
{
	cursor = new CL_InputCursor_Mouse_Win32();
	axes = new CL_InputAxis_Mouse_Win32[2];

	buttons = new CL_InputButton_Mouse_Win32*[NUM_MBUTTONS];
	for (int i=0; i<NUM_MBUTTONS; i++) buttons[i] = new CL_InputButton_Mouse_Win32(i);

	CL_System_Win32::add_listener(this);
}

CL_Mouse_Win32::~CL_Mouse_Win32()
{
	CL_System_Win32::remove_listener(this);

	delete cursor;
  delete[] axes;

	for (int i=0; i<NUM_MBUTTONS; i++) delete buttons[i];
	delete[] buttons;
}

char *CL_Mouse_Win32::get_name() const
{
	return "Standard mouse";
}

CL_InputDevice::InputDeviceType CL_Mouse_Win32::get_type() const
{
	return CL_InputDevice::type_mouse;
}

int CL_Mouse_Win32::get_num_buttons() const
{
	return NUM_MBUTTONS;
}

CL_InputButton *CL_Mouse_Win32::get_button(int button_num)
{
	return buttons[button_num];
}

int CL_Mouse_Win32::get_num_axes() const
{
	return 2;
}

CL_InputAxis *CL_Mouse_Win32::get_axis(int axis_num)
{
	cl_assert(((unsigned int) axis_num)<2);
	return &axes[axis_num];
}

int CL_Mouse_Win32::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_Mouse_Win32::get_hat(int hat_num)
{
	return NULL;
}

int CL_Mouse_Win32::get_num_cursors() const
{
	return 1;
}

CL_InputCursor *CL_Mouse_Win32::get_cursor(int cursor_num)
{
	return cursor;
}

void CL_Mouse_Win32::keep_alive()
{
	if (CL_Display::get_current_card() == NULL) return;

	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	CL_DisplayCard_Win32Compatible *win32card =
		(CL_DisplayCard_Win32Compatible *) CL_Display::get_current_card();

	BOOL res = ScreenToClient(win32card->get_hwnd(), &cursor_pos);

	if (cursor->x != cursor_pos.x ||
		cursor->y != cursor_pos.y)
	{
		cursor->x = (float)cursor_pos.x;
		cursor->y = (float)cursor_pos.y;

		CL_Input::sig_mouse_move()(this, (int)cursor->x, (int)cursor->y);
		CL_Mouse::sig_move()((int)cursor->x, (int)cursor->y);
	}

	axes[0].center = float(CL_Display::get_width())/2.0f;
	axes[1].center = float(CL_Display::get_height())/2.0f;
	
	axes[0].pos = (float)cursor_pos.x;
	axes[1].pos = (float)cursor_pos.y;
}

bool CL_Mouse_Win32::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (CL_Display::get_current_card() == NULL) return false;

	bool up = false;
	switch (uMsg)
	{
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		up = true;
		// fall through
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			int button_states = wParam;

			for (int i=0; i<NUM_MBUTTONS; i++)
			{
				if (buttons[i] == NULL) continue;

				// This code didn't work on Sphairs mouse:
				// bool new_state = (button_states&(1<<i)) ? true : false;

				// Hardcoding the bitposition instead:
				bool new_state = false;
				if(i == 0)
					new_state = (button_states & 1) ? true : false;
				else if(i == 1)
					new_state = (button_states & 16) ? true : false;
				else if(i == 2)
					new_state = (button_states & 2) ? true : false;

				if (new_state != buttons[i]->button_state)
				{
					buttons[i]->button_state = new_state;

					CL_Key key;
					key.ascii = -1;
					key.id = CL_MOUSE_LEFTBUTTON + i;
					key.x = cursor->x;
					key.y = cursor->y;

					if (up == false)
					{
						key.state = CL_Key::Pressed;
						CL_Input::sig_button_press()(this, key);
						CL_Mouse::sig_button_press()(key);
					}
					else
					{
						key.state = CL_Key::Released;
						CL_Input::sig_button_release()(this, key);
						CL_Mouse::sig_button_release()(key);
					}
				}
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if (cursor->x != x ||
				cursor->y != y)
			{
				cursor->x = (float)x;
				cursor->y = (float)y;

				CL_Input::sig_mouse_move()(this, (int)cursor->x, (int)cursor->y);
				CL_Mouse::sig_move()((int)cursor->x, (int)cursor->y);
			}

			axes[0].center = float(CL_Display::get_width())/2.0f;
			axes[1].center = float(CL_Display::get_height())/2.0f;
			
			axes[0].pos = (float)x;
			axes[1].pos = (float)y;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			int keyflags = LOWORD(wParam);			/* key flags */ 
			short delta = (short) HIWORD(wParam);	/* wheel rotation */
			short x_pos = (short) LOWORD(lParam);	/* horizontal position of pointer */
			short y_pos = (short) HIWORD(lParam);	/* vertical position of pointer */ 

			// TODO: Add a Modifier field to CL_Key and put keyflags into it

			CL_Key key;
			key.ascii = -1;
			if(delta > 0)
				key.id = CL_MOUSE_WHEELUP;
			else
				key.id = CL_MOUSE_WHEELDOWN;

			key.x = cursor->x;
			key.y = cursor->y;

			key.state = CL_Key::Pressed;
			CL_Input::sig_button_press()(this, key);
			CL_Mouse::sig_button_press()(key);

			key.state = CL_Key::Released;
			CL_Input::sig_button_release()(this, key);
			CL_Mouse::sig_button_release()(key);
		}
		break;

	default:
		return false;
	}

	return true;
}

/*******************************
	 CL_InputButton_Mouse_Win32
*******************************/

CL_InputButton_Mouse_Win32::CL_InputButton_Mouse_Win32(int _key)
{
	key = (1<<_key);
	button_state = false;
}

CL_InputButton_Mouse_Win32::~CL_InputButton_Mouse_Win32()
{
}

bool CL_InputButton_Mouse_Win32::is_pressed()
{
	return button_state;
}

/*******************************
	 CL_InputCursor_Mouse_Win32
*******************************/

CL_InputCursor_Mouse_Win32::CL_InputCursor_Mouse_Win32()
{
	x = 0;
	y = 0;
}

CL_InputCursor_Mouse_Win32::~CL_InputCursor_Mouse_Win32()
{
}

float CL_InputCursor_Mouse_Win32::get_x()
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	CL_DisplayCard_Win32Compatible *win32card =
		(CL_DisplayCard_Win32Compatible *) CL_Display::get_current_card();

	ScreenToClient(win32card->get_hwnd(), &cursor_pos);

	return (float)cursor_pos.x;
}

float CL_InputCursor_Mouse_Win32::get_y()
{
	POINT cursor_pos;
	GetCursorPos(&cursor_pos);

	CL_DisplayCard_Win32Compatible *win32card =
		(CL_DisplayCard_Win32Compatible *) CL_Display::get_current_card();

	ScreenToClient(win32card->get_hwnd(), &cursor_pos);

	return (float)cursor_pos.y;
}

void CL_InputCursor_Mouse_Win32::set_position (float x, float y)
{
  // FIXME: implement me
}

float CL_InputCursor_Mouse_Win32::get_max_x()
{
	if (CL_Display::get_current_card() == NULL) return -1;
	return (float)CL_Display::get_width();
}

float CL_InputCursor_Mouse_Win32::get_max_y()
{
	if (CL_Display::get_current_card() == NULL) return -1;
	return (float)CL_Display::get_height();
}

/*******************************
	 CL_InputAxis_Mouse_Win32
*******************************/

CL_InputAxis_Mouse_Win32::CL_InputAxis_Mouse_Win32()
{
	pos = 0;
	center = 1;
}

CL_InputAxis_Mouse_Win32::~CL_InputAxis_Mouse_Win32()
{
}

float CL_InputAxis_Mouse_Win32::get_pos()
{
	return (pos - center) / center;
}
