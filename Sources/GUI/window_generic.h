/*
	$Id: window_generic.h,v 1.8 2001/12/28 22:41:50 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_window_generic
#define header_window_generic

#include "API/GUI/window.h"

class CL_Window_Generic
{
// Construction:
public:
	CL_Window_Generic(CL_Window *self, const std::string &title);

	~CL_Window_Generic();

// Attributes:
public:
	std::string title;

	CL_Component *client_area;

// Operations:
public:
	void set_client_size(int width, int height);

// Signals:
public:

// Callbacks:
public:
	void on_set_options(const CL_ComponentOptions &options);

	void on_resize(int old_width, int old_height);

	CL_Slot slot_set_options;

	CL_Slot slot_resize;

// Implementation:
private:
	CL_Window *window;
};

#endif

