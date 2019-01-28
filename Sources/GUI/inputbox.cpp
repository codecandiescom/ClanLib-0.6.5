/*
	$Id: inputbox.cpp,v 1.45 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "inputbox_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_InputBox::CL_InputBox(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, std::string(), false, false, 1024);
	get_style_manager()->connect_styles("inputbox", this);
	find_preferred_size();
}

CL_InputBox::CL_InputBox(
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, text, false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
	find_preferred_size();
}

CL_InputBox::CL_InputBox(
	const CL_Rect &pos,
	const std::string &text,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, text, false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
}

CL_InputBox::CL_InputBox(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_InputBox_Generic(this, "", false, false, 0);
	get_style_manager()->connect_styles("inputbox", this);
}

CL_InputBox::~CL_InputBox()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_InputBox::get_length() const
{
	return impl->text.size();
}

const std::string &CL_InputBox::get_text() const
{
	return impl->text;
}

const std::string &CL_InputBox::get_marked_text() const
{
	return impl->get_marked_text();
}

bool CL_InputBox::has_marked_text() const
{
	return (get_selection_length() > 0);
}

int CL_InputBox::get_selection_start() const
{
	return impl->get_selection_start();
}

int CL_InputBox::get_selection_length() const
{
	return impl->get_selection_length();
}

int CL_InputBox::get_max_length() const
{
	return impl->max_length;
}

bool CL_InputBox::in_password_mode() const
{
	return impl->password_mode;
}

bool CL_InputBox::is_read_only() const
{
	return impl->read_only;
}

int CL_InputBox::get_cursor_position() const
{
	return impl->cursor_position;
}

bool CL_InputBox::is_edited() const
{
	return impl->edited;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_InputBox::set_text(const std::string &text)
{
	impl->set_text(text);
}

void CL_InputBox::set_text(int number)
{
	char buf[20];
	sprintf(buf, "%d", number);
	impl->set_text(buf);
}

void CL_InputBox::set_text(double number)
{
	char buf[20];
	sprintf(buf, "%f", number);
	impl->set_text(buf);
}

void CL_InputBox::set_max_length(int length)
{
	impl->set_max_length(length);
}

void CL_InputBox::set_password_mode(bool enable)
{
	impl->password_mode = enable;
}

void CL_InputBox::set_read_only(bool enable)
{
	impl->read_only = enable;
}

void CL_InputBox::select_all()
{
	impl->select_all();
}

void CL_InputBox::deselect()
{
	impl->deselect();
}

void CL_InputBox::set_selection(int start, int length)
{
	impl->set_selection(start, length);
}

void CL_InputBox::set_cursor_position(int pos)
{
	impl->set_cursor_position(pos);
}

void CL_InputBox::clear()
{
	impl->set_text("");
}

void CL_InputBox::backspace()
{
	impl->backspace();
}

void CL_InputBox::del()
{
	impl->del();
}

void CL_InputBox::cut()
{
	impl->cut();
}

void CL_InputBox::move_cursor(int delta, bool mark)
{
	impl->move_cursor(delta, mark);
}

void CL_InputBox::move_cursor_word(int delta, bool mark)
{
	impl->move_cursor_word(delta, mark);
}

void CL_InputBox::home(bool mark)
{
	impl->home(mark);
}

void CL_InputBox::end(bool mark)
{
	impl->end(mark);
}

void CL_InputBox::set_edited(bool on)
{
	impl->edited = on;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const std::string &> &CL_InputBox::sig_changed()
{
	return impl->sig_changed;
}

CL_Signal_v0 &CL_InputBox::sig_return_pressed()
{
	return impl->sig_return_pressed;
}

CL_Signal_v0 &CL_InputBox::sig_activity()
{
	return impl->sig_activity;
}
