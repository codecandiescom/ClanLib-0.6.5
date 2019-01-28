/*
	$Id: button.h,v 1.56 2002/02/20 18:03:14 sphair Exp $

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

#ifndef header_button
#define header_button

#include "component.h"

class CL_Surface;
class CL_Button_Generic;

//: Button component
class CL_Button : public CL_Component
{
//! Construction:
public:
	//: Button Constructor
	CL_Button(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Point &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Point &pos,
		const std::string &text,
		CL_Surface *surface_up,
		CL_Surface *surface_down,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Constructor
	CL_Button(
		const CL_Point &pos,
		const std::string &text,
		CL_Surface *surface_up,
		CL_Surface *surface_down,
		CL_Surface *surface_disabled,
		CL_Surface *surface_highlighted,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Button Destructor
	virtual ~CL_Button();

//! Attributes:
public:
	//: Returns the button text.
	const std::string &get_text() const;

	//: Returns true if the button is a toggle button.
	bool is_toggle_button() const;

	//: Returns true if the button pressed down, or false if it is standing up. 
	bool is_down() const;

	//: Returns the surface used when button is in the up-state.
	CL_Surface *get_surface_up() const;

	//: Returns the surface used when button is in the down-state.
	CL_Surface *get_surface_down() const;

	//: Returns the surface used when button is highlighted.
	CL_Surface *get_surface_highlighted() const;

	//: Returns the surface used when button is disabled.
	CL_Surface *get_surface_disabled() const;

//! Operations:
public:
	//: Sets the button text.
	void set_text(const std::string &text);

	//: If toggle is true, pressing the button toggles it between an On and and Off state. 
	//: If toggle is false, pressing the button causes an action, then the button returns to the unpressed state. 
	void set_toggle_mode(bool toggle = true);

	//: Sets the state of the button to pressed down if enable is true or to standing up if enable is false. 
	//: The pressed(), released() and toggled() signals are not emitted by this function. 
	void set_down(bool enable = true);

	//: Toggle the button.
	//: The toggled() signal are emitted by this function.
	void toggle();

	//: Sets the surface used when button is in the up-state.
	void set_surface_up(CL_Surface *surface);

	//: Set the surface used when button is in the down-state.
	void set_surface_down(CL_Surface *surface);

	//: Sets the surface used when button is highlighted.
	void set_surface_highlighted(CL_Surface *surface);

	//: Sets the surface used when button is disabled.
	void set_surface_disabled(CL_Surface *surface);

//! Signals:
public:
	//: This signal is emitted whenever a toggle button changes status.
	//- bool on - Is true if the button is on, or false otherwise.
	CL_Signal_v1<bool> &sig_toggled();

	//: This signal is emitted when the button is activated (i.e. first pressed down and then released when the mouse cursor is inside the button).
	CL_Signal_v0 &sig_clicked();
	
	//: This signal is emitted when the button is pressed down. 
	//: In most cases, you want to use sig_clicked() instead.
	CL_Signal_v0 &sig_pressed();

	//: This signal is emitted when the button is released.
	CL_Signal_v0 &sig_released();

//! Implementation:
private:
	CL_Button(const CL_Button &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_Button_Generic *impl;
};

#endif
