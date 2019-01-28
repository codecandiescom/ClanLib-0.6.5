/*
	$Id: filedialog.cpp,v 1.12 2001/12/27 23:14:03 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/filedialog.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "filedialog_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_FileDialog::CL_FileDialog(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Window(parent, style), impl(NULL)
{
	impl = new CL_FileDialog_Generic(this, "", "", "");
}

CL_FileDialog::CL_FileDialog(
	const std::string &title,
	const std::string &file,
	const std::string &filter,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Window(parent, style), impl(NULL)
{
	impl = new CL_FileDialog_Generic(this, title, file, filter);
}

CL_FileDialog::~CL_FileDialog()
{
	delete impl;
}

const std::string CL_FileDialog::open(
	CL_Component *parent)
{
	CL_FileDialog filedialog("Open File", "", "*.*", NULL, parent->get_style_manager());
	filedialog.run(parent);

	return filedialog.get_file();
}

const std::string CL_FileDialog::open(
	const std::string &file,
	const std::string &filter,
	CL_Component *parent)
{
	CL_FileDialog filedialog("Open File", file, filter, NULL, parent->get_style_manager());
	filedialog.run(parent);

	return filedialog.get_file();
}

const std::string CL_FileDialog::save(
	CL_Component *parent)
{
	CL_FileDialog filedialog("Save File", "", "*.*", NULL, parent->get_style_manager());
	filedialog.run(parent);

	return filedialog.get_file();
}

const std::string CL_FileDialog::save(
	const std::string &file,
	const std::string &filter,
	CL_Component *parent)
{
	CL_FileDialog filedialog("Save File", file, filter, NULL, parent->get_style_manager());
	filedialog.run(parent);

	return filedialog.get_file();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_FileDialog::get_file() const
{
	return impl->get_file();
}

const std::string &CL_FileDialog::get_filter() const
{
	return impl->get_filter();
}

const std::string &CL_FileDialog::get_dir() const
{
	return impl->get_dir();
}

const std::string &CL_FileDialog::get_path() const
{
	return impl->get_path();
}

bool CL_FileDialog::is_hidden_files_visible() const
{
	return impl->is_hidden_files_visible();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_FileDialog::set_file(const std::string &filename)
{
	impl->set_file(filename);
}

void CL_FileDialog::set_dir(const std::string &dir)
{
	impl->set_dir(dir);
}

void CL_FileDialog::set_filter(const std::string &filter)
{
	impl->set_filter(filter);
}

void CL_FileDialog::show_hidden_files(bool enable)
{
	impl->show_hidden_files(enable);
}

void CL_FileDialog::refresh()
{
	impl->refresh();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_file_highlighted()
{
	return impl->sig_file_highlighted;
}

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_file_selected()
{
	return impl->sig_file_selected;
}

CL_Signal_v1<const std::string &> &CL_FileDialog::sig_dir_entered()
{
	return impl->sig_dir_entered;
}
