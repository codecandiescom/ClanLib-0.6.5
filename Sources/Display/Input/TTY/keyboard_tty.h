/*
	$Id: keyboard_tty.h,v 1.2 2001/11/30 22:38:41 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_tty
#define header_keyboard_tty

#ifndef USE_TTY_INPUT
#ifdef USE_SVGALIB
#define USE_TTY_INPUT
#else
#ifdef USE_FBDEV
#define USE_TTY_INPUT
#endif
#endif
#endif

#ifdef USE_TTY_INPUT

#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"
#include "Core/System/Unix/init_linux.h"
#include "API/Core/System/keep_alive.h"
#include <termios.h>

class CL_InputButton_TTYKeyboard : public CL_InputButton
{
public:
	CL_InputButton_TTYKeyboard(int key, char *keymap);
	virtual bool is_pressed();

private:
	int key;
	char *keymap;
};

class CL_TTYKeyboard : public CL_Keyboard, CL_KeepAlive
{
public:
	CL_TTYKeyboard();
	virtual ~CL_TTYKeyboard();

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
	void restore();

	char keymap[128];
	
	int fd;
	int old_mode;
	int old_kd;
	struct termios old_termios;
	void handle_code(char code);
	char translate(int kb_value);
	
	CL_InputButton_TTYKeyboard **buttons;
};

#endif

#endif
