/*
	$Id: keyboard_x11.cpp,v 1.18 2002/03/02 16:43:48 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_X11

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "keyboard_x11.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/key.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include <API/Display/Input/keyboard.h>
#include "API/Core/System/cl_assert.h"

#include <Display/Input/X11/keyboard_x11.h>
#include <Display/Display/X11/display_xwindow.h>

#include "Core/System/Generic/string_asm.h"

/******************
  Keyboard
******************/

CL_XWindowKeyboard::CL_XWindowKeyboard(
	CL_XWindow_CompatibleCard *_card)
{
	card = _card;

	for (int j=0; j<X_KEYBOARD_BUFFERSIZE; j++) keys_return[j]=0;

	buttons = new CL_InputButton_XKeyboard*[CL_NUM_KEYS];
	for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;

	slot_xevent = CL_XWindow_CompatibleCard::sig_xevent.connect(
		this, &CL_XWindowKeyboard::on_xevent);
}

CL_XWindowKeyboard::~CL_XWindowKeyboard()
{
	for (int i=0; i<CL_NUM_KEYS; i++) delete buttons[i];
	delete[] buttons;

	CL_XWindow_CompatibleCard::sig_xevent.disconnect(slot_xevent);

}

void CL_XWindowKeyboard::on_xevent(XEvent &e)
{
	if (e.type != KeyPress && e.type != KeyRelease) return;
/*	
	keep_alive();
	std::cout << "keymap: ";
	for (int j=0; j<X_KEYBOARD_BUFFERSIZE; j++) std::cout << (int) keys_return[j];
	std::cout << std::endl;
*/
	KeySym sym = XLookupKeysym(&e.xkey,0);
//	std::cout << "key sym: " << sym << std::endl;
		
	CL_Key key;
		
	key.x = key.y = -1; // since the keyboard is no pointing device

	key.id = map_keysym_to_id(sym);
	key.ascii = -1; // todo: map to ascii here.
		
	// Figure out the ascii value from symkey:
	char buf[11];
	buf[10]=0;
	XLookupString(&e.xkey, buf, 10, NULL, NULL);
	if (strlen(buf) > 0) key.ascii = buf[0];
		
	if (e.type == KeyRelease)
	{
		key.state = CL_Key::Released;
		CL_Input::sig_button_release()(this, key);
	}
	else
	{
		key.state = CL_Key::Pressed;
		CL_Input::sig_button_press()(this, key);
	}
}

void CL_XWindowKeyboard::keep_alive()
{
	// Check if window has focus right now:
	Window focus_win;
	int focus_state;
	XGetInputFocus(
		card->get_display(),
		&focus_win,
		&focus_state);
		
	// get_window() returns NULL if in DGA mode.
	if (card->get_window() == 0 ||
		card->get_window() == focus_win)
	{
		XQueryKeymap(card->get_display(), keys_return);
	}
	else
	{
		// no keys are down when not in focus:
		for (int j=0; j<X_KEYBOARD_BUFFERSIZE; j++) keys_return[j]=0;
	}
}

char *CL_XWindowKeyboard::get_name() const
{
	return "XWindow keyboard";
}

CL_InputDevice::InputDeviceType CL_XWindowKeyboard::get_type() const
{
	return CL_InputDevice::type_keyboard;
}

int CL_XWindowKeyboard::get_num_buttons() const
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_XWindowKeyboard::get_button(int button_num)
{
	if (buttons[button_num] != NULL) return buttons[button_num];

	int translate_key = map_id_to_keysym(button_num);
	if (translate_key == -1) return NULL;

	buttons[button_num] = 
		new CL_InputButton_XKeyboard(translate_key, keys_return);

	return buttons[button_num];
}

int CL_XWindowKeyboard::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_XWindowKeyboard::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_XWindowKeyboard::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_XWindowKeyboard::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_XWindowKeyboard::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_XWindowKeyboard::get_cursor(int /*cursor_num*/)
{
	return NULL;
}

int CL_XWindowKeyboard::map_keysym_to_id(int keysym)
{
	switch (keysym)
	{
	case 269025062: return CL_KEY_ESCAPE;
	case XK_Select: return CL_KEY_ENTER;
	case XK_BackSpace: return CL_KEY_BACKSPACE;
	case XK_Tab: return CL_KEY_TAB;
//	case XK_Clear: return CL_KEY_CLEAR;
	case XK_Return: return CL_KEY_ENTER;
	case XK_Pause: return CL_KEY_PAUSE;
//	case XK_Scroll_Lock: return CL_KEY_SCROLL;
	case XK_Escape: return CL_KEY_ESCAPE;
	case XK_Delete: return CL_KEY_DELETE;
//	case XK_Kanji: return CL_KEY_KANJI;
	case XK_Home: return CL_KEY_HOME;
	case XK_Left: return CL_KEY_LEFT;
	case XK_Up: return CL_KEY_UP;
	case XK_Right: return CL_KEY_RIGHT;
	case XK_Down: return CL_KEY_DOWN;
	case XK_Page_Up: return CL_KEY_PAGEUP;
	case XK_Page_Down: return CL_KEY_PAGEDOWN;
	case XK_End: return CL_KEY_END;
	case XK_Print: return CL_KEY_PRINT;
	case XK_Insert: return CL_KEY_INSERT;
	case XK_Num_Lock: return CL_KEY_NUMLOCK;
	case XK_KP_0: return CL_KEY_KP_0;
	case XK_KP_1: return CL_KEY_KP_1;
	case XK_KP_2: return CL_KEY_KP_2;
	case XK_KP_3: return CL_KEY_KP_3;
	case XK_KP_4: return CL_KEY_KP_4;
	case XK_KP_5: return CL_KEY_KP_5;
	case XK_KP_6: return CL_KEY_KP_6;
	case XK_KP_7: return CL_KEY_KP_7;
	case XK_KP_8: return CL_KEY_KP_8;
	case XK_KP_9: return CL_KEY_KP_9;
	case XK_F1: return CL_KEY_F1;
	case XK_F2: return CL_KEY_F2;
	case XK_F3: return CL_KEY_F3;
	case XK_F4: return CL_KEY_F4;
	case XK_F5: return CL_KEY_F5;
	case XK_F6: return CL_KEY_F6;
	case XK_F7: return CL_KEY_F7;
	case XK_F8: return CL_KEY_F8;
	case XK_F9: return CL_KEY_F9;
	case XK_F10: return CL_KEY_F10;
	case XK_F11: return CL_KEY_F11;
	case XK_F12: return CL_KEY_F12;
	case XK_Shift_L: return CL_KEY_LSHIFT;
	case XK_Shift_R: return CL_KEY_RSHIFT;
	case XK_Control_L: return CL_KEY_LCTRL;
	case XK_Control_R: return CL_KEY_RCTRL;
	case XK_Caps_Lock: return CL_KEY_CAPSLOCK;
	case XK_Meta_L: return CL_KEY_ALT;
	case XK_Meta_R: return CL_KEY_ALTGR;
	case XK_Alt_L: return CL_KEY_ALT;
	case XK_Alt_R: return CL_KEY_ALTGR;

	case XK_space: return CL_KEY_SPACE;
//	case XK_comma: return CL_KEY_COMMA;
//	case XK_minus: return CL_KEY_MINUS;
//	case XK_period: return CL_KEY_PERIOD;
//	case XK_slash: return CL_KEY_SLASH;
	case XK_0: return CL_KEY_0;
	case XK_1: return CL_KEY_1;
	case XK_2: return CL_KEY_2;
	case XK_3: return CL_KEY_3;
	case XK_4: return CL_KEY_4;
	case XK_5: return CL_KEY_5;
	case XK_6: return CL_KEY_6;
	case XK_7: return CL_KEY_7;
	case XK_8: return CL_KEY_8;
	case XK_9: return CL_KEY_9;
//	case XK_semicolon: return CL_KEY_PUNKTUM;
//	case XK_equal: return CL_KEY_EQUAL;
//	case XK_bracketleft: return CL_KEY_BRACKETLEFT;
//	case XK_backslash: return CL_KEY_BACKSLASH;
//	case XK_bracketright: return CL_KEY_BRACKETRIGHT;
	case XK_a: return CL_KEY_A;
	case XK_b: return CL_KEY_B;
	case XK_c: return CL_KEY_C;
	case XK_d: return CL_KEY_D;
	case XK_e: return CL_KEY_E;
	case XK_f: return CL_KEY_F;
	case XK_g: return CL_KEY_G;
	case XK_h: return CL_KEY_H;
	case XK_i: return CL_KEY_I;
	case XK_j: return CL_KEY_J;
	case XK_k: return CL_KEY_K;
	case XK_l: return CL_KEY_L;
	case XK_m: return CL_KEY_M;
	case XK_n: return CL_KEY_N;
	case XK_o: return CL_KEY_O;
	case XK_p: return CL_KEY_P;
	case XK_q: return CL_KEY_Q;
	case XK_r: return CL_KEY_R;
	case XK_s: return CL_KEY_S;
	case XK_t: return CL_KEY_T;
	case XK_u: return CL_KEY_U;
	case XK_v: return CL_KEY_V;
	case XK_w: return CL_KEY_W;
	case XK_x: return CL_KEY_X;
	case XK_y: return CL_KEY_Y;
	case XK_z: return CL_KEY_Z;
	}

	return CL_KEY_NONE_OF_THE_ABOVE;
}

int CL_XWindowKeyboard::map_id_to_keysym(int id)
{
	switch (id)
	{
	case CL_KEY_A: return 38;
	case CL_KEY_B: return 56;
	case CL_KEY_C: return 54;
	case CL_KEY_D: return 40;
	case CL_KEY_E: return 26;
	case CL_KEY_F: return 41;
	case CL_KEY_G: return 42;
	case CL_KEY_H: return 43;
	case CL_KEY_I: return 31;
	case CL_KEY_J: return 44;
	case CL_KEY_K: return 45;
	case CL_KEY_L: return 46;
	case CL_KEY_M: return 58;
	case CL_KEY_N: return 57;
	case CL_KEY_O: return 32;
	case CL_KEY_P: return 33;
	case CL_KEY_Q: return 24;
	case CL_KEY_R: return 27;
	case CL_KEY_S: return 39;
	case CL_KEY_T: return 28;
	case CL_KEY_U: return 30;
	case CL_KEY_V: return 55;
	case CL_KEY_W: return 25;
	case CL_KEY_X: return 53;
	case CL_KEY_Y: return 29;
	case CL_KEY_Z: return 52;

	case CL_KEY_0: return 19;
	case CL_KEY_1: return 10;
	case CL_KEY_2: return 11;
	case CL_KEY_3: return 12;
	case CL_KEY_4: return 13;
	case CL_KEY_5: return 14;
	case CL_KEY_6: return 15;
	case CL_KEY_7: return 16;
	case CL_KEY_8: return 17;
	case CL_KEY_9: return 18;

	case CL_KEY_F1: return 67;
	case CL_KEY_F2: return 68;
	case CL_KEY_F3: return 69;
	case CL_KEY_F4: return 70;
	case CL_KEY_F5: return 71;
	case CL_KEY_F6: return 72;
	case CL_KEY_F7: return 73;
	case CL_KEY_F8: return 74;
	case CL_KEY_F9: return 75;
	case CL_KEY_F10: return 76;
	case CL_KEY_F11: return 95;
	case CL_KEY_F12: return 96;

	case CL_KEY_ESCAPE: return 9; 
	case CL_KEY_LEFT: return 100;
	case CL_KEY_RIGHT: return 102;
	case CL_KEY_UP: return 98;
	case CL_KEY_DOWN: return 104;
	case CL_KEY_LCTRL: return 37;
	case CL_KEY_RCTRL: return 109;
	case CL_KEY_LSHIFT: return 50;
	case CL_KEY_RSHIFT: return 62;
	case CL_KEY_ALT: return 64;
	case CL_KEY_ALTGR: return 113;
	case CL_KEY_TAB: return 23;
	case CL_KEY_ENTER: return 36;
	case CL_KEY_SPACE: return 65;
	case CL_KEY_BACKSPACE: return 22;
	case CL_KEY_INSERT: return 106;
	case CL_KEY_DELETE: return 107;
	case CL_KEY_HOME: return 97;
	case CL_KEY_END: return 98;
	case CL_KEY_PAGEUP: return 99;
	case CL_KEY_PAGEDOWN: return 105;
	case CL_KEY_CAPSLOCK: return 66;
	case CL_KEY_NUMLOCK: return 77;
	case CL_KEY_SCRLOCK: return 78;
	case CL_KEY_PRINT: return 111;
	case CL_KEY_PAUSE: return 110;
	case CL_KEY_KP_DIV: return 112;
	case CL_KEY_KP_MULT: return 63;
	case CL_KEY_KP_MINUS: return 82;
	case CL_KEY_KP_PLUS: return 86;
	case CL_KEY_KP_ENTER: return 108;
 
 	/* Found these codes in /usr/X11R6/lib/X11/xkb/keycodes/xfree86
 	 * from an XFree86 4.0.1 installation. 
 	 * Tested on US 105 key keyboard. <timcr038@student.otago.ac.nz>
 	 */
 	case CL_KEY_KP_0: return 90;
 	case CL_KEY_KP_1: return 87;
 	case CL_KEY_KP_2: return 88;
 	case CL_KEY_KP_3: return 89;
 	case CL_KEY_KP_4: return 83;
 	case CL_KEY_KP_5: return 84;
 	case CL_KEY_KP_6: return 85;
 	case CL_KEY_KP_7: return 79;
 	case CL_KEY_KP_8: return 80;
 	case CL_KEY_KP_9: return 81;
	}
	
	return -1;
}

/***************************
  CL_InputButton_XKeyboard
***************************/

CL_InputButton_XKeyboard::CL_InputButton_XKeyboard(
	int _key, char *_keys_return)
{
	key = _key;
	keys_return = _keys_return;
}

bool CL_InputButton_XKeyboard::is_pressed()
{
	int byte = key/8;
	int bit = key%8;

	bool pressed = (keys_return[byte] & (1<<bit)) ? true : false;
	if (pressed) return true;
	
	if (key == 36) // enter hack
	{
		int select_key = 127;
		byte = select_key/8;
		bit = select_key%8;

		pressed = (keys_return[byte] & (1<<bit)) ? true : false;
	}
	
	if (key == 9) // escape hack
	{
		int select_key = 131;
		byte = select_key/8;
		bit = select_key%8;

		pressed = (keys_return[byte] & (1<<bit)) ? true : false;
	}
	
	return pressed;
}

#endif
