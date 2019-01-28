/*
	$Id: keyboard_svgalib.h,v 1.1 2001/03/06 15:09:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_keyboard_svgalib
#define header_keyboard_svgalib

#ifdef USE_SVGALIB

#include "API/Display/Input/input.h"
#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/keyboard.h"

class CL_InputButton_SVGAKeyboard : public CL_InputButton
{
protected:
	int key;

public:
	CL_InputButton_SVGAKeyboard(int _key);
	virtual bool is_pressed();
};

class CL_SVGAKeyboard : public CL_Keyboard
{
protected:
	CL_Array<CL_InputButton_SVGAKeyboard> buttons;

public:
	CL_SVGAKeyboard();
	virtual ~CL_SVGAKeyboard();

	// CL_Input and CL_Keyboard:
	virtual char *get_name();

	virtual int get_num_buttons();
	virtual CL_InputButton *get_button(int button_num);

	virtual int get_num_axes();
	virtual CL_InputAxis *get_axis(int axis_num);

	virtual int get_num_hats();
	virtual CL_InputHat *get_hat(int hat_num);

	virtual int get_num_cursors();
	virtual CL_InputCursor *get_cursor(int cursor_num);
};

#endif

#endif
