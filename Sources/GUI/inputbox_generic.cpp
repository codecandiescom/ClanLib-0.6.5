/*
	$Id: inputbox_generic.cpp,v 1.39 2002/03/30 10:35:03 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "inputbox_generic.h"
#include "API/GUI/component_options.h"
#include "API/Display/Input/input.h"
#include "API/Display/Input/inputdevice.h"

CL_InputBox_Generic::CL_InputBox_Generic(
	CL_InputBox *self, 
	const std::string &text,
	bool password_mode,
	bool readonly,
	int maxlength)
: inputbox(self), password_mode(password_mode), read_only(readonly),
  max_length(maxlength), text(text)
{
	ctrl_down = false;
	shift_down = false;

	selecting = false;
	mouse_selecting = false;
	selection_start = 0;
	selection_end = 0;

	cursor_position = 0;

	edited = false;

	slot_set_options = inputbox->sig_set_options().connect(
		this, &CL_InputBox_Generic::on_set_options);
	slot_input_down = inputbox->sig_input_down().connect(
		this, &CL_InputBox_Generic::on_input_down);
	slot_input_up = inputbox->sig_input_up().connect(
		this, &CL_InputBox_Generic::on_input_up);
	slot_mouse_move = inputbox->sig_mouse_move().connect(
		this, &CL_InputBox_Generic::on_mouse_move);
}

void CL_InputBox_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("text"))
		set_text(options.get_value("text"));
	else
		set_text("");

	if(options.exists("passwordmode"))
		password_mode = options.get_value_as_bool("passwordmode");
	else
		password_mode = false;

	if(options.exists("readonly"))
		read_only = options.get_value_as_bool("readonly");
	else
		read_only = false;

	if(options.exists("maxlength"))
		set_max_length(options.get_value_as_int("maxlength"));
	else
		set_max_length(0);
}

void CL_InputBox_Generic::on_input_down(CL_InputDevice *device, const CL_Key &key)
{
	// Start selecting with mouse?
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
		cursor_position = text.size();
		selecting = false;
		selection_start = -1;
		selection_end = -1;
//		cursor_position = get_mouse_position(key.x, key.y);
/*		mouse_selecting = true;
		selecting = true;
		selection_start = cursor_position;
		inputbox->capture_mouse();
*/		return;
	}

	// Keyboard?
	if(device->get_type() != CL_InputDevice::type_keyboard)
		return;

	switch (key.id)
	{
	case CL_KEY_DELETE:
		del();
		break;
	case CL_KEY_BACKSPACE:
		backspace();
		break;
	case CL_KEY_END:
		check_selection();	
		end(selecting);
		break;
	case CL_KEY_HOME:
		check_selection();	
		home(selecting);
		break;
	case CL_KEY_LEFT:
		check_selection();	
//		if (ctrl_down)
//			word_left();
//		else
			move_cursor(-1, selecting);
		break;
	case CL_KEY_RIGHT:
		check_selection();	
//		if (ctrl_down)
//			word_right();
//		else
			move_cursor(1, selecting);
		break;
	case CL_KEY_ENTER:
		sig_return_pressed();
//		inputbox->clear_focus();
		break;
	case CL_KEY_LCTRL:
	case CL_KEY_RCTRL:
		ctrl_down = true;
		break;
	case CL_KEY_LSHIFT:
	case CL_KEY_RSHIFT:
		shift_down = true;
		break;
	default:
		update_text(key);
		break;
	}
}

void CL_InputBox_Generic::on_input_up(CL_InputDevice *device, const CL_Key &key)
{
	// End mouse-selecting
	if(key.id == CL_MOUSE_LEFTBUTTON)
	{
/*		cursor_position = get_mouse_position(key.x, key.y);
		if (mouse_selecting && cursor_position == selection_start)
			selecting = false;
		mouse_selecting = false;
		inputbox->release_mouse();
*/		return;
	}

	switch (key.id)
	{
	case CL_KEY_LCTRL:
	case CL_KEY_RCTRL:
		ctrl_down = false;
		break;
	case CL_KEY_LSHIFT:
	case CL_KEY_RSHIFT:
		shift_down = false;
		break;
	}
}

void CL_InputBox_Generic::on_mouse_move(int x, int y)
{
	if (mouse_selecting)
		set_cursor_position(get_mouse_position(x, y));
}

void CL_InputBox_Generic::check_selection()
{
	if(shift_down)
	{
		if(selecting == false)
		{
			selecting = true;
			selection_start = cursor_position;
		}
	}
	else
	{
		selecting = false;
		selection_start = -1;
		selection_end = -1;
	}
}

void CL_InputBox_Generic::set_text(const std::string &new_text)
{
	// TODO: Check against max_length

	text = new_text;
	selecting = false;
	selection_start = -1;
	selection_end = -1;
	cursor_position = text.size();
}

const std::string &CL_InputBox_Generic::get_marked_text() const
{
	throw CL_Error("CL_InputBox_Generic::get_marked_text() is not implemented");
}

void CL_InputBox_Generic::set_max_length(int length)
{
	max_length = length;
	int text_size = text.size();

	if (text_size > max_length && max_length != 0)
		text.erase(max_length, text.size());
}

void CL_InputBox_Generic::select_all()
{
	throw CL_Error("CL_InputBox_Generic::select_all() is not implemented");
}

void CL_InputBox_Generic::deselect()
{
	throw CL_Error("CL_InputBox_Generic::deselect() is not implemented");
}

void CL_InputBox_Generic::set_selection(int start, int length)
{
	throw CL_Error("CL_InputBox_Generic::set_selection() is not implemented");
}

void CL_InputBox_Generic::set_cursor_position(int pos)
{
	cursor_position = pos;
	if(cursor_position < 0)
		cursor_position = 0;
	if(cursor_position > max_length - 1 && max_length != 0)
		cursor_position = max_length - 1;
}

void CL_InputBox_Generic::del()
{
	if (selecting)
		cut();
	else if (cursor_position < (int)text.size())
	{
		text.erase(cursor_position, 1);
		sig_changed(text);
	}

	sig_activity();
}

void CL_InputBox_Generic::backspace()
{
	if (selecting)
		cut();
	else if (cursor_position > 0)
	{
		text.erase(cursor_position - 1, 1);
		cursor_position--;
		sig_changed(text);
	}

	sig_activity();
}

void CL_InputBox_Generic::home(bool mark)
{
	if(mark)
	{
		selecting = true;
		selection_start = cursor_position;
		selection_end = 0;
	}

	cursor_position = 0;

	sig_activity();
}

void CL_InputBox_Generic::end(bool mark)
{
	if(mark)
	{
		selecting = true;
		selection_start = cursor_position;
		selection_end = text.size();
	}

	cursor_position = text.size();

	sig_activity();
}

void CL_InputBox_Generic::move_cursor(int delta, bool mark)
{
	cursor_position += delta;

	// Check bounds
	if (cursor_position < 0) cursor_position = 0;
	if (cursor_position > (int)text.size()) cursor_position = (int)text.size();

	if(mark)
		selection_end = cursor_position;

	sig_activity();
}

void CL_InputBox_Generic::move_cursor_word(int delta, bool mark)
{
	throw CL_Error("CL_InputBox_Generic::move_cursor_word() is not implemented");
/*
		if (ctrl_down)
		{
			while (caret_position > 0 && text[caret_position] == ' ') caret_position--;
			while (caret_position > 0 && text[caret_position] != ' ') caret_position--;
		}
		if (ctrl_down)
		{
			while (caret_position < (int)text.size() && text[caret_position] == ' ') caret_position++;
			while (caret_position < (int)text.size() && text[caret_position] != ' ') caret_position++;
		}
		sig_activity();
*/
}

void CL_InputBox_Generic::cut()
{
	if(selecting) 
	{
		int start = get_selection_start();
		int end = get_selection_length();
		text.erase(start, end);
		cursor_position = start;

		selecting = false;
		selection_start = selection_end = -1;

		if (cursor_position > (int)text.size())
			cursor_position = (int)text.size();

		sig_changed(text);
	}
}

int CL_InputBox_Generic::get_selection_start()
{
	if(selection_start < selection_end)
		return selection_start;
	else
		return selection_end;
}

int CL_InputBox_Generic::get_selection_length()
{
	return abs(selection_start - selection_end);
}

void CL_InputBox_Generic::update_text(CL_Key key)
{
	if (key.ascii >= 32)
	{
		cut();
		text.insert(text.begin() + cursor_position, key.ascii);
		cursor_position++;

		sig_changed(text);
		sig_activity();
	}
}

int CL_InputBox_Generic::get_mouse_position(int x, int y)
{
/*	int delta_x = x - X_TEXTOFFSET;
	unsigned int i;
	for (i = 0; i < text.size(); i++)
	{
		char buf[2];
		buf[0] = text[i];
		buf[1] = 0;
		int w = fnt_inputbox->get_text_width(buf);
		delta_x -= w;
		if (delta_x <= -w/2)
		{
				return i + last_offset;
		}
	}
	return i;
*/
	return text.size() - 1;
}
