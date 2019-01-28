/*
	$Id: inputbuffer.cpp,v 1.6 2002/01/23 11:47:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/key.h"
#include "API/Display/Input/inputbuffer.h"
#include "API/Display/Input/keyboard.h"

CL_InputBuffer::CL_InputBuffer()
{
	device = CL_Input::keyboards[0];

	slot_button_press = CL_Input::sig_button_press().connect(this, &CL_InputBuffer::on_button_press);
	slot_button_release = CL_Input::sig_button_release().connect(this, &CL_InputBuffer::on_button_release);
}

CL_InputBuffer::CL_InputBuffer(CL_InputDevice *device)
{
	this->device = device;

	slot_button_press = CL_Input::sig_button_press().connect(this, &CL_InputBuffer::on_button_press);
	slot_button_release = CL_Input::sig_button_release().connect(this, &CL_InputBuffer::on_button_release);
}

CL_InputBuffer::CL_InputBuffer(
	CL_Signal_v4<CL_Component *, CL_InputDevice *, CL_Key, bool &> &sig_key_down,
	CL_Signal_v3<CL_Component *, CL_InputDevice *, CL_Key> &sig_key_up)
{
	device = CL_Input::keyboards[0];

	slot_key_down = sig_key_down.connect(this, &CL_InputBuffer::on_key_down);
	slot_key_up = sig_key_up.connect(this, &CL_InputBuffer::on_key_up);
}

CL_InputBuffer::~CL_InputBuffer()
{
}

CL_Key CL_InputBuffer::peek_key() const
{
	if (buffer.empty())
	{
		CL_Key key;
		key.state = CL_Key::NoKey;
		return key;
	}

	return buffer.front();
}

CL_Key CL_InputBuffer::get_key()
{
	if (buffer.empty())
	{
		CL_Key key;
		key.state = CL_Key::NoKey;
		return key;
	}
	
	CL_Key key = buffer.front();
	buffer.pop();
	return key;
}

int CL_InputBuffer::keys_left()
{
	return buffer.size();
}

void CL_InputBuffer::clear()
{
	while (!buffer.empty()) buffer.pop();
}

void CL_InputBuffer::on_button_press(
	CL_InputDevice *device,
	const CL_Key &key)
{
	if (this->device != device) return; // wrong device.

	buffer.push(key);
}

void CL_InputBuffer::on_button_release(
	CL_InputDevice *device,
	const CL_Key &key)
{
	if (this->device != device) return; // wrong device.

	buffer.push(key);
}

void CL_InputBuffer::on_key_down(CL_Component *, CL_InputDevice *device, CL_Key key, bool &retval)
{
	if (this->device != device) return; // wrong device.
	buffer.push(key);
}

void CL_InputBuffer::on_key_up(CL_Component *, CL_InputDevice *device, CL_Key key)
{
	if (this->device != device) return; // wrong device.
	buffer.push(key);
}
