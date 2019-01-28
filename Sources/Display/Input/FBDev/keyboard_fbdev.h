/*
	$Id: keyboard_fbdev.h,v 1.1 2001/03/06 15:09:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_fbdev
#define header_keyboard_fbdev

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
#include "API/Core/System/keep_alive.h"
#include "Core/System/Unix/init_linux.h"

#include <termios.h>

class CL_InputButton_FBDevKeyboard : public CL_InputButton
{
public:
	CL_InputButton_FBDevKeyboard(int key, char *keymap);
	virtual bool is_pressed();

private:
	int key;
	char *keymap;
};

class CL_FBDevKeyboard : public CL_Keyboard, CL_KeepAlive
{
public:
	CL_FBDevKeyboard();
	virtual ~CL_FBDevKeyboard();

	virtual char *get_name() const;

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
	
	CL_InputButton_FBDevKeyboard **buttons;
};

#endif /* USE_TTY_INPUT */

#endif
