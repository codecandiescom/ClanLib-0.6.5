#ifndef header_aboutdialog
#define header_aboutdialog

#include <ClanLib/gui.h>

class AboutDialog : public CL_Window
{
// Construction:
public:
	AboutDialog(CL_StyleManager *style);
	~AboutDialog();

// Attributes:
public:

// Operations:
public:

// Implementation:
private:
	void on_ok();

	CL_Slot slot_ok;

	CL_Label label_title;
	CL_Label label_credits;
	CL_Button button_ok;
};

#endif
