/*
	$Id: directfb_target.cpp,v 1.6 2002/08/23 10:13:19 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "directfb_target.h"
#include "API/Core/System/error.h"
#include "API/Display/Display/display.h"
#include "API/Display/Input/input.h"
#include "Display/Display/DirectFB/display_directfb.h"
#include "Display/Input/DirectFB/keyboard_directfb.h"
#include "Display/Input/DirectFB/mouse_directfb.h"

DFBEnumerationResult inputdevice_callback( unsigned int id, DFBInputDeviceDescription desc, void *data )
{
  if (desc.type & DIDTF_KEYBOARD && id != DIDID_KEYBOARD)
    CL_Input::keyboards.push_back (new CL_DirectFBKeyboard(id));

  if (desc.type & DIDTF_MOUSE)
    CL_Input::pointers.push_back (new CL_DirectFBMouse(id));

#if 0
  if (desc.type & DIDTF_JOYSTICK)
    CL_Input::joysticks.push_back (new CL_DirectFBJoystick(id));
#endif

  return DFENUM_OK;
}



const char *CL_DirectFBTarget::get_long_name()
{
  return "DirectFB";
}

const char *CL_DirectFBTarget::get_short_name()
{
  return "directfb";
}

void CL_DirectFBTarget::init()
{
  DFBResult ret;

  ret = DirectFBInit (NULL, NULL);
  if (ret)
    {
      DirectFBError ("CL_DirectFBTarget - DirectFBInit", ret);
      throw new CL_Error ("CL_DirectFBTarget - DirectFBInit failed");
    }

  ret = DirectFBCreate (&dfb);
  if (ret)
    {
      DirectFBError ("CL_DirectFBTarget - DirectFBCreate", ret);
      throw new CL_Error ("CL_DirectFBTarget - DirectFBCreate failed");
    }

  /* Make sure the primary keyboard is added first */
  CL_Input::keyboards.push_back (new CL_DirectFBKeyboard(DIDID_KEYBOARD));

  ret = dfb->EnumInputDevices (dfb, inputdevice_callback, this);
  if (ret)
    {
      DirectFBError ("CL_DirectFBTarget - EnumInputDevices", ret);
      dfb->Release (dfb);
      throw new CL_Error ("CL_DirectFBTarget - EnumInputDevices failed");
    }
  
  CL_DirectFB_DisplayCard *card = new CL_DirectFB_DisplayCard (0);
  CL_Display::cards.push_back(card);
}

void CL_DirectFBTarget::deinit()
{
  dfb->Release (dfb);
}
