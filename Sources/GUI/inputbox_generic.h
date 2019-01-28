/*
	$Id: inputbox_generic.h,v 1.20 2002/02/03 18:03:30 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_inputbox_generic
#define header_inputbox_generic

#include "API/GUI/inputbox.h"
#include <string>

class CL_InputBox_Generic
{
public:
	CL_InputBox_Generic(
		CL_InputBox *self,
		const std::string &text,
		bool password_mode,
		bool readonly,
		int maxlength);

	CL_InputBox *inputbox;

	bool password_mode;
	bool read_only;

	bool edited;

	int cursor_position;

	int max_length;

	std::string text;

	CL_Signal_v1<const std::string &> sig_changed;
	CL_Signal_v0 sig_return_pressed;
	CL_Signal_v0 sig_activity;

	CL_Slot slot_set_options;
	CL_Slot slot_input_down;
	CL_Slot slot_input_up;
	CL_Slot slot_mouse_move;

	void on_set_options(const CL_ComponentOptions &options);
	void on_input_down(CL_InputDevice *device, const CL_Key &key);
	void on_input_up(CL_InputDevice *device, const CL_Key &key);
	void on_mouse_move(int x, int y);

	void set_text(const std::string &text);
	const std::string &get_marked_text() const;
	void set_max_length(int length);
	void select_all();
	void deselect();
	void set_selection(int start, int length);
	void set_cursor_position(int pos);
	void backspace();
	void del();
	void cut();
	void move_cursor(int delta, bool mark);
	void move_cursor_word(int delta, bool mark);
	void home(bool mark);
	void end(bool mark);
	int get_selection_start();
	int get_selection_length();

private:
	bool selecting;
	bool mouse_selecting;
	int selection_start;
	int selection_end;

	bool ctrl_down;
	bool shift_down;

	void check_selection();

	void update_text(CL_Key key);
	int get_mouse_position(int x, int y);
};

#endif
