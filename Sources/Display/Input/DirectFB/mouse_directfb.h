/*
	$Id: mouse_directfb.h,v 1.4 2002/05/29 15:53:04 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mouse_directfb
#define header_mouse_directfb

#ifdef USE_DIRECTFB

#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Display/Input/inputcursor.h"
#include "Display/Display/DirectFB/display_directfb.h"
#include "Core/System/Unix/init_linux.h"
#include "API/Core/System/keep_alive.h"

class CL_InputButton_DirectFBMouse : public CL_InputButton
{
public:
  CL_InputButton_DirectFBMouse (IDirectFBInputDevice           *mouse,
				DFBInputDeviceButtonIdentifier  button);
  virtual ~CL_InputButton_DirectFBMouse();

  virtual bool is_pressed();

private:
  IDirectFBInputDevice           *mouse;
  DFBInputDeviceButtonIdentifier  button;
};

class CL_InputCursor_DirectFBMouse : public CL_InputCursor
{
 public:
  CL_InputCursor_DirectFBMouse();
  virtual ~CL_InputCursor_DirectFBMouse();

  virtual float get_x();
  virtual float get_y();
  virtual float get_max_x();
  virtual float get_max_y();

  void set_position(float x, float y) {};

 private:
  IDirectFB *dfb;
  IDirectFBDisplayLayer *layer;
};

class CL_InputAxis_DirectFBMouse : public CL_InputAxis
{
friend class CL_DirectFBMouse;

public:
  CL_InputAxis_DirectFBMouse (IDirectFBInputDevice         *mouse,
			      DFBInputDeviceAxisIdentifier  axis);
  virtual ~CL_InputAxis_DirectFBMouse();

  virtual float get_pos();

private:
  IDirectFBInputDevice         *mouse;
  DFBInputDeviceAxisIdentifier  axis;
};

class CL_DirectFBMouse : public CL_InputDevice, CL_KeepAlive
{
 protected:
  DFBInputDeviceDescription      desc;
  CL_InputButton_DirectFBMouse **buttons;
  CL_InputAxis_DirectFBMouse   **axes;
  CL_InputCursor_DirectFBMouse  *cursor;

 public:
  CL_DirectFBMouse (unsigned int id);
  virtual ~CL_DirectFBMouse();

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
  unsigned int id;

  IDirectFB *dfb;
  IDirectFBInputDevice *mouse;
  IDirectFBEventBuffer *mousebuffer;
};

#endif /* USE_DIRECTFB */

#endif
