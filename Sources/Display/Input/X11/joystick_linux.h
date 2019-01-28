/*
	$Id: joystick_linux.h,v 1.3 2001/12/22 17:03:35 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	TODO: Move update() to a keep alive interface.
	      Add joystick hat support.
	      Calibrate joystick somehow.
	      Create a native X11 joystick driver too.
*/

#ifndef header_joystick_linux
#define header_joystick_linux
#ifdef USE_JOY
// TODO: ifdef this out if it isn't a linux system.

#include <linux/version.h>

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#error "You need to use at least 2.0 Linux kernel." 
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)


#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Core/System/keep_alive.h"
#include "Core/System/Unix/init_linux.h"

//#ifdef USE_JOY

#include <linux/joystick.h>

//#endif

class CL_LinuxJoystick_Axis;
class CL_LinuxJoystick_Button;

class CL_LinuxJoystick : public CL_InputDevice, CL_KeepAlive
{
public:
	CL_LinuxJoystick();
	virtual ~CL_LinuxJoystick();
	bool init(int number = 0);
	
	// CL_InputDevice functions:
	// -------------------------

	virtual char *get_name() const { return "Linux Joystick"; }

	virtual InputDeviceType get_type() const { return CL_InputDevice::type_joystick; }

	virtual int get_num_axes() const { return num_axes; }
	virtual CL_InputAxis *get_axis(int num);

	virtual int get_num_buttons() const { return num_buttons; }
	virtual CL_InputButton *get_button(int num);

	virtual int get_num_hats() const { return 0; }
	virtual CL_InputHat *get_hat(int) { return NULL; }
	
	virtual int get_num_buffers() const { return 0; }
	virtual CL_InputBuffer *get_buffer(int) { return NULL; }
	
	virtual int get_num_cursors() const { return 0; }
	virtual CL_InputCursor *get_cursor(int) { return NULL; }

	// CL_KeepAlive:
	// --------------------
	virtual void keep_alive();
private:

	int fd;
	
	int num_buttons;
	int num_axes;
	
	js_event jev;

	CL_LinuxJoystick_Axis *axes;
	CL_LinuxJoystick_Button *buttons;
};

class CL_LinuxJoystick_Axis : public CL_InputAxis
{
public:
	CL_LinuxJoystick_Axis() { value = 0; }

	virtual float get_pos() { return value/(float) 32768; }
	void set_value(int v) { value = v; }
private:
	int value;
};

class CL_LinuxJoystick_Button : public CL_InputButton
{
public:
	CL_LinuxJoystick_Button() { value = 0; }

	virtual bool is_pressed() { if (value) return true; return false; }
	void set_value(int v) { value = v; }
private:
	int value;
};

#endif

#endif
#endif
