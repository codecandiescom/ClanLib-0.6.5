/*
	$Id: gui_manager.h,v 1.32 2002/01/30 13:17:16 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_gui_manager
#define header_gui_manager

#include <stddef.h>

#include "component.h"

class CL_ComponentManager;
class CL_StyleManager;
class CL_GUIManager_Generic;

//: Root component.
// The GUI manager component is the 'root component' of any GUI system. It
// is responsible channeling input and output from the general clanlib display
// API and into the GUI system.
// The GUI manager must to be the top-level component in any component tree.
// Modal dialog interfaces in clanGUI is archived by constructing a GUI manager
// with a parent component. This will disable the input of the previous GUI, but
// will keep calling its painting signals.
class CL_GUIManager : public CL_Component
{
//! Construction:
public:
	//: GUI Manager Constructor
	CL_GUIManager(CL_StyleManager *style);

	//: GUI Manager Constructor
	CL_GUIManager(CL_Component *parent, CL_StyleManager *style);
	
	//: GUI Manager Destructor
	virtual ~CL_GUIManager();

//! Attributes:
public:
	//: Returns the currently focused component.
	CL_Component *get_focus();

	//: Returns the currently captured component.
	CL_Component *get_capture();

//! Operations:
public:
	//: Sets the focus component of the gui.
	void set_focus(CL_Component *component);

	//: Draws the GUI once.
	void show();

	//: Redraws the GUI continuosly.
	void run();

	//: Break run loop.
	void quit();

	//: Tells the GUI to start accepting input.
	void enable_input();
	
	//: Tells the GUI to stop accepting input.
	void disable_input();

	//: Sets the GUI in capture mode. [should we move this out of API space? -- mbn]
	void gui_capture_mouse(CL_Component *component);

	//: Takes GUI out of capture mode. [should we move this out of API space? -- mbn]
	void gui_release_mouse();

//! Implementation:
private:
	CL_GUIManager_Generic *impl;
};

#endif
