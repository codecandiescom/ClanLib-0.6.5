/*
	$Id: messagebox.cpp,v 1.12 2002/01/29 23:55:56 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <string>
#include "../API/GUI/messagebox.h"
#include "../API/GUI/component_options.h"
#include "../API/GUI/stylemanager.h"
#include "../API/GUI/gui_manager.h"
#include "messagebox_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MessageBox::CL_MessageBox(CL_Component *parent, CL_StyleManager *style)
: CL_Window(parent, style), impl(0)
{
	impl = new CL_MessageBox_Generic(this, "", "", "", "", "");
}

CL_MessageBox::CL_MessageBox(
	const std::string &title,
	const std::string &text,
	const std::string &button1,
	const std::string &button2,
	const std::string &button3,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Window(parent, style), impl(0)
{
	impl = new CL_MessageBox_Generic(this, title, text, button1, button2, button3);
}

CL_MessageBox::~CL_MessageBox()
{
	delete impl;
}

void CL_MessageBox::info(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
{
	if(style == NULL)
		style = parent->get_style_manager();
	CL_MessageBox message("Information", text, "Ok", "", "", NULL, style);
	message.run(parent);
}

void CL_MessageBox::info(
	const std::string &title,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
{
	if(style == NULL)
		style = parent->get_style_manager();
	CL_MessageBox message(title, text, "Ok", "", "", NULL, style);
	message.run(parent);
}

int CL_MessageBox::info(
	const std::string &title,
	const std::string &text,
	const std::string &button1,
	const std::string &button2,
	const std::string &button3,
	CL_Component *parent,
	CL_StyleManager *style)
{
	if(style == NULL)
		style = parent->get_style_manager();
	CL_MessageBox message(title, text, button1, button2, button3, NULL, style);
	message.run(parent);
	return message.get_result_button();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_MessageBox::get_text() const
{
	return impl->text;
}

int CL_MessageBox::get_result_button() const
{
	return impl->result_button;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MessageBox::set_text(const std::string &text)
{
	impl->text = text;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v0 &CL_MessageBox::sig_button1()
{
	return impl->sig_button[0];
}

CL_Signal_v0 &CL_MessageBox::sig_button2()
{
	return impl->sig_button[1];
}

CL_Signal_v0 &CL_MessageBox::sig_button3()
{
	return impl->sig_button[2];
}
