/*
	$Id: window_generic.cpp,v 1.12 2001/12/28 23:45:01 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "window_generic.h"
#include "API/GUI/component_options.h"

/*
CL_ComponentOptions CL_Window_Generic::create_options(
	const CL_Rect &pos,
	const std::string &title)
{
	CL_ComponentOptions options;

	options.add_option("x", pos.x1);
	options.add_option("y", pos.y1);
	options.add_option("width", pos.get_width());
	options.add_option("height", pos.get_height());
	options.add_option("title", title);

	return options;
}
*/

CL_Window_Generic::CL_Window_Generic(
	CL_Window *self,
	const std::string &title)
: title(title), client_area(0), window(self)
{
	client_area = new CL_Component(window);
	CL_Rect rect(3, 24, window->get_width() - 3, window->get_height() - 3);
	client_area->set_position(rect);

	slot_set_options = window->sig_set_options().connect(this, &CL_Window_Generic::on_set_options);
	slot_resize = window->sig_resize().connect(this, &CL_Window_Generic::on_resize);

	on_resize(window->get_width(), window->get_height());
}

CL_Window_Generic::~CL_Window_Generic()
{
	delete client_area;
}

void CL_Window_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("title"))
		title = options.get_value("title");
}

void CL_Window_Generic::set_client_size(int width, int height)
{
	// Rezize window
	window->set_size(
		width + 6,
		height + 27);
}

void CL_Window_Generic::on_resize(int old_width, int old_height)
{
	// dont do this here, is handled by theme -mbn
//	// Resize client area
//	CL_Rect rect(3, 24, window->get_width() - 3, window->get_height() - 3);
//	client_area->set_position(rect);
}
