/*
	$Id: key.h,v 1.11 2001/10/18 09:23:04 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Input"
//! header=display.h

#ifndef header_key
#define header_key

//: Key buttons available on a keyboard.
enum
{
	CL_NO_KEY,

	CL_KEY_A, CL_KEY_B, CL_KEY_C, CL_KEY_D, CL_KEY_E, CL_KEY_F, CL_KEY_G,
	CL_KEY_H, CL_KEY_I, CL_KEY_J, CL_KEY_K, CL_KEY_L, CL_KEY_M, CL_KEY_N,
	CL_KEY_O, CL_KEY_P, CL_KEY_Q, CL_KEY_R, CL_KEY_S, CL_KEY_T, CL_KEY_U,
	CL_KEY_V, CL_KEY_W, CL_KEY_X, CL_KEY_Y, CL_KEY_Z, CL_KEY_0, CL_KEY_1,
	CL_KEY_2, CL_KEY_3, CL_KEY_4, CL_KEY_5, CL_KEY_6, CL_KEY_7, CL_KEY_8,
	CL_KEY_9,

	CL_KEY_F1, CL_KEY_F2, CL_KEY_F3, CL_KEY_F4, CL_KEY_F5, CL_KEY_F6,
	CL_KEY_F7, CL_KEY_F8, CL_KEY_F9, CL_KEY_F10, CL_KEY_F11, CL_KEY_F12,

	CL_KEY_ESCAPE, CL_KEY_LEFT, CL_KEY_RIGHT, CL_KEY_UP, CL_KEY_DOWN,
	CL_KEY_LCTRL, CL_KEY_RCTRL, CL_KEY_LSHIFT, CL_KEY_RSHIFT, CL_KEY_ALT,
	CL_KEY_ALTGR, CL_KEY_TAB, CL_KEY_ENTER, CL_KEY_SPACE, CL_KEY_BACKSPACE,
	CL_KEY_INSERT, CL_KEY_DELETE, CL_KEY_HOME, CL_KEY_END, CL_KEY_PAGEUP,
	CL_KEY_PAGEDOWN, CL_KEY_CAPSLOCK, CL_KEY_NUMLOCK, CL_KEY_SCRLOCK,
	CL_KEY_PRINT, CL_KEY_PAUSE, CL_KEY_KP_DIV, CL_KEY_KP_MULT,
	CL_KEY_KP_MINUS, CL_KEY_KP_PLUS, CL_KEY_KP_ENTER,
	
	CL_KEY_KP_DECIMAL, 
	CL_KEY_KP_0, CL_KEY_KP_1, CL_KEY_KP_2, CL_KEY_KP_3, CL_KEY_KP_4, 
	CL_KEY_KP_5, CL_KEY_KP_6, CL_KEY_KP_7, CL_KEY_KP_8, CL_KEY_KP_9,

	CL_KEY_NONE_OF_THE_ABOVE,

	CL_MOUSE_LEFTBUTTON,
	CL_MOUSE_MIDDLEBUTTON,
	CL_MOUSE_RIGHTBUTTON,
	CL_MOUSE_THUMBBUTTON,
	CL_MOUSE_WHEELUP,
	CL_MOUSE_WHEELDOWN,
	CL_MOUSE_WHEELBUTTON,

	CL_NUM_KEYS
};

//: Structure descripting a key event.
//-
//- CL_KEY_BACKSPACE	backspace
//- CL_KEY_TAB       tab
//- CL_KEY_ENTER     return
//- CL_KEY_PAUSE     pause
//- CL_KEY_ESCAPE    escape
//- CL_KEY_SPACE     space
//- CL_KEY_0         0
//- ...
//- CL_KEY_9         9
//- CL_KEY_A         a
//- ...
//- CL_KEY_Z         z
//- CL_KEY_KP_0      keypad 0
//- ...
//- CL_KEY_KP_9      keypad 9
//- CL_KEY_KP_DEV    keypad divide
//- CL_KEY_KP_MULT   keypad multiply
//- CL_KEY_KP_MINUS  keypad minus
//- CL_KEY_KP_PLUS   keypad plus
//- CL_KEY_KP_DECIMAL	keypad decimal point
//- CL_KEY_KP_ENTER  keypad enter
//- CL_KEY_UP        up arrow
//- CL_KEY_DOWN      down arrow
//- CL_KEY_RIGHT     right arrow
//- CL_KEY_LEFT      left arrow
//- CL_KEY_DELETE    delete
//- CL_KEY_INSERT    insert
//- CL_KEY_HOME      home
//- CL_KEY_END       end
//- CL_KEY_PAGEUP    page up
//- CL_KEY_PAGEDOWN  page down
//- CL_KEY_F1        F1
//- ...
//- CL_KEY_F12       F12
//- CL_KEY_NUMLOCK   numlock
//- CL_KEY_CAPSLOCK  capslock
//- CL_KEY_SCRLOCK   scrollock
//- CL_KEY_RSHIFT    right shift
//- CL_KEY_LSHIFT    left shift
//- CL_KEY_RCTRL     right ctrl
//- CL_KEY_LCTRL     left ctrl
//- CL_KEY_ALTGR     right alt / alt gr
//- CL_KEY_ALT       left alt / alt
//- CL_KEY_PRINT     print-screen
//-
//- CL_MOUSE_LEFTBUTTON		mouse left button
//- CL_MOUSE_MIDDLEBUTTON	mouse middle button
//- CL_MOUSE_RIGHTBUTTON		mouse right button
//- CL_MOUSE_THUMBBUTTON		mouse thumb button
//- CL_MOUSE_WHEELUP			mouse wheel up
//- CL_MOUSE_WHEELDOWN		mouse wheel down
//- CL_MOUSE_WHEELBUTTON		mouse wheel click
class CL_Key
{
public:
//! Enums:
	enum State
	{
		NoKey,
		Pressed,
		Released
	};

public:
//! Variables:
	//: Key ID.
	int id;

	//: Ascii value or -1 if not available.
	int ascii;
	
	//: <p>Key state. Was it pressed or released. NoKey is used when there were
	//: no key available in the input buffer.</p>
	State state;

	//: x-coordinate position if a pointing device was the source. -1 if none.
	float x;
	
	//: y-coordinate position if a pointing device was the source. -1 if none.
	float y;

//! Construction:
	//: Key Constructor
	CL_Key()
	{
		id    = CL_NO_KEY;
		state = NoKey;
		ascii = -1;
		x     = -1;
		y     = -1;
	}

	//: Key Constructor
	CL_Key(int l_id, State l_state = NoKey, int l_ascii = -1, float l_x = -1, float l_y = -1)
	{
		id    = l_id;
		state = l_state;
		ascii = l_ascii;
		x     = l_x;
		y     = l_y;
	}
};

#endif
