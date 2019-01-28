/*
	$Id: messagebox.h,v 1.20 2002/01/29 23:55:56 sphair Exp $

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

#ifndef header_messagebox
#define header_messagebox

#include "window.h"

class CL_MessageBox_Generic;

//: MessageBox component
class CL_MessageBox : public CL_Window
{
//! Construction:
public:
	//: MessageBox constructor
	CL_MessageBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: MessageBox constructor
	CL_MessageBox(
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: MessageBox destructor
	virtual ~CL_MessageBox();

	//: Creates a messagebox "Information" with text and an OK button.
	static void info(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Creates a message box with title, text and an OK button.
	static void info(
		const std::string &title,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Creates a message box with title, text and up to three buttons.
	static int info(
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

//! Attributes:
public:
	//: Returns the message text.
	const std::string &get_text() const;

	//: Returns the index of the button clicked.
	int get_result_button() const;

//! Operations:
public:
	//: Sets the message text.
	void set_text(const std::string &text);

//! Signals:
public:
	//: Signal for button 1
	CL_Signal_v0 &sig_button1();

	//: Signal for button 2
	CL_Signal_v0 &sig_button2();

	//: Signal for button 3
	CL_Signal_v0 &sig_button3();

//! Implementation:
private:
	// TODO:Fix this copy constructor:
//	CL_MessageBox(const CL_MessageBox &copy) : CL_Window(NULL, NULL) { return; } // disallow copy construction.
	CL_MessageBox_Generic *impl;
};

#endif
