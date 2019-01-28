/*
	$Id: filedialog_generic.h,v 1.13 2001/12/27 23:14:03 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_file_dialog_generic
#define header_file_dialog_generic

#include <string>

#include "API/Core/System/clanstring.h"
#include "API/signals.h"
#include "API/GUI/window.h"
#include "API/GUI/label.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/button.h"

class CL_ComponentOptions;
class CL_FileDialog;
class CL_StyleManager;
class CL_ListBox;

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_NewDir

class CL_FileDialog_CreateDir : public CL_Window
{
// Construction
public:
	CL_FileDialog_CreateDir(CL_Component *parent);

	std::string file;

// Implementation
private:
	void on_quit(bool ok);

	CL_Label label;
	CL_InputBox input;
	CL_Button button_ok;
	CL_Button button_cancel;

	CL_Slot slot_button_ok;
	CL_Slot slot_button_cancel;
};

/////////////////////////////////////////////////////////////////////////////
// CL_FileDialog_Generic

class CL_FileDialog_Generic
{
// Construction
public:
	CL_FileDialog_Generic(
		CL_FileDialog *self,
		const std::string &title,
		const std::string &file,
		const std::string &filter);
	~CL_FileDialog_Generic();

// Attributes
public:
	const std::string &get_file() const;
	const std::string &get_filter() const;
	const std::string &get_dir() const;
	const std::string &get_path() const;
	bool is_hidden_files_visible() const;

// Operations
public:
	void set_file(const std::string &filename, bool refresh = true);
	void set_dir(const std::string &dir, bool refresh = true);
	void set_filter(const std::string &filter, bool refresh = true);
	void show_hidden_files(bool enable);
	void refresh();

// Slots:
private:
	CL_Slot slot_file_activated;
	CL_Slot slot_button_ok;
	CL_Slot slot_button_cancel;
	CL_Slot slot_button_parent;
	CL_Slot slot_button_createdir;
	CL_Slot slot_input_dir;
	CL_Slot slot_input_file;
	CL_Slot slot_input_filter;
	CL_Slot slot_set_options;

// Callbacks:
public:
	void on_file_activated(int index);
	void on_button_quit(bool ok);
	void on_button_parent();
	void on_button_createdir();
	void on_edit_dir();
	void on_edit_file();
	void on_edit_filter();
	void on_set_options(const CL_ComponentOptions &options);

// Signals:
public:
	CL_Signal_v1<const std::string &> sig_file_highlighted;
	CL_Signal_v1<const std::string &> sig_file_selected;
	CL_Signal_v1<const std::string &> sig_dir_entered;

// Implementation:
private:
	bool check_filter(CL_String text, CL_String filter, bool case_sensitive = false);
	void read_dir();

	CL_FileDialog *filedialog;

	bool show_hidden;

	CL_Label *label_dir;
	CL_InputBox *input_dir;
	CL_Button *button_parent;
	CL_Button *button_createdir;

	CL_ListBox *listbox_files;

	CL_Label *label_file;
	CL_InputBox *input_file;

	CL_Label *label_filter;
	CL_InputBox *input_filter;

	CL_Button *button_ok;
	CL_Button *button_cancel;

	std::string file;
	std::string filter;
	std::string dir;
	std::string path;
};

#endif
