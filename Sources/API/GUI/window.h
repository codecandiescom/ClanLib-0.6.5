/*
	$Id: window.h,v 1.25 2002/01/25 18:32:02 sphair Exp $

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

#ifndef header_window
#define header_window

#include "component.h"

class CL_Window_Generic;

//: Window component
class CL_Window : public CL_Component
{
//! Construction:
public:
	//: Creates a window.
	CL_Window(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Creates a window.
	CL_Window(
		const std::string &title,
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: Creates a window located at the given position.
	CL_Window(
		const CL_Rect &pos,
		const std::string &title,
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: Creates a window located at the given position.
	CL_Window(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Window destructor
	virtual ~CL_Window();

//! Attributes:
public:
	//: Returns the client area of the component.
	CL_Component *get_client_area() const;

	//: Returns the window title.
	const std::string &get_title() const;

//! Operations:
public:
	//: Sets the window title.
	//- text - An std::string with the new title
	void set_title(const std::string &text);

	//: Sets the size of the client area, and resizes the window accordingly.
	//- width - The new width of the window
	//- height - The new height of the window
	void set_client_size(int width, int height);

//! Signals:
public:

//! Implementation:
private:
	CL_Window(const CL_Window &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Window_Generic *impl;
};  

#endif
