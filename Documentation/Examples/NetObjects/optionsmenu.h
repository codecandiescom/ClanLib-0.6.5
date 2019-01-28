#ifndef header_optionsmenu
#define header_optionsmenu

#include <ClanLib/gui.h>

class Options;

class OptionsMenu : public CL_Frame
{
// Construction:
public:
	OptionsMenu(CL_Component *parent, CL_StyleManager *style, Options *options);
	~OptionsMenu();

// Attributes:
public:
	bool get_result();

// Operations:
public:

// Callbacks:
private:
	void on_paint();
	void on_quit(bool save);

// Implementation:
private:
	CL_Slot slot_paint;
	CL_Slot slot_ok;
	CL_Slot slot_cancel;
	CL_Slot slot_name_enter;

	CL_Button button_ok;
	CL_Button button_cancel;

	CL_Label label_name;
	CL_InputBox input_name;

	CL_CheckBox checkbox_intro;

	CL_Label label_message;

	Options *options;

	CL_Surface *title;

	bool result;
};

#endif
