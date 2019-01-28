/*
	$Id: keyboard_svgalib.cpp,v 1.1 2001/03/06 15:09:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_SVGALIB

#include <vgakeyboard.h>
#include "keyboard_svgalib.h"

#include <API/Display/Input/inputaxis.h>
#include <Display/Input/Svgalib/keyboard_svgalib.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/Svgalib/keyboard_svgalib.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include <Display/Input/Svgalib/keyboard_svgalib.h>

#include "API/Core/System/cl_assert.h"

CL_SVGAKeyboard::CL_SVGAKeyboard()
{
	int err = keyboard_init();
	cl_assert(err != -1); // Failed to init svgalib keyboard handler

	keyboard_translatekeys(TRANSLATE_CURSORKEYS);
}

CL_SVGAKeyboard::~CL_SVGAKeyboard()
{
	keyboard_close();
	
	int num = buttons.get_num_items();
	for (int i=0; i<num; i++) delete buttons[i];
}

char *CL_SVGAKeyboard::get_name()
{
	return "Standard keyboard";
}

int CL_SVGAKeyboard::get_num_buttons()
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_SVGAKeyboard::get_button(int button_num)
{
	keyboard_update();
	if (buttons[button_num] != NULL) return buttons[button_num];

	int translate_key = -1;
	switch (button_num)
	{
	case CL_KEY_A: translate_key = SCANCODE_A; break;
	case CL_KEY_B: translate_key = SCANCODE_B; break;
	case CL_KEY_C: translate_key = SCANCODE_C; break;
	case CL_KEY_D: translate_key = SCANCODE_D; break;
	case CL_KEY_E: translate_key = SCANCODE_E; break;
	case CL_KEY_F: translate_key = SCANCODE_F; break;
	case CL_KEY_G: translate_key = SCANCODE_G; break;
	case CL_KEY_H: translate_key = SCANCODE_H; break;
	case CL_KEY_I: translate_key = SCANCODE_I; break;
	case CL_KEY_J: translate_key = SCANCODE_J; break;
	case CL_KEY_K: translate_key = SCANCODE_K; break;
	case CL_KEY_L: translate_key = SCANCODE_L; break;
	case CL_KEY_M: translate_key = SCANCODE_M; break;
	case CL_KEY_N: translate_key = SCANCODE_N; break;
	case CL_KEY_O: translate_key = SCANCODE_O; break;
	case CL_KEY_P: translate_key = SCANCODE_P; break;
	case CL_KEY_Q: translate_key = SCANCODE_Q; break;
	case CL_KEY_R: translate_key = SCANCODE_R; break;
	case CL_KEY_S: translate_key = SCANCODE_S; break;
	case CL_KEY_T: translate_key = SCANCODE_T; break;
	case CL_KEY_U: translate_key = SCANCODE_U; break;
	case CL_KEY_V: translate_key = SCANCODE_V; break;
	case CL_KEY_W: translate_key = SCANCODE_W; break;
	case CL_KEY_X: translate_key = SCANCODE_X; break;
	case CL_KEY_Y: translate_key = SCANCODE_Y; break;
	case CL_KEY_Z: translate_key = SCANCODE_Z; break;

	case CL_KEY_0: translate_key = SCANCODE_0; break;
	case CL_KEY_1: translate_key = SCANCODE_1; break;
	case CL_KEY_2: translate_key = SCANCODE_2; break;
	case CL_KEY_3: translate_key = SCANCODE_3; break;
	case CL_KEY_4: translate_key = SCANCODE_4; break;
	case CL_KEY_5: translate_key = SCANCODE_5; break;
	case CL_KEY_6: translate_key = SCANCODE_6; break;
	case CL_KEY_7: translate_key = SCANCODE_7; break;
	case CL_KEY_8: translate_key = SCANCODE_8; break;
	case CL_KEY_9: translate_key = SCANCODE_9; break;

	case CL_KEY_F1: translate_key = SCANCODE_F1; break;
	case CL_KEY_F2: translate_key = SCANCODE_F2; break;
	case CL_KEY_F3: translate_key = SCANCODE_F3; break;
	case CL_KEY_F4: translate_key = SCANCODE_F4; break;
	case CL_KEY_F5: translate_key = SCANCODE_F5; break;
	case CL_KEY_F6: translate_key = SCANCODE_F6; break;
	case CL_KEY_F7: translate_key = SCANCODE_F7; break;
	case CL_KEY_F8: translate_key = SCANCODE_F8; break;
	case CL_KEY_F9: translate_key = SCANCODE_F9; break;
	case CL_KEY_F10: translate_key = SCANCODE_F10; break;
	case CL_KEY_F11: translate_key = SCANCODE_F11; break;
	case CL_KEY_F12: translate_key = SCANCODE_F12; break;

	case CL_KEY_ESCAPE: translate_key = SCANCODE_ESCAPE; break;
	case CL_KEY_LEFT: translate_key = SCANCODE_CURSORLEFT; break;
	case CL_KEY_RIGHT: translate_key = SCANCODE_CURSORRIGHT; break;
	case CL_KEY_UP: translate_key = SCANCODE_CURSORUP; break;
	case CL_KEY_DOWN: translate_key = SCANCODE_CURSORDOWN; break;
	case CL_KEY_LCTRL: translate_key = SCANCODE_LEFTCONTROL; break;
	case CL_KEY_RCTRL: translate_key = SCANCODE_RIGHTCONTROL; break;
	case CL_KEY_LSHIFT: translate_key = SCANCODE_LEFTSHIFT; break;
	case CL_KEY_RSHIFT: translate_key = SCANCODE_RIGHTSHIFT; break;
	case CL_KEY_ALT: translate_key = SCANCODE_LEFTALT; break;
	case CL_KEY_ALTGR: translate_key = SCANCODE_RIGHTALT; break;
	case CL_KEY_TAB: translate_key = SCANCODE_TAB; break;
	case CL_KEY_ENTER: translate_key = SCANCODE_ENTER; break;
	case CL_KEY_SPACE: translate_key = SCANCODE_SPACE; break;
	case CL_KEY_BACKSPACE: translate_key = SCANCODE_BACKSPACE; break;
	case CL_KEY_INSERT: translate_key = SCANCODE_INSERT; break;
	case CL_KEY_DELETE: translate_key = SCANCODE_REMOVE; break;
	case CL_KEY_HOME: translate_key = SCANCODE_HOME; break;
	case CL_KEY_END: translate_key = SCANCODE_END; break;
	case CL_KEY_PAGEUP: translate_key = SCANCODE_PAGEUP; break;
	case CL_KEY_PAGEDOWN: translate_key = SCANCODE_PAGEDOWN; break;
	case CL_KEY_CAPSLOCK: translate_key = SCANCODE_CAPSLOCK; break;
	case CL_KEY_NUMLOCK: translate_key = SCANCODE_NUMLOCK; break;
	case CL_KEY_SCRLOCK: translate_key = SCANCODE_SCROLLLOCK; break;
	case CL_KEY_PRINT: translate_key = SCANCODE_PRINTSCREEN; break;
	case CL_KEY_PAUSE: translate_key = SCANCODE_BREAK; break;
	case CL_KEY_KP_DIV: translate_key = SCANCODE_KEYPADDIVIDE; break;
	case CL_KEY_KP_MULT: translate_key = SCANCODE_KEYPADMULTIPLY; break;
	case CL_KEY_KP_MINUS: translate_key = SCANCODE_KEYPADMINUS; break;
	case CL_KEY_KP_PLUS: translate_key = SCANCODE_KEYPADPLUS; break;
	case CL_KEY_KP_ENTER: translate_key = SCANCODE_KEYPADENTER; break;
	}

	if (translate_key == -1) return NULL;

	buttons.add(
		new CL_InputButton_SVGAKeyboard(translate_key),
		button_num);

	return buttons[button_num];
}

int CL_SVGAKeyboard::get_num_axes()
{
	return 0;
}

CL_InputAxis *CL_SVGAKeyboard::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_SVGAKeyboard::get_num_hats()
{
	return 0;
}

CL_InputHat *CL_SVGAKeyboard::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_SVGAKeyboard::get_num_cursors()
{
	return 0;
}

CL_InputCursor *CL_SVGAKeyboard::get_cursor(int /*cursor_num*/)
{
	return NULL;
}

/***********************
  InputButton SVGA
***********************/

CL_InputButton_SVGAKeyboard::CL_InputButton_SVGAKeyboard(int _key)
{
	key = _key;
}

bool CL_InputButton_SVGAKeyboard::is_pressed()
{
	if (keyboard_keypressed(key)) return true;
	return false;
}

#endif
