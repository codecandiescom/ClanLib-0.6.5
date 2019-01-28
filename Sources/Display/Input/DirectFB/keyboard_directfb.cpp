/*
	$Id: keyboard_directfb.cpp,v 1.7 2002/06/07 19:36:33 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_DIRECTFB

#include <directfb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "keyboard_directfb.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/key.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include <API/Display/Input/keyboard.h>
#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"

#include "Core/System/Generic/string_asm.h"

/******************
  Keyboard
******************/

CL_DirectFBKeyboard::CL_DirectFBKeyboard (unsigned int id)
{
  DFBResult ret;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_DirectFBKeyboard - DirectFBCreate", ret);
      throw new CL_Error ("CL_DirectFBKeyboard - DirectFBCreate failed");
    }

  ret = dfb->GetInputDevice (dfb, id, &keyboard);
  if (ret)
    {
      DirectFBError ("CL_DirectFBKeyboard - GetInputDevice", ret);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFBKeyboard - GetInputDevice failed");
    }

  ret = keyboard->CreateEventBuffer (keyboard, &keybuffer);
  if (ret)
    {
      DirectFBError ("CL_DirectFBKeyboard - CreateEventBuffer", ret);
      keyboard->Release (keyboard);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFBKeyboard - CreateEventBuffer failed");
    }

  buttons = new CL_InputButton_DirectFBKeyboard*[CL_NUM_KEYS];
  for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;

  this->id = id;
}

CL_DirectFBKeyboard::~CL_DirectFBKeyboard()
{
  for (int i=0; i<CL_NUM_KEYS; i++)
    if (buttons[i])
      delete buttons[i];

  delete[] buttons;

  keybuffer->Release (keybuffer);
  keyboard->Release (keyboard);
  dfb->Release (dfb);
}

void CL_DirectFBKeyboard::keep_alive()
{
  DFBInputEvent evt;

  while (keybuffer->GetEvent (keybuffer, DFB_EVENT(&evt)) == DFB_OK)
    {
      if (evt.type == DIET_KEYPRESS  ||  evt.type == DIET_KEYRELEASE)
	{
	  CL_Key key;
		
	  key.x = key.y = -1; // since the keyboard is no pointing device

	  key.id = map_directfb_to_cl (evt.key_id);
	  key.ascii = (evt.key_symbol > 0 && evt.key_symbol < 0x100) ? : 0;
		
	  if (evt.type == DIET_KEYRELEASE)
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
    }
}

char *CL_DirectFBKeyboard::get_name() const
{
  if (id == DIDID_KEYBOARD)
    return "DirectFB Primary Keyboard";
  else
    return "DirectFB Additional Keyboard";
}

CL_InputDevice::InputDeviceType CL_DirectFBKeyboard::get_type() const
{
	return CL_InputDevice::type_keyboard;
}

int CL_DirectFBKeyboard::get_num_buttons() const
{
  return CL_NUM_KEYS;
}

CL_InputButton *CL_DirectFBKeyboard::get_button(int button_num)
{
  if (buttons[button_num] != NULL) return buttons[button_num];

  DFBInputDeviceKeyIdentifier translate_key = map_cl_to_directfb (button_num);
  if (translate_key == DIKI_UNKNOWN) return NULL;

  buttons[button_num] = new CL_InputButton_DirectFBKeyboard (keyboard, translate_key);

  return buttons[button_num];
}

int CL_DirectFBKeyboard::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_DirectFBKeyboard::get_axis(int axis_num)
{
	return NULL;
}

int CL_DirectFBKeyboard::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_DirectFBKeyboard::get_hat(int hat_num)
{
	return NULL;
}

int CL_DirectFBKeyboard::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_DirectFBKeyboard::get_cursor(int cursor_num)
{
	return NULL;
}

int CL_DirectFBKeyboard::map_directfb_to_cl (DFBInputDeviceKeyIdentifier key)
{
  switch (key)
    {
    case DIKI_A:   return CL_KEY_A;
    case DIKI_B:   return CL_KEY_B;
    case DIKI_C:   return CL_KEY_C;
    case DIKI_D:   return CL_KEY_D;
    case DIKI_E:   return CL_KEY_E;
    case DIKI_F:   return CL_KEY_F;
    case DIKI_G:   return CL_KEY_G;
    case DIKI_H:   return CL_KEY_H;
    case DIKI_I:   return CL_KEY_I;
    case DIKI_J:   return CL_KEY_J;
    case DIKI_K:   return CL_KEY_K;
    case DIKI_L:   return CL_KEY_L;
    case DIKI_M:   return CL_KEY_M;
    case DIKI_N:   return CL_KEY_N;
    case DIKI_O:   return CL_KEY_O;
    case DIKI_P:   return CL_KEY_P;
    case DIKI_Q:   return CL_KEY_Q;
    case DIKI_R:   return CL_KEY_R;
    case DIKI_S:   return CL_KEY_S;
    case DIKI_T:   return CL_KEY_T;
    case DIKI_U:   return CL_KEY_U;
    case DIKI_V:   return CL_KEY_V;
    case DIKI_W:   return CL_KEY_W;
    case DIKI_X:   return CL_KEY_X;
    case DIKI_Y:   return CL_KEY_Y;
    case DIKI_Z:   return CL_KEY_Z;

    case DIKI_0:   return CL_KEY_0;
    case DIKI_1:   return CL_KEY_1;
    case DIKI_2:   return CL_KEY_2;
    case DIKI_3:   return CL_KEY_3;
    case DIKI_4:   return CL_KEY_4;
    case DIKI_5:   return CL_KEY_5;
    case DIKI_6:   return CL_KEY_6;
    case DIKI_7:   return CL_KEY_7;
    case DIKI_8:   return CL_KEY_8;
    case DIKI_9:   return CL_KEY_9;

    case DIKI_F1:  return CL_KEY_F1;
    case DIKI_F2:  return CL_KEY_F2;
    case DIKI_F3:  return CL_KEY_F3;
    case DIKI_F4:  return CL_KEY_F4;
    case DIKI_F5:  return CL_KEY_F5;
    case DIKI_F6:  return CL_KEY_F6;
    case DIKI_F7:  return CL_KEY_F7;
    case DIKI_F8:  return CL_KEY_F8;
    case DIKI_F9:  return CL_KEY_F9;
    case DIKI_F10: return CL_KEY_F10;
    case DIKI_F11: return CL_KEY_F11;
    case DIKI_F12: return CL_KEY_F12;

    case DIKI_ESCAPE:      return CL_KEY_ESCAPE;
    case DIKI_LEFT:        return CL_KEY_LEFT;
    case DIKI_RIGHT:       return CL_KEY_RIGHT;
    case DIKI_UP:          return CL_KEY_UP;
    case DIKI_DOWN:        return CL_KEY_DOWN;
    case DIKI_CONTROL_L:   return CL_KEY_LCTRL;
    case DIKI_CONTROL_R:   return CL_KEY_RCTRL;
    case DIKI_SHIFT_L:     return CL_KEY_LSHIFT;
    case DIKI_SHIFT_R:     return CL_KEY_RSHIFT;
    case DIKI_ALT_L:       return CL_KEY_ALT;
    case DIKI_ALT_R:       return CL_KEY_ALT;
    case DIKI_ALTGR:       return CL_KEY_ALTGR;
    case DIKI_TAB:         return CL_KEY_TAB;
    case DIKI_ENTER:       return CL_KEY_ENTER;
    case DIKI_SPACE:       return CL_KEY_SPACE;
    case DIKI_BACKSPACE:   return CL_KEY_BACKSPACE;
    case DIKI_INSERT:      return CL_KEY_INSERT;
    case DIKI_DELETE:      return CL_KEY_DELETE;
    case DIKI_HOME:        return CL_KEY_HOME;
    case DIKI_END:         return CL_KEY_END;
    case DIKI_PAGE_UP:     return CL_KEY_PAGEUP;
    case DIKI_PAGE_DOWN:   return CL_KEY_PAGEDOWN;
    case DIKI_CAPS_LOCK:   return CL_KEY_CAPSLOCK;
    case DIKI_NUM_LOCK:    return CL_KEY_NUMLOCK;
    case DIKI_SCROLL_LOCK: return CL_KEY_SCRLOCK;
    case DIKI_PRINT:       return CL_KEY_PRINT;
    case DIKI_PAUSE:       return CL_KEY_PAUSE;
    case DIKI_KP_DIV:      return CL_KEY_KP_DIV;
    case DIKI_KP_MULT:     return CL_KEY_KP_MULT;
    case DIKI_KP_MINUS:    return CL_KEY_KP_MINUS;
    case DIKI_KP_PLUS:     return CL_KEY_KP_PLUS;
    case DIKI_KP_ENTER:    return CL_KEY_KP_ENTER;

    default:
      break;
    }

  return CL_KEY_NONE_OF_THE_ABOVE;
}

DFBInputDeviceKeyIdentifier CL_DirectFBKeyboard::map_cl_to_directfb(int id)
{
  switch (id)
    {
    case CL_KEY_A:   return DIKI_A;
    case CL_KEY_B:   return DIKI_B;
    case CL_KEY_C:   return DIKI_C;
    case CL_KEY_D:   return DIKI_D;
    case CL_KEY_E:   return DIKI_E;
    case CL_KEY_F:   return DIKI_F;
    case CL_KEY_G:   return DIKI_G;
    case CL_KEY_H:   return DIKI_H;
    case CL_KEY_I:   return DIKI_I;
    case CL_KEY_J:   return DIKI_J;
    case CL_KEY_K:   return DIKI_K;
    case CL_KEY_L:   return DIKI_L;
    case CL_KEY_M:   return DIKI_M;
    case CL_KEY_N:   return DIKI_N;
    case CL_KEY_O:   return DIKI_O;
    case CL_KEY_P:   return DIKI_P;
    case CL_KEY_Q:   return DIKI_Q;
    case CL_KEY_R:   return DIKI_R;
    case CL_KEY_S:   return DIKI_S;
    case CL_KEY_T:   return DIKI_T;
    case CL_KEY_U:   return DIKI_U;
    case CL_KEY_V:   return DIKI_V;
    case CL_KEY_W:   return DIKI_W;
    case CL_KEY_X:   return DIKI_X;
    case CL_KEY_Y:   return DIKI_Y;
    case CL_KEY_Z:   return DIKI_Z;

    case CL_KEY_0:   return DIKI_0;
    case CL_KEY_1:   return DIKI_1;
    case CL_KEY_2:   return DIKI_2;
    case CL_KEY_3:   return DIKI_3;
    case CL_KEY_4:   return DIKI_4;
    case CL_KEY_5:   return DIKI_5;
    case CL_KEY_6:   return DIKI_6;
    case CL_KEY_7:   return DIKI_7;
    case CL_KEY_8:   return DIKI_8;
    case CL_KEY_9:   return DIKI_9;

    case CL_KEY_F1:  return DIKI_F1;
    case CL_KEY_F2:  return DIKI_F2;
    case CL_KEY_F3:  return DIKI_F3;
    case CL_KEY_F4:  return DIKI_F4;
    case CL_KEY_F5:  return DIKI_F5;
    case CL_KEY_F6:  return DIKI_F6;
    case CL_KEY_F7:  return DIKI_F7;
    case CL_KEY_F8:  return DIKI_F8;
    case CL_KEY_F9:  return DIKI_F9;
    case CL_KEY_F10: return DIKI_F10;
    case CL_KEY_F11: return DIKI_F11;
    case CL_KEY_F12: return DIKI_F12;

    case CL_KEY_ESCAPE:    return DIKI_ESCAPE;
    case CL_KEY_LEFT:      return DIKI_LEFT;
    case CL_KEY_RIGHT:     return DIKI_RIGHT;
    case CL_KEY_UP:        return DIKI_UP;
    case CL_KEY_DOWN:      return DIKI_DOWN;
    case CL_KEY_LCTRL:     return DIKI_CONTROL_L;
    case CL_KEY_RCTRL:     return DIKI_CONTROL_R;
    case CL_KEY_LSHIFT:    return DIKI_SHIFT_L;
    case CL_KEY_RSHIFT:    return DIKI_SHIFT_R;
    case CL_KEY_ALT:       return DIKI_ALT_L;
    case CL_KEY_ALTGR:     return DIKI_ALTGR;
    case CL_KEY_TAB:       return DIKI_TAB;
    case CL_KEY_ENTER:     return DIKI_ENTER;
    case CL_KEY_SPACE:     return DIKI_SPACE;
    case CL_KEY_BACKSPACE: return DIKI_BACKSPACE;
    case CL_KEY_INSERT:    return DIKI_INSERT;
    case CL_KEY_DELETE:    return DIKI_DELETE;
    case CL_KEY_HOME:      return DIKI_HOME;
    case CL_KEY_END:       return DIKI_END;
    case CL_KEY_PAGEUP:    return DIKI_PAGE_UP;
    case CL_KEY_PAGEDOWN:  return DIKI_PAGE_DOWN;
    case CL_KEY_CAPSLOCK:  return DIKI_CAPS_LOCK;
    case CL_KEY_NUMLOCK:   return DIKI_NUM_LOCK;
    case CL_KEY_SCRLOCK:   return DIKI_SCROLL_LOCK;
    case CL_KEY_PRINT:     return DIKI_PRINT;
    case CL_KEY_PAUSE:     return DIKI_PAUSE;
    case CL_KEY_KP_DIV:    return DIKI_KP_DIV;
    case CL_KEY_KP_MULT:   return DIKI_KP_MULT;
    case CL_KEY_KP_MINUS:  return DIKI_KP_MINUS;
    case CL_KEY_KP_PLUS:   return DIKI_KP_PLUS;
    case CL_KEY_KP_ENTER:  return DIKI_KP_ENTER;
    }
	
  return DIKI_UNKNOWN;
}

/***************************
  CL_InputButton_XKeyboard
***************************/

CL_InputButton_DirectFBKeyboard::CL_InputButton_DirectFBKeyboard(IDirectFBInputDevice *keyboard,
								 DFBInputDeviceKeyIdentifier key)
{
  keyboard->AddRef (keyboard);

  this->key      = key;
  this->keyboard = keyboard;
}

CL_InputButton_DirectFBKeyboard::~CL_InputButton_DirectFBKeyboard()
{
  keyboard->Release (keyboard);
}

bool CL_InputButton_DirectFBKeyboard::is_pressed()
{
  DFBInputDeviceKeyState state;

  keyboard->GetKeyState (keyboard, key, &state);

  return (state == DIKS_DOWN);
}

#endif
