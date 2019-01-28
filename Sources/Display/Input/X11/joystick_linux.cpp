/*
	$Id: joystick_linux.cpp,v 1.1 2001/03/06 15:09:22 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "API/Core/System/cl_assert.h"
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/X11/joystick_linux.h>
#ifdef USE_JOY
#include "joystick_linux.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)

CL_LinuxJoystick::CL_LinuxJoystick()
{
	fd = -1;
	num_buttons = 0;
	num_axes = 0;
	axes = NULL;
	buttons = NULL;
}

CL_LinuxJoystick::~CL_LinuxJoystick()
{
	if (fd != -1)
	{
		close(fd);
	}
	
	delete[] axes;
	delete[] buttons;
}

bool CL_LinuxJoystick::init(int number)
{
	cl_assert(fd == -1); // do not call init twice!

	char devname[10];
	sprintf( devname, "/dev/js%d", number );
	fd = open( devname, O_RDONLY | O_NONBLOCK );
	if (fd == -1) return false; // no joystick available

	ioctl( fd, JSIOCGBUTTONS, &num_buttons );
	ioctl( fd, JSIOCGAXES, &num_axes );
/*
	cout << "Number of axes:     " << num_axes << endl;
	cout << "Number of buttons:  " << num_buttons << endl;
*/
	axes = new CL_LinuxJoystick_Axis[num_axes];
	buttons = new CL_LinuxJoystick_Button[num_buttons];

	return true;
}

void CL_LinuxJoystick::keep_alive()
{	
	cl_assert(fd != -1); // init _MUST_ be called before update!
	
	while (read( fd, &jev, sizeof(js_event) ) != -1)
	{
		switch (jev.type)
		{
		case JS_EVENT_AXIS:
			axes[jev.number].set_value(jev.value);
			break;

		case JS_EVENT_BUTTON:
			buttons[jev.number].set_value(jev.value);
			break;
		}
	}
}

CL_InputAxis *CL_LinuxJoystick::get_axis(int num)
{
	cl_assert(num >= 0); // disallow negative values

	if (num >= num_axes) return NULL;
	return &axes[num];
}

CL_InputButton *CL_LinuxJoystick::get_button(int num)
{
	cl_assert(num >= 0); // disallow negative values

	if (num >= num_buttons) return NULL;
	return &buttons[num];
}
#endif
#endif
