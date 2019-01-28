/*
	$Id: keyboard_x11.h,v 1.3 2001/11/30 22:38:42 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_x11
#define header_keyboard_x11

#ifdef USE_X11

#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"
#include "Core/System/Unix/init_linux.h"
#include "API/Core/System/keep_alive.h"
#include "API/signals.h"

// Don't change this; hardcoded in X
#define X_KEYBOARD_BUFFERSIZE 32

class CL_InputButton_XKeyboard : public CL_InputButton
{
public:
	CL_InputButton_XKeyboard(int key, char *keys_return);
	virtual bool is_pressed();

private:
	int key;
	char *keys_return;
};

class CL_XWindow_CompatibleCard;
class CL_XWindowKeyboard : public CL_Keyboard, CL_KeepAlive
{
public:
	CL_XWindowKeyboard(CL_XWindow_CompatibleCard *card);
	virtual ~CL_XWindowKeyboard();

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
	static int map_keysym_to_id(int keysym);
	static int map_id_to_keysym(int id);
	
	void on_xevent(XEvent &event);
	CL_Slot slot_xevent;

	char keys_return[X_KEYBOARD_BUFFERSIZE];
	
	CL_InputButton_XKeyboard **buttons;
	CL_XWindow_CompatibleCard *card;
};

#endif /* USE_X11 */

#endif
