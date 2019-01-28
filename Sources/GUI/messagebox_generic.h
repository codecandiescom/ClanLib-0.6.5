/*
	$Id: messagebox_generic.h,v 1.9 2002/01/16 19:16:51 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_dialog_generic
#define header_dialog_generic

#include "API/GUI/messagebox.h"
#include "API/GUI/button.h"
#include "API/GUI/label.h"

class CL_MessageBox_Generic
{
public:
	CL_MessageBox_Generic(
		CL_MessageBox *self,
		const std::string &title,
		const std::string &text,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3);

	CL_MessageBox_Generic::~CL_MessageBox_Generic();

	void set_text(const std::string &text);

	CL_Slot slot_set_options;
	void on_set_options(const CL_ComponentOptions &options);

	std::string text;

	CL_MessageBox *messagebox;

	CL_Label *label_text;
	CL_Button *button[3];
	int button_count;

	CL_Signal_v0 sig_button[3];

	CL_Slot slot_button[3];
	void on_button(int button);

	int result_button;
};

#endif
