/*
	$Id: window.cpp,v 1.18 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/window.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "window_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction

CL_Window::CL_Window(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, std::string());
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::CL_Window(
	const CL_Rect &pos,
	const std::string &title,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title);
	get_style_manager()->connect_styles("window", this);
}

CL_Window::CL_Window(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, std::string());
	get_style_manager()->connect_styles("window", this);
}

CL_Window::CL_Window(
	const std::string &title,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_Window_Generic(this, title);
	get_style_manager()->connect_styles("window", this);
	find_preferred_size();
}

CL_Window::~CL_Window()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_Window::get_client_area() const
{
	return impl->client_area;
}

const std::string &CL_Window::get_title() const
{
	return impl->title;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Window::set_title(const std::string &text)
{
	impl->title = text;
}

void CL_Window::set_client_size(int width, int height)
{
	impl->set_client_size(width, height);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:
