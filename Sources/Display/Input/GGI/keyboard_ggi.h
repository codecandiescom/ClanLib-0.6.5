/*
	$Id: keyboard_ggi.h,v 1.2 2001/11/30 22:38:40 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_ggi
#define header_keyboard_ggi


#ifdef USE_GGI

#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"
#include "API/Core/System/keep_alive.h"
#include "Core/System/Unix/init_linux.h"
#include "Display/Display/GGI/display_ggi.h"

#include <ggi/ggi.h>

class CL_InputButton_GGIKeyboard : public CL_InputButton
{
public:
	CL_InputButton_GGIKeyboard(int key, char *keymap);
	bool is_pressed();

private:
	int key;
	char *keymap;
};

class CL_GGIKeyboard : public CL_Keyboard, CL_KeepAlive
{
public:
	CL_GGIKeyboard(CL_GGI_DisplayCard* card);
	virtual ~CL_GGIKeyboard();

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
	char keymap[128];
	ggi_visual_t m_vis;
	
	void handle_event(gii_event* event);
	char translate(int kb_value);
	char translate_ascii(int kb_value);
	
	CL_InputButton_GGIKeyboard **buttons;
};

#endif /* USE_GGI */

#endif
