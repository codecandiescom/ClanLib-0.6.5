/*
	$Id: keyboard_be.cpp,v 1.1 2001/03/06 15:09:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <Display/Input/Be/keyboard_be.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbuffer.h>
#include <Display/Input/Be/keyboard_be.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/Be/keyboard_be.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include <Core/System/Be/app_beos.h>
//#include <be/app/Message.h>

/******************
  Keyboard
******************/

CL_BeKeyboard::CL_BeKeyboard()
{
	memset( &keymap, 0, sizeof(keymap) );

	buttons = new CL_InputButton_BeKeyboard*[CL_NUM_KEYS];
	for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;
}

CL_BeKeyboard::~CL_BeKeyboard()
{
	for (int i=0; i<CL_NUM_KEYS; i++) delete buttons[i];
	delete[] buttons;
}

void CL_BeKeyboard::handle_modifiers(BMessage* msg)
{
	int32 mods;
	msg->FindInt32("modifiers", &mods);
	
	keymap[CL_KEY_LSHIFT] = (mods & B_LEFT_SHIFT_KEY);
	keymap[CL_KEY_RSHIFT] = (mods & B_RIGHT_SHIFT_KEY);
	keymap[CL_KEY_LCTRL] = (mods & B_LEFT_CONTROL_KEY);
	keymap[CL_KEY_RCTRL] = (mods & B_RIGHT_CONTROL_KEY);
	keymap[CL_KEY_ALT] = (mods & B_LEFT_OPTION_KEY);
	keymap[CL_KEY_ALTGR] = (mods & B_RIGHT_OPTION_KEY);
}

void CL_BeKeyboard::handle_key(BMessage* msg, bool down)
{
	int8 val;
	msg->FindInt8("byte", &val);

	switch (val)
	{
		case B_ENTER:
//			if (down) buffer.add_key( CL_KEY_ENTER );
			keymap[CL_KEY_ENTER] = down;
			break;
		case B_ESCAPE:
//			if (down) buffer.add_key( CL_KEY_ESCAPE );
			keymap[CL_KEY_ESCAPE] = down;
			break;
		case B_BACKSPACE:
//			if (down) buffer.add_key( CL_KEY_BACKSPACE );
			keymap[CL_KEY_BACKSPACE] = down;
			break;
		case B_SPACE:
//			if (down) buffer.add_key( CL_KEY_SPACE );
			keymap[CL_KEY_SPACE] = down;
			break;
		case B_TAB:
//			if (down) buffer.add_key( CL_KEY_TAB );
			keymap[CL_KEY_TAB] = down;
			break;
		case B_LEFT_ARROW:
//			if (down) buffer.add_key( CL_KEY_LEFT );
			keymap[CL_KEY_LEFT] = down;
			break;
		case B_RIGHT_ARROW:
//			if (down) buffer.add_key( CL_KEY_RIGHT );
			keymap[CL_KEY_RIGHT] = down;
			break;
		case B_UP_ARROW:
//			if (down) buffer.add_key( CL_KEY_UP );
			keymap[CL_KEY_UP] = down;
			break;
		case B_DOWN_ARROW:
//			if (down) buffer.add_key( CL_KEY_DOWN );
			keymap[CL_KEY_DOWN] = down;
			break;
		case B_INSERT:
//			if (down) buffer.add_key( CL_KEY_INSERT );
			keymap[CL_KEY_INSERT] = down;
			break;
		case B_DELETE:
//			if (down) buffer.add_key( CL_KEY_DELETE );
			keymap[CL_KEY_DELETE] = down;
			break;
		case B_HOME:
//			if (down) buffer.add_key( CL_KEY_HOME );
			keymap[CL_KEY_HOME] = down;
			break;
		case B_END:
//			if (down) buffer.add_key( CL_KEY_END );
			keymap[CL_KEY_END] = down;
			break;
		case B_PAGE_UP:
//			if (down) buffer.add_key( CL_KEY_PAGEUP );
			keymap[CL_KEY_PAGEUP] = down;
			break;
		case B_PAGE_DOWN:
//			if (down) buffer.add_key( CL_KEY_PAGEDOWN );
			keymap[CL_KEY_PAGEDOWN] = down;
			break;
	}
	
	switch (msg->FindInt32("key"))
	{
		case B_F1_KEY:
//			if (down) buffer.add_key( CL_KEY_F1 );
			keymap[CL_KEY_F1] = down;
			break;
		case B_F2_KEY:
//			if (down) buffer.add_key( CL_KEY_F2 );
			keymap[CL_KEY_F2] = down;
			break;
		case B_F3_KEY:
//			if (down) buffer.add_key( CL_KEY_F3 );
			keymap[CL_KEY_F3] = down;
			break;
		case B_F4_KEY:
//			if (down) buffer.add_key( CL_KEY_F4 );
			keymap[CL_KEY_F4] = down;
			break;
		case B_F5_KEY:
//			if (down) buffer.add_key( CL_KEY_F5 );
			keymap[CL_KEY_F5] = down;
			break;
		case B_F6_KEY:
//			if (down) buffer.add_key( CL_KEY_F6 );
			keymap[CL_KEY_F6] = down;
			break;
		case B_F7_KEY:
//			if (down) buffer.add_key( CL_KEY_F7 );
			keymap[CL_KEY_F7] = down;
			break;
		case B_F8_KEY:
//			if (down) buffer.add_key( CL_KEY_F8 );
			keymap[CL_KEY_F8] = down;
			break;
		case B_F9_KEY:
//			if (down) buffer.add_key( CL_KEY_F9 );
			keymap[CL_KEY_F9] = down;
			break;
		case B_F10_KEY:
//			if (down) buffer.add_key( CL_KEY_F10 );
			keymap[CL_KEY_F10] = down;
			break;
		case B_F11_KEY:
//			if (down) buffer.add_key( CL_KEY_F11 );
			keymap[CL_KEY_F11] = down;
			break;
		case B_F12_KEY:
//			if (down) buffer.add_key( CL_KEY_F12 );
			keymap[CL_KEY_F12] = down;
			break;
	}

	switch (msg->FindInt32("raw_char"))
	{
		case 'a':
//			if (down) buffer.add_key( CL_KEY_A );
			keymap[CL_KEY_A] = down;
			break;
		case 'b':
//			if (down) buffer.add_key( CL_KEY_B );
			keymap[CL_KEY_B] = down;
			break;
		case 'c':
//			if (down) buffer.add_key( CL_KEY_C );
			keymap[CL_KEY_C] = down;
			break;
		case 'd':
//			if (down) buffer.add_key( CL_KEY_D );
			keymap[CL_KEY_D] = down;
			break;
		case 'e':
//			if (down) buffer.add_key( CL_KEY_E );
			keymap[CL_KEY_E] = down;
			break;
		case 'f':
//			if (down) buffer.add_key( CL_KEY_F );
			keymap[CL_KEY_F] = down;
			break;
		case 'g':
//			if (down) buffer.add_key( CL_KEY_G );
			keymap[CL_KEY_G] = down;
			break;
		case 'h':
//			if (down) buffer.add_key( CL_KEY_H );
			keymap[CL_KEY_H] = down;
			break;
		case 'i':
//			if (down) buffer.add_key( CL_KEY_I );
			keymap[CL_KEY_I] = down;
			break;
		case 'j':
//			if (down) buffer.add_key( CL_KEY_J );
			keymap[CL_KEY_J] = down;
			break;
		case 'k':
//			if (down) buffer.add_key( CL_KEY_K );
			keymap[CL_KEY_K] = down;
			break;
		case 'l':
//			if (down) buffer.add_key( CL_KEY_L );
			keymap[CL_KEY_L] = down;
			break;
		case 'm':
//			if (down) buffer.add_key( CL_KEY_M );
			keymap[CL_KEY_M] = down;
			break;
		case 'n':
//			if (down) buffer.add_key( CL_KEY_N );
			keymap[CL_KEY_N] = down;
			break;
		case 'o':
//			if (down) buffer.add_key( CL_KEY_O );
			keymap[CL_KEY_O] = down;
			break;
		case 'p':
//			if (down) buffer.add_key( CL_KEY_P );
			keymap[CL_KEY_P] = down;
			break;
		case 'q':
//			if (down) buffer.add_key( CL_KEY_Q );
			keymap[CL_KEY_Q] = down;
			break;
		case 'r':
//			if (down) buffer.add_key( CL_KEY_R );
			keymap[CL_KEY_R] = down;
			break;
		case 's':
//			if (down) buffer.add_key( CL_KEY_S );
			keymap[CL_KEY_S] = down;
			break;
		case 't':
//			if (down) buffer.add_key( CL_KEY_T );
			keymap[CL_KEY_T] = down;
			break;
		case 'u':
//			if (down) buffer.add_key( CL_KEY_U );
			keymap[CL_KEY_U] = down;
			break;
		case 'v':
//			if (down) buffer.add_key( CL_KEY_V );
			keymap[CL_KEY_V] = down;
			break;
		case 'w':
//			if (down) buffer.add_key( CL_KEY_W );
			keymap[CL_KEY_W] = down;
			break;
		case 'x':
//			if (down) buffer.add_key( CL_KEY_X );
			keymap[CL_KEY_X] = down;
			break;
		case 'y':
//			if (down) buffer.add_key( CL_KEY_Y );
			keymap[CL_KEY_Y] = down;
			break;
		case 'z':
//			if (down) buffer.add_key( CL_KEY_Z );
			keymap[CL_KEY_Z] = down;
			break;
	}
}

char *CL_BeKeyboard::get_name() const
{
	return "Be Keyboard";
}

int CL_BeKeyboard::get_num_buttons() const
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_BeKeyboard::get_button(int button_num)
{
	if (buttons[button_num] != NULL) return buttons[button_num];
	
	buttons[button_num] = 
		new CL_InputButton_BeKeyboard(button_num, keymap);

	return buttons[button_num];
}

int CL_BeKeyboard::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_BeKeyboard::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_BeKeyboard::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_BeKeyboard::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_BeKeyboard::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_BeKeyboard::get_cursor(int /*cursor_num*/)
{
	return NULL;
}


/***************************
  CL_InputButton_BeKeyboard
***************************/

CL_InputButton_BeKeyboard::CL_InputButton_BeKeyboard(
	int _key, bool *_keymap)
{
	key = _key;
	keymap = _keymap;
}

bool CL_InputButton_BeKeyboard::is_pressed()
{
	return keymap[key];
}
