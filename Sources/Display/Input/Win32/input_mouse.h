/*
	$Id: input_mouse.h,v 1.3 2002/05/17 08:47:58 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_input_mouse
#define header_input_mouse

#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputcursor.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Display/Input/inputbutton.h"
#include "Core/System/Win32/init_win32.h"
#include "API/Core/System/keep_alive.h"

class CL_InputButton_Mouse_Win32;
class CL_InputCursor_Mouse_Win32;
class CL_InputAxis_Mouse_Win32;

class CL_Mouse_Win32 : public CL_InputDevice, public CL_KeepAlive, public CL_Win32EventListener
{
protected:
	CL_InputButton_Mouse_Win32 **buttons;
	CL_InputCursor_Mouse_Win32 *cursor;
	CL_InputAxis_Mouse_Win32 *axes;

public:
	CL_Mouse_Win32();
	virtual ~CL_Mouse_Win32();

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
	virtual bool received_event(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class CL_InputButton_Mouse_Win32 : public CL_InputButton
{
public:
	int key;
	bool button_state;

	CL_InputButton_Mouse_Win32(int key);
	virtual ~CL_InputButton_Mouse_Win32();

	virtual bool is_pressed();
};

class CL_InputCursor_Mouse_Win32 : public CL_InputCursor
{
friend class CL_Mouse_Win32;

protected:
	float x, y;

public:

	CL_InputCursor_Mouse_Win32();
	virtual ~CL_InputCursor_Mouse_Win32();

	virtual float get_x();
	virtual float get_y();
	virtual float get_max_x();
	virtual float get_max_y();
	virtual void  set_position (float x, float y);
};

class CL_InputAxis_Mouse_Win32 : public CL_InputAxis
{
friend class CL_Mouse_Win32;

protected:
	float pos, center;
	
public:
	CL_InputAxis_Mouse_Win32();
	virtual ~CL_InputAxis_Mouse_Win32();

	virtual float get_pos();
};

#endif
