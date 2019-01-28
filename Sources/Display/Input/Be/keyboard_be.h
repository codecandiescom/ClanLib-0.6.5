/*
	$Id: keyboard_be.h,v 1.1 2001/03/06 15:09:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_be
#define header_keyboard_be

#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"
#include "Core/System/Be/app_beos.h"
#include "API/Core/System/keep_alive.h"

#include <Message.h>

class CL_InputButton_BeKeyboard : public CL_InputButton
{
public:
	CL_InputButton_BeKeyboard(int key, bool *keymap);
	bool is_pressed();

private:
	int key;
	bool *keymap;
};

class CL_BeKeyboard : public CL_Keyboard, CL_KeepAlive
{
public:
	CL_BeKeyboard();
	virtual ~CL_BeKeyboard();

	virtual char *get_name() const;

	virtual int get_num_buttons() const;
	virtual CL_InputButton *get_button(int button_num);

	virtual int get_num_axes() const;
	virtual CL_InputAxis *get_axis(int axis_num);

	virtual int get_num_hats() const;
	virtual CL_InputHat *get_hat(int hat_num);

	virtual int get_num_cursors() const;
	virtual CL_InputCursor *get_cursor(int cursor_num);

	void handle_key( BMessage* msg, bool down );
	void handle_modifiers( BMessage* msg );

private:
	bool keymap[128];
	
	char translate(int kb_value);
	
	CL_InputButton_BeKeyboard **buttons;
//	CL_Array<CL_InputButton_BeKeyboard> buttons;
};

#endif
