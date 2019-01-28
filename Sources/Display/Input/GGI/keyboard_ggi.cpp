/*
	$Id: keyboard_ggi.cpp,v 1.4 2002/01/23 11:47:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include <ggi/ggi.h>

#include "keyboard_ggi.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/key.h>
#include <Display/Input/GGI/keyboard_ggi.h>
#include <Display/Display/GGI/display_ggi.h>
#include <API/Display/Input/inputaxis.h>
#include <Display/Input/GGI/keyboard_ggi.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/GGI/keyboard_ggi.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include "Core/System/Generic/string_asm.h"

/******************
  Keyboard
******************/

CL_GGIKeyboard::CL_GGIKeyboard(CL_GGI_DisplayCard* card)
{
	memset( &keymap, 0, sizeof(keymap) );
	
	m_vis = card->vis;
	
	buttons = new CL_InputButton_GGIKeyboard*[CL_NUM_KEYS];
	for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;
}

CL_GGIKeyboard::~CL_GGIKeyboard()
{
	for (int i=0; i<CL_NUM_KEYS; i++) delete buttons[i];
	delete[] buttons;
}

void CL_GGIKeyboard::keep_alive()
{
	ggi_event_mask	mask;
	ggi_event		event;
	struct timeval	tv = {0,0};

	mask = ggiEventPoll( m_vis, emKeyboard, &tv );
	
	while (mask)
	{
		ggiEventRead( m_vis, &event, emKeyboard );
		handle_event( &event );
		
		mask = ggiEventPoll( m_vis, emKeyboard, &tv );
	}
}

void CL_GGIKeyboard::handle_event(gii_event* event)
{
	bool keydown;

	switch (event->any.type)
	{
		case evKeyPress:
		case evKeyRepeat:
			keydown = true;
			break;
		case evKeyRelease:
			keydown = false;
			break;
		default:
			return;
	}

	CL_Key key;
	key.id = translate(event->key.label);

	if (event->key.sym < 256) 
	{
		key.ascii = event->key.sym;
	}
	else
	{
		key.ascii = -1;		
	}

	key.x = key.y = -1;

	if (keydown)
	{
		key.state = CL_Key::Pressed;
		CL_Input::sig_button_press()(this, key);
	}
	else
	{
		key.state = CL_Key::Released;
		CL_Input::sig_button_release()(this, key);
	}
	keymap[translate(event->key.label)] = keydown;
}

char CL_GGIKeyboard::translate(int kb_value)
{
	switch (kb_value)
	{
	case GIIK_F1:	return CL_KEY_F1;
	case GIIK_F2:	return CL_KEY_F2;
	case GIIK_F3:	return CL_KEY_F3;
	case GIIK_F4:	return CL_KEY_F4;
	case GIIK_F5:	return CL_KEY_F5;
	case GIIK_F6:	return CL_KEY_F6;
	case GIIK_F7:	return CL_KEY_F7;
	case GIIK_F8:	return CL_KEY_F8;
	case GIIK_F9:	return CL_KEY_F9;
	case GIIK_F10:	return CL_KEY_F10;
	case GIIK_F11:	return CL_KEY_F11;
	case GIIK_F12:	return CL_KEY_F12;
	
	case GIIUC_A:	return CL_KEY_A;
	case GIIUC_B:	return CL_KEY_B;
	case GIIUC_C:	return CL_KEY_C;
	case GIIUC_D:	return CL_KEY_D;
	case GIIUC_E:	return CL_KEY_E;
	case GIIUC_F:	return CL_KEY_F;
	case GIIUC_G:	return CL_KEY_G;
	case GIIUC_H:	return CL_KEY_H;
	case GIIUC_I:	return CL_KEY_I;
	case GIIUC_J:	return CL_KEY_J;
	case GIIUC_K:	return CL_KEY_K;
	case GIIUC_L:	return CL_KEY_L;
	case GIIUC_M:	return CL_KEY_M;
	case GIIUC_N:	return CL_KEY_N;
	case GIIUC_O:	return CL_KEY_O;
	case GIIUC_P:	return CL_KEY_P;
	case GIIUC_Q:	return CL_KEY_Q;
	case GIIUC_R:	return CL_KEY_R;
	case GIIUC_S:	return CL_KEY_S;
	case GIIUC_T:	return CL_KEY_T;
	case GIIUC_U:	return CL_KEY_U;
	case GIIUC_V:	return CL_KEY_V;
	case GIIUC_W:	return CL_KEY_W;
	case GIIUC_X:	return CL_KEY_X;
	case GIIUC_Y:	return CL_KEY_Y;
	case GIIUC_Z:	return CL_KEY_Z;
	
	case GIIUC_0:	return CL_KEY_0;
	case GIIUC_1:	return CL_KEY_1;
	case GIIUC_2:	return CL_KEY_2;
	case GIIUC_3:	return CL_KEY_3;
	case GIIUC_4:	return CL_KEY_4;
	case GIIUC_5:	return CL_KEY_5;
	case GIIUC_6:	return CL_KEY_6;
	case GIIUC_7:	return CL_KEY_7;
	case GIIUC_8:	return CL_KEY_8;
	case GIIUC_9:	return CL_KEY_9;
	
	case GIIUC_Escape:	return CL_KEY_ESCAPE;
	case GIIK_Left:	return CL_KEY_LEFT;
	case GIIK_Right:	return CL_KEY_RIGHT;
	case GIIK_Up:		return CL_KEY_UP;
	case GIIK_Down:	return CL_KEY_DOWN;
	case GIIK_Enter:	return CL_KEY_ENTER;

	case GIIK_CtrlL:	return CL_KEY_LCTRL;
	case GIIK_CtrlR:	return CL_KEY_RCTRL;
	case GIIK_ShiftL:	return CL_KEY_LSHIFT;
	case GIIK_ShiftR:	return CL_KEY_RSHIFT;
	case GIIK_AltL:	return CL_KEY_ALT;
	case GIIK_AltR:	return CL_KEY_ALT;
	case GIIK_AltGr:	return CL_KEY_ALTGR;
	case GIIUC_Tab:	return CL_KEY_TAB;
	case GIIUC_Space:	return CL_KEY_SPACE;
	case GIIUC_BackSpace:	return CL_KEY_BACKSPACE;
	case GIIK_Insert:	return CL_KEY_INSERT;
	case GIIK_Delete:	return CL_KEY_DELETE;
	case GIIK_Home:	return CL_KEY_HOME;
	case GIIK_End:		return CL_KEY_END;
	case GIIK_PageUp:	return CL_KEY_PAGEUP;
	case GIIK_PageDown:	return CL_KEY_PAGEDOWN;
	case GIIK_Caps:	return CL_KEY_CAPSLOCK;
	case GIIK_Num:		return CL_KEY_NUMLOCK;
	case GIIK_Scroll:	return CL_KEY_SCRLOCK;
	case GIIK_PrintScreen:	return CL_KEY_PRINT;
	case GIIK_Pause:	return CL_KEY_PAUSE;
	case GIIK_PSlash:	return CL_KEY_KP_DIV;
	case GIIK_PAsterisk:	return CL_KEY_KP_MULT;
	case GIIK_PMinus:	return CL_KEY_KP_MINUS;
	case GIIK_PPlus:	return CL_KEY_KP_PLUS;
	case GIIK_PEnter:	return CL_KEY_KP_ENTER;
	}
	
	return CL_KEY_NONE_OF_THE_ABOVE;
}

char *CL_GGIKeyboard::get_name() const
{
	return "GGI keyboard";
}

CL_InputDevice::InputDeviceType CL_GGIKeyboard::get_type() const
{
	return CL_InputDevice::type_keyboard;
}

int CL_GGIKeyboard::get_num_buttons() const
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_GGIKeyboard::get_button(int button_num)
{
	if (button_num < 0 || button_num >= CL_NUM_KEYS) return NULL;

	if (buttons[button_num] == NULL)
		buttons[button_num] = new CL_InputButton_GGIKeyboard(button_num, keymap);

	return buttons[button_num];
}

int CL_GGIKeyboard::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_GGIKeyboard::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_GGIKeyboard::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_GGIKeyboard::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_GGIKeyboard::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_GGIKeyboard::get_cursor(int /*cursor_num*/)
{
	return NULL;
}

/***************************
  CL_InputButton_GGIKeyboard
***************************/

CL_InputButton_GGIKeyboard::CL_InputButton_GGIKeyboard(
	int _key, char *_keymap)
{
	key = _key;
	keymap = _keymap;
}

bool CL_InputButton_GGIKeyboard::is_pressed()
{
	return keymap[key];
}

#endif
