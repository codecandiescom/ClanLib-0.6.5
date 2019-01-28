/*
	$Id: filedialog_generic.cpp,v 1.35 2002/01/16 18:55:52 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "filedialog_generic.h"
#include "API/GUI/filedialog.h"
#include "API/GUI/listbox.h"
#include "API/GUI/button.h"
#include "API/GUI/label.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/component_options.h"
#include "API/Display/Display/display.h"

#include "API/Core/IOData/directory_scanner.h"

#ifndef WIN32
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <direct.h>
#define _chdir chdir
#endif

#ifdef __BORLANDC__
#include <dir.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction:

/*CL_ComponentOptions CL_FileDialog_Generic::create_options(
	const std::string &title,
	const std::string &file,
	const std::string &filter)
{
	CL_ComponentOptions options;

	// TODO: Calculate proper size
	int width = 400;
	int height = 315;
	int x = (CL_Display::get_width() - width) / 2;
	int y = (CL_Display::get_height() - height) / 2;

	options.add_option("x", x);
	options.add_option("y", y);
	options.add_option("width", width);
	options.add_option("height", height);
	options.add_option("title", title);
	options.add_option("file", file);
	options.add_option("filter", filter);
	options.add_option("show_hidden", false);

	return options;
}
*/
CL_FileDialog_Generic::CL_FileDialog_Generic(
	CL_FileDialog *self,
	const std::string &title,
	const std::string &file,
	const std::string &filter)
: filedialog(self)
{
	// TODO: Calculate proper size
	int width = 400;
	int height = 315;
	int x = (CL_Display::get_width() - width) / 2;
	int y = (CL_Display::get_height() - height) / 2;
	filedialog->set_position(CL_Rect(x, y, x + width, y + height));
	filedialog->set_title(title);

	CL_Component *client_area = filedialog->get_client_area();

	width = client_area->get_width();

	label_dir = new CL_Label(CL_Point(10, 12), "Directory:", client_area);
	input_dir = new CL_InputBox(CL_Rect(65, 10, width - 120, 30), client_area);
	input_dir->enable(false);

	button_parent = new CL_Button(CL_Rect(width - 115, 9, width - 65, 28), "Parent", client_area);
	button_createdir = new CL_Button(CL_Rect(width - 60, 9, width - 10, 28), "New", client_area);

	listbox_files = new CL_ListBox(CL_Rect(10, 32, width - 10, 197), client_area);

	label_file = new CL_Label(CL_Point(10, 207), "Filename:", client_area);
	input_file = new CL_InputBox(CL_Rect(65, 205, width - 10, 225), client_area);

	label_filter = new CL_Label(CL_Point(10, 232), "Filter:", client_area);
	input_filter = new CL_InputBox(CL_Rect(65, 230, width - 10, 250), client_area);

	button_ok = new CL_Button(CL_Rect(10, 260, 100, 280), "OK", client_area);
	button_cancel = new CL_Button(CL_Rect(width - 110, 260, width - 10, 280), "Cancel", client_area);

	slot_set_options = self->sig_set_options().connect(
		this, &CL_FileDialog_Generic::on_set_options);
	slot_file_activated = listbox_files->sig_activated().connect(
		this, &CL_FileDialog_Generic::on_file_activated);
	slot_button_ok = button_ok->sig_clicked().connect(
		this, &CL_FileDialog_Generic::on_button_quit, true);
	slot_button_cancel = button_cancel->sig_clicked().connect(
		this, &CL_FileDialog_Generic::on_button_quit, false);
	slot_button_parent = button_parent->sig_clicked().connect(
		this, &CL_FileDialog_Generic::on_button_parent);
	slot_button_createdir = button_createdir->sig_clicked().connect(
		this, &CL_FileDialog_Generic::on_button_createdir);
	slot_input_file = input_file->sig_return_pressed().connect(
		this, &CL_FileDialog_Generic::on_edit_file);
	slot_input_filter = input_filter->sig_return_pressed().connect(
		this, &CL_FileDialog_Generic::on_edit_filter);
	slot_input_dir = input_dir->sig_return_pressed().connect(
		this, &CL_FileDialog_Generic::on_edit_dir);

	set_file(file, false);
	set_filter(filter, false);
	show_hidden = false;

	read_dir();
}

CL_FileDialog_Generic::~CL_FileDialog_Generic()
{
	delete label_dir;
	delete input_dir;
	delete button_parent;
	delete button_createdir;
	delete listbox_files;
	delete label_file;
	delete input_file;
	delete label_filter;
	delete input_filter;
	delete button_ok;
	delete button_cancel;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_FileDialog_Generic::get_file() const
{
	return file;
}

const std::string &CL_FileDialog_Generic::get_filter() const
{
	return filter;
}

const std::string &CL_FileDialog_Generic::get_dir() const
{
	return dir;
}

const std::string &CL_FileDialog_Generic::get_path() const
{
	throw CL_Error("CL_FileDialog_Generic::get_path() not implemented");
}

bool CL_FileDialog_Generic::is_hidden_files_visible() const
{
	return show_hidden;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_FileDialog_Generic::set_file(const std::string &new_file, bool perform_refresh)
{
	input_file->set_text(new_file);
	file = new_file;

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::set_dir(const std::string &new_dir, bool perform_refresh)
{
	input_dir->set_text(new_dir);
	dir = new_dir;

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::set_filter(const std::string &new_filter, bool perform_refresh)
{
	input_filter->set_text(new_filter);
	filter = new_filter;

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::show_hidden_files(bool enable)
{
	show_hidden = enable;
}

void CL_FileDialog_Generic::refresh()
{
	listbox_files->clear();	
	bool parent = false;

	CL_DirectoryScanner scanner;
	scanner.scan (".");

	while (scanner.next())
	{
		CL_String file(scanner.get_name());

		if(file == "." || file == "..")
			parent = true;
		else 
		{
			if (!scanner.is_hidden() || show_hidden == true)
			{	
				if (scanner.is_directory())
				{
					CL_String tmp = CL_String("[DIR] ") + file;
					file = tmp;
					listbox_files->insert_item(file);
				}
				else
				{
					if(check_filter(file, filter))
						listbox_files->insert_item(file);
				}
			}
		}
	}

	button_parent->enable(parent);

	listbox_files->sort();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_FileDialog_Generic::on_file_activated(int index)
{
	CL_String item = listbox_files->get_current_text();

	if(item.left(5) == "[DIR]")
	{
		CL_String dir = item.right(item.get_length() - 6);
		chdir(dir);
		read_dir();
	}
	else
		set_file(item, false);
}

void CL_FileDialog_Generic::on_button_quit(bool ok)
{
	if(ok == false)
		file = "";

	filedialog->quit();
}

void CL_FileDialog_Generic::on_button_parent()
{
	chdir("..");
	read_dir();
}

void CL_FileDialog_Generic::on_button_createdir()
{
	CL_FileDialog_CreateDir dialog(filedialog);

	if(dialog.file != "")
	{
#ifdef WIN32
		_mkdir(dialog.file.c_str());
#else
		mkdir(dialog.file.c_str(), 755);
#endif
		refresh();
	}
}

void CL_FileDialog_Generic::on_edit_dir()
{
	set_dir(input_dir->get_text());
}

void CL_FileDialog_Generic::on_edit_file()
{
	set_file(input_file->get_text());
}

void CL_FileDialog_Generic::on_edit_filter()
{
	set_filter(input_filter->get_text());
}

void CL_FileDialog_Generic::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("file"))
		set_file(options.get_value("file"), false);

	if(options.exists("filter"))
		set_filter(options.get_value("filter"), false);

	if(options.exists("show_hidden"))
		show_hidden = options.get_value_as_bool("show_hidden");

	read_dir();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_FileDialog_Generic::read_dir()
{
	char cwd_buffer[1024];
	if (getcwd(cwd_buffer, 1024) == NULL)
		throw CL_Error("Working dir is more than 1024 characters!");

	CL_String cwd(cwd_buffer);
	int find_last1 = cwd.find_last('/');
	int find_last2 = cwd.find_last('\\');

	int index = 0;
	if(find_last1 != -1)
		index = find_last1 + 1;
	else if(find_last2 != -1)
		index = find_last2 + 1;

	cwd = cwd.right(cwd.get_length() - index);

	set_dir(cwd);
}

bool CL_FileDialog_Generic::check_filter(CL_String text, CL_String filter, bool case_sensitive)
{
	int text_pos = 0;
	unsigned int filter_pos = 0;

	if(case_sensitive == false)
	{
		text.to_upper();
		filter.to_upper();
	}

	do {
		if(text[text_pos] != filter[(int)filter_pos] &&
		   filter[(int)filter_pos]!='*' &&
		   filter[(int)filter_pos]!='?')
			return false;

		while(filter[(int)filter_pos] == '*' && text[text_pos] == filter[(int)filter_pos + 1])
			filter_pos++;

		if(text[text_pos] == filter[(int)filter_pos] || filter[(int)filter_pos] == '?')
			filter_pos++;

		text_pos++;
	} while(text[text_pos] != 0);

	if(filter_pos >= strlen(filter) - 1)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_CreateDir Construction:

CL_FileDialog_CreateDir::CL_FileDialog_CreateDir(CL_Component *parent)
:
	CL_Window(CL_Rect(10,10,300,105), "Create new directory", NULL, parent->get_style_manager()),
	label(CL_Point(10,10), "New directory:", get_client_area()),
	input(CL_Rect(90, 8, 275, 0), get_client_area()),
	button_ok(CL_Rect(10, 40, 100, 60), "Ok", get_client_area()),
	button_cancel(CL_Rect(185, 40, 275, 60), "Cancel", get_client_area())
{
	slot_button_ok = button_ok.sig_clicked().connect(
		this, &CL_FileDialog_CreateDir::on_quit, true);
	slot_button_cancel = button_cancel.sig_clicked().connect(
		this, &CL_FileDialog_CreateDir::on_quit, false);

	run(parent);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_NewDir Callbacks:

void CL_FileDialog_CreateDir::on_quit(bool ok)
{
	if(ok)
		file = input.get_text();

	quit();
}
