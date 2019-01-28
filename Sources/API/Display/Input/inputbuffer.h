/*
	$Id: inputbuffer.h,v 1.9 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_inputbuffer
#define header_inputbuffer

#include <queue>
#include "key.h"
#include "../../signals.h"

class CL_Component;
class CL_InputDevice;

//: Input buffer interface.
//- Use this class to read buffered keys from the keyboard (or any other device).
class CL_InputBuffer
{
public:
//! Construction:
	//: Constructs an input buffer that attaches itself to the first keyboard.
	CL_InputBuffer();

	//: Constructs an input buffer that attaches itself to the specified device.
	CL_InputBuffer(CL_InputDevice *device);
	
	//: Constructs an input buffer that attaches itself to the specified signals.
	//: Use it to buffer input from a GUI component.
	CL_InputBuffer(
		CL_Signal_v4<CL_Component *, CL_InputDevice *, CL_Key, bool &> &sig_key_down,
		CL_Signal_v3<CL_Component *, CL_InputDevice *, CL_Key> &sig_key_up);

	//: Input Buffer Destructor
	virtual ~CL_InputBuffer();

//! Operations:
	//: Returns the next key in the buffer, but unlike get_key() it does not
	//: pop it from the input buffer.
	CL_Key peek_key() const;

	//: Pops the next key in the buffer and returns it.
	CL_Key get_key();

	//: Returns number of keys still available in the buffer.
	//- Returns - Number of key values lefts in buffer.
	int keys_left();

	//: Clears the buffer. After this, the buffer will not contain any keys.
	void clear();

//! Implementation:
private:
	CL_Slot slot_button_press;
	CL_Slot slot_button_release;
	CL_Slot slot_key_down;
	CL_Slot slot_key_up;

	void on_button_press(CL_InputDevice *device, const CL_Key &key);
	void on_button_release(CL_InputDevice *device, const CL_Key &key);

	void on_key_down(CL_Component *, CL_InputDevice *device, CL_Key key, bool &retval);
	void on_key_up(CL_Component *, CL_InputDevice *device, CL_Key key);

	std::queue<CL_Key> buffer;
	CL_InputDevice *device;
};

#endif
