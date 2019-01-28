/*
	$Id: inputdevice.h,v 1.7 2001/12/22 17:03:33 sphair Exp $

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

#ifndef header_inputdevice
#define header_inputdevice

class CL_InputButton;
class CL_InputAxis;
class CL_InputHat;
class CL_InputBuffer;
class CL_InputCursor;

//: Input device interface.
//- <p>Class interface representing an input device, such as a keyboard, mouse,
//- joystick, gamepad or digitizer.</p>
class CL_InputDevice
{
public:
//! Enums:
	enum InputDeviceType
	{
		type_joystick,
		type_mouse,
		type_keyboard
	};

public:
//! Construction:
	//: Input Device Destructor
	virtual ~CL_InputDevice() { return; }

//! Attributes:
	//: Returns the name of the input device.
	//- Returns - Name of the device.
	virtual char *get_name() const =0;

	//: Return the type of the inputdevice.
	virtual InputDeviceType get_type() const =0;

	//: Returns the number of buttons on this device.
	//- Returns - Return the number of buttons available.
	virtual int get_num_buttons() const =0;

	//: Returns a input button interface used to access the specified button number. 
	//: NULL is returned if button doesn't exist.
	//- button_num - Number of the button to be returned.
	//- Returns - Pointer to the button requested (or NULL if none).
	virtual CL_InputButton *get_button(int button_num)=0;

	//: Returns the number of axes on this device.
	//- Returns - Number of axes available.
	virtual int get_num_axes() const =0;

	//: Returns a input axis interface used to access the specified axis number. 
	//: NULL is returned if axis doesn't exist.
	//- axis_num - Number of the axis to be returned.
	//- Returns - Pointer to axis requested (or NULL if none).
	virtual CL_InputAxis *get_axis(int axis_num)=0;

	//: Returns the number of hats available on this device.
	//- Returns - Number of input hats available.
	virtual int get_num_hats() const =0;
	
	//: Returns a input hat interface used to access the specified hat number. 
	//: NULL is returned if the hat doesn't exist.
	//- hat_num - Number of the hat to be returned.
	//- Returns - Pointer to input hat requested (or NULL if none).
	virtual CL_InputHat *get_hat(int hat_num)=0;

	//: Returns the number of input cursors available on this device.
	//- Returns - Number of cursors available.
	virtual int get_num_cursors() const =0;

	//: <p>Returns a input cursor interface used to access the specified cursor 
	//: number. NULL is returned if the cursor doesn't exist.</p>
	//- cursor_num - Number of the cursor to be returned.
	//- Returns - Pointer to cursor requested (or NULL if none).
	virtual CL_InputCursor *get_cursor(int cursor_num)=0;
};

#endif
