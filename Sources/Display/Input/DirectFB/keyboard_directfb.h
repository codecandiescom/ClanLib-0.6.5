/*
	$Id: keyboard_directfb.h,v 1.3 2002/02/26 20:38:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_directfb
#define header_keyboard_directfb

#ifdef USE_DIRECTFB

#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"
#include "Core/System/Unix/init_linux.h"
#include "API/Core/System/keep_alive.h"
#include "API/signals.h"


class CL_InputButton_DirectFBKeyboard : public CL_InputButton
{
public:
  CL_InputButton_DirectFBKeyboard (IDirectFBInputDevice        *keyboard,
				   DFBInputDeviceKeyIdentifier  key);
  virtual ~CL_InputButton_DirectFBKeyboard();

  virtual bool is_pressed();

private:
  IDirectFBInputDevice        *keyboard;
  DFBInputDeviceKeyIdentifier  key;
};

class CL_DirectFBKeyboard : public CL_Keyboard, CL_KeepAlive
{
 public:
  CL_DirectFBKeyboard (unsigned int id);
  virtual ~CL_DirectFBKeyboard();

  virtual char *get_name() const;

  virtual InputDeviceType get_type() const;

  virtual int get_num_buttons() const;
  virtual CL_InputButton *get_button(int button_num);

  virtual int get_num_axes() const;
  virtual CL_InputAxis *get_axis(int axis_num);

  virtual int get_num_hats() const;
  virtual CL_InputHat *get_hat(int hat_num);

  virtual int get_num_cursors() const;
  virtual CL_InputCursor *get_cursor(int cursor_num);

  virtual void keep_alive();

 private:
  DFBInputDeviceKeyIdentifier map_cl_to_directfb (int id);
  int map_directfb_to_cl (DFBInputDeviceKeyIdentifier key);

  CL_InputButton_DirectFBKeyboard **buttons;

  unsigned int id;

  IDirectFB *dfb;
  IDirectFBInputDevice *keyboard;
  IDirectFBEventBuffer *keybuffer;
};

#endif /* USE_DIRECTFB */

#endif
