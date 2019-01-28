/*
	$Id: input_keyboard.h,v 1.2 2001/11/30 22:38:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_input_keyboard
#define header_input_keyboard

#include "API/Display/Input/input.h"
#include "API/Display/Input/inputbutton.h"
#include "Core/System/Win32/init_win32.h"
#include "API/Display/Input/keyboard.h"
#include "API/Core/System/keep_alive.h"

class CL_InputButton_Keyboard_Win32;

class CL_Keyboard_Win32
: public CL_Keyboard,
	public CL_KeepAlive,
	public CL_Win32EventListener
{
protected:
	unsigned char keys_down[256];
	CL_InputButton_Keyboard_Win32 **buttons;

	CL_InputButton *map_button(int key);

public:
	CL_Keyboard_Win32();
	virtual ~CL_Keyboard_Win32();

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

class CL_InputButton_Keyboard_Win32 : public CL_InputButton
{
protected:
	int key;
	unsigned char *keys_down;

public:
	CL_InputButton_Keyboard_Win32(int key, unsigned char *keys_down);
	virtual ~CL_InputButton_Keyboard_Win32();

	virtual bool is_pressed();
};

#endif
