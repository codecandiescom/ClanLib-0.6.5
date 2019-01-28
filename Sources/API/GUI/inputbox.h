/*
	$Id: inputbox.h,v 1.38 2002/02/04 15:21:34 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_inputbox
#define header_inputbox

#include "component.h"

class CL_InputBox_Generic;

//: Inputbox component
class CL_InputBox : public CL_Component
{
//! Construction:
public:
	//: InputBox Constructor
	CL_InputBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Destructor
	virtual ~CL_InputBox();

//! Attributes:
public:
	//: Returns the inputfield text.
	const std::string &get_text() const;

	//: Returns the length of the inputfield text.
	int get_length() const;

	//: Returns the marked text from the inputfield.
	const std::string &get_marked_text() const;

	//: Returns true if part of the text has been marked by the user.
	bool has_marked_text() const;

	//: Returns the position where the selection starts.
	int get_selection_start() const;

	//: Returns the length of current selection.
	int get_selection_length() const;

	//: Returns the maximum permitted length of the text.
	int get_max_length() const;

	//: Returns true if password mode is enabled, false othewise.
	bool in_password_mode() const;

	//: Returns whether the inputfield is read-only. 
	bool is_read_only() const;

	//: Returns the current cursor position.
	int get_cursor_position() const;

	//: Returns true if the contents has been edited.
	bool is_edited() const;

//! Operations:
public:
	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	void set_text(const std::string &text);

	//: Sets the line edit contents to a plain text containing the printed value of num.
	void set_text(int number);

	//: Sets the line edit contents to a plain text containing the printed value of num.
	void set_text(double number);

	//: Set the maximum length of the text in the editor.
	void set_max_length(int length);

	//: If enable is true, display asterisks instead of the characters actually entered.
	void set_password_mode(bool enable = true);

	//: Enables or disables read-only mode.
	void set_read_only(bool enable = true);
	
	//: Selects all text, and moves the cursor to the end.
	void select_all();

	//: Deselects all text (i.e. removes marking) and leaves the cursor at the current position. 
	void deselect();

	//: Sets the marked area at start and be length characters long. 
	void set_selection(int start, int length);

	//: Sets the cursor position to pos. 
	void set_cursor_position(int pos);

	//: Syntactic sugar for set_text("").
	void clear();

	//: Deletes the character to the left of the text cursor and moves the cursor one position to the left.
	//: If a text has been marked, the cursor will be put at the beginning of the marked text
	//: and the marked text will be removed.
	void backspace();

	//: Deletes the character on the right side of the text cursor.
	//: If a text has been marked, the cursor will be put at the beginning of the marked text and the marked
	//: text will be removed.
	void del();

	//: Cut the selected text if any.
	void cut();

	//: Moves the cursor position.
	//- If delta is positive, the cursor is moved forward.
	//- If delta is negative, the cursor is moved backward.
	//- If mark is true, the text is marked.
	void move_cursor(int delta, bool mark = false);

	//: Moves the cursor by word.
	//- If delta is positive, the cursor is moved forward.
	//- If delta is negative, the cursor is moved backward.
	//- If mark is true, the text is marked.
	void move_cursor_word(int delta, bool mark = false);

	//: Moves the text cursor to the left end of the line.
	//- If mark is true, text will be marked towards the first position
	//- If mark is false, any marked text will be unmarked if the cursor is moved. 
	void home(bool mark = false);

	//: Moves the text cursor to the right end of the line.
	//- If mark is true, text will be marked towards the last position
	//- If mark is false, any marked text will be unmarked if the cursor is moved. 
	void end(bool mark = false);

	//: Sets the edited flag to on. 
	//: The edited flag is changed to true whenever the user changes its contents. 
	void set_edited(bool on = true);

//! Signals:
public:
	//: This signal is emitted whenever text is changed in the inputfield.
	//- const std::string & - Contains the new string.
	CL_Signal_v1<const std::string &> &sig_changed();

	//: This signal is emitted when enter is pressed in the inputfield.
	CL_Signal_v0 &sig_return_pressed();

	//: This signal is emitted whenever some character is changed/added/removed, cursor moved etc.
	CL_Signal_v0 &sig_activity();

//! Implementation:
private:
	CL_InputBox(const CL_InputBox &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_InputBox_Generic *impl;
};

#endif
