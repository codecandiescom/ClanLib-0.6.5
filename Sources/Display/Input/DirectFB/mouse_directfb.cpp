/*
	$Id: mouse_directfb.cpp,v 1.8 2002/02/26 20:38:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_DIRECTFB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "API/Core/System/cl_assert.h"

#include "mouse_directfb.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/mouse.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/key.h>
#include <API/Display/Input/inputhat.h>
#include <Display/Input/DirectFB/mouse_directfb.h>
#include <Display/Display/DirectFB/display_directfb.h>

/*************************************
  CL_DirectFBMouse
*************************************/

CL_DirectFBMouse::CL_DirectFBMouse (unsigned int id)
{
  DFBResult ret;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_DirectFBMouse - DirectFBCreate", ret);
      throw new CL_Error ("CL_DirectFBMouse - DirectFBCreate failed");
    }

  ret = dfb->GetInputDevice (dfb, id, &mouse);
  if (ret)
    {
      DirectFBError ("CL_DirectFBMouse - GetInputDevice", ret);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFBMouse - GetInputDevice failed");
    }

  ret = mouse->CreateEventBuffer (mouse, &mousebuffer);
  if (ret)
    {
      DirectFBError ("CL_DirectFBMouse - CreateEventBuffer", ret);
      mouse->Release (mouse);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFBMouse - CreateEventBuffer failed");
    }

  mouse->GetDescription (mouse, &desc);


  cursor = new CL_InputCursor_DirectFBMouse();

  axes = new CL_InputAxis_DirectFBMouse*[desc.max_axis+1];
  for (int i=0; i<=desc.max_axis; i++)
    axes[i] = new CL_InputAxis_DirectFBMouse (mouse, (DFBInputDeviceAxisIdentifier) i);

  buttons = new CL_InputButton_DirectFBMouse*[desc.max_button+1];
  for (int i=0; i<=desc.max_button; i++)
    buttons[i] = new CL_InputButton_DirectFBMouse (mouse, (DFBInputDeviceButtonIdentifier) i);

  this->id = id;
}

CL_DirectFBMouse::~CL_DirectFBMouse()
{
  delete cursor;

  for (int i=0; i<=desc.max_button; i++)
    delete buttons[i];
  delete[] buttons;

  for (int i=0; i<=desc.max_axis; i++)
    delete axes[i];
  delete[] axes;
}


char *CL_DirectFBMouse::get_name() const
{
  if (id == DIDID_MOUSE)
    return "DirectFB Primary Mouse";
  else
    return "DirectFB Additional Mouse";
}

CL_InputDevice::InputDeviceType CL_DirectFBMouse::get_type() const
{
	return CL_InputDevice::type_mouse;
}

int CL_DirectFBMouse::get_num_buttons() const
{
  return desc.max_button + 1;
}

CL_InputButton *CL_DirectFBMouse::get_button(int button_num)
{
  if (button_num < 0 || button_num > desc.max_button)
    return NULL;

  return buttons[button_num];
}

int CL_DirectFBMouse::get_num_axes() const
{
  return desc.max_axis + 1;
}

CL_InputAxis *CL_DirectFBMouse::get_axis(int axis_num)
{
  if (axis_num < 0 || axis_num > desc.max_axis)
    return NULL;

  return axes[axis_num];
}

int CL_DirectFBMouse::get_num_hats() const
{
  return 0;
}

CL_InputHat *CL_DirectFBMouse::get_hat(int hat_num)
{
  return NULL;
}

int CL_DirectFBMouse::get_num_cursors() const
{
  return 1;
}

void CL_DirectFBMouse::keep_alive()
{
  DFBInputEvent evt;

  while (mousebuffer->GetEvent (mousebuffer, DFB_EVENT(&evt)) == DFB_OK)
    {
      switch (evt.type)
	{
	case DIET_BUTTONPRESS:
	  CL_Input::sig_button_press()(
				     this,
				     CL_Key(
					    evt.button,
					    CL_Key::Pressed,
					    -1,
					    cursor->get_x(),
					    cursor->get_y()));
	  CL_Mouse::sig_button_press()(
				     CL_Key(
					    evt.button,
					    CL_Key::Pressed,
					    -1,
					    cursor->get_x(),
					    cursor->get_y()));
	  break;
	case DIET_BUTTONRELEASE:
	  CL_Input::sig_button_press()(
				     this,
				     CL_Key(
					    evt.button,
					    CL_Key::Released,
					    -1,
					    cursor->get_x(),
					    cursor->get_y()));
	  CL_Mouse::sig_button_press()(
				     CL_Key(
					    evt.button,
					    CL_Key::Released,
					    -1,
					    cursor->get_x(),
					    cursor->get_y()));
	  break;
	case DIET_AXISMOTION:
	  CL_Mouse::sig_move()((int)cursor->get_x(), (int)cursor->get_y());
	  CL_Input::sig_mouse_move()(this, (int)cursor->get_x(), (int)cursor->get_y());
	  break;
	default:
	  break;
	}
    }
}

CL_InputCursor *CL_DirectFBMouse::get_cursor(int cursor_num)
{
  return cursor;
}

/*******************************
  CL_InputButton_DirectFBMouse
*******************************/

CL_InputButton_DirectFBMouse::CL_InputButton_DirectFBMouse (IDirectFBInputDevice *mouse,
							    DFBInputDeviceButtonIdentifier button)
{
  mouse->AddRef (mouse);

  this->mouse  = mouse;
  this->button = button;
}

CL_InputButton_DirectFBMouse::~CL_InputButton_DirectFBMouse()
{
  mouse->Release (mouse);
}

bool CL_InputButton_DirectFBMouse::is_pressed()
{
  DFBInputDeviceButtonState state;

  mouse->GetButtonState (mouse, button, &state);

  return (state == DIBS_DOWN);
}

/*******************************
  CL_InputCursor_DirectFBMouse
*******************************/

CL_InputCursor_DirectFBMouse::CL_InputCursor_DirectFBMouse()
{
  DFBResult ret;

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_InputCursor_DirectFBMouse - DirectFBCreate", ret);
      throw new CL_Error ("CL_InputCursor_DirectFBMouse - DirectFBCreate failed");
    }

  ret = dfb->GetDisplayLayer (dfb, DLID_PRIMARY, &layer);
  if (ret)
    {
      DirectFBError ("CL_InputCursor_DirectFBMouse - GetDisplayLayer", ret);
      dfb->Release (dfb);
      throw new CL_Error ("CL_InputCursor_DirectFBMouse - GetDisplayLayer failed");
    }
  
}

CL_InputCursor_DirectFBMouse::~CL_InputCursor_DirectFBMouse()
{
  layer->Release (layer);
  dfb->Release (dfb);
}

float CL_InputCursor_DirectFBMouse::get_x()
{
  int x;

  layer->GetCursorPosition (layer, &x, NULL);

  return x;
}

float CL_InputCursor_DirectFBMouse::get_y()
{
  int y;

  layer->GetCursorPosition (layer, NULL, &y);

  return y;
}

float CL_InputCursor_DirectFBMouse::get_max_x()
{
  DFBDisplayLayerConfig dlc;

  layer->GetConfiguration (layer, &dlc);

  return dlc.width;
}

float CL_InputCursor_DirectFBMouse::get_max_y()
{
  DFBDisplayLayerConfig dlc;

  layer->GetConfiguration (layer, &dlc);

  return dlc.height;
}

/*******************************
  CL_InputAxis_DirectFBMouse
*******************************/

CL_InputAxis_DirectFBMouse::CL_InputAxis_DirectFBMouse(IDirectFBInputDevice         *mouse,
						       DFBInputDeviceAxisIdentifier  axis)
{
  mouse->AddRef (mouse);

  this->mouse = mouse;
  this->axis  = axis;
}

CL_InputAxis_DirectFBMouse::~CL_InputAxis_DirectFBMouse()
{
  mouse->Release (mouse);
}


float CL_InputAxis_DirectFBMouse::get_pos()
{
  int pos;

  mouse->GetAxis (mouse, axis, &pos);

  return pos;
}

#endif
