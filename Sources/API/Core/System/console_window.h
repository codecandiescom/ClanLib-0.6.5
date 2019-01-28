/*
	$Id: console_window.h,v 1.11 2001/10/10 11:50:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_console_window
#define header_console_window

#include <string>

class CL_ConsoleWindow_Generic;

//: Text console window.
//- Note: This class has no effect under Linux.
class CL_ConsoleWindow
{
//! Construction:
public:
	//: Console Window Constructor
	CL_ConsoleWindow(
		const std::string &title,
		int width = 80,
		int height = 25);

	//: Console Window Destructor
	~CL_ConsoleWindow();

//! Operations:
public:
	//: Redirects stdout, stdin and stderr to the console window.
	void redirect_stdio();

	//: Redirects stdout and stderr to filename.
	void redirect_stdio(const std::string &filename);

	//: Waits until user hits a key.
	void wait_for_key();

	//: Displays 'press any key to close this console window',
	//: and waits until user hits a key.
	void display_close_message();

//! Implementation:
private:
	CL_ConsoleWindow_Generic *impl;
};

#endif
