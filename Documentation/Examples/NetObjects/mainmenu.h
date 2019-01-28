#ifndef header_mainmenu
#define header_mainmenu

#include <ClanLib/gui.h>

#include "options.h"

class MainMenu : public CL_Frame
{
// Construction:
public:
	MainMenu(CL_Component *parent, CL_StyleManager *style);
	~MainMenu();

// Attributes:
public:

// Operations:
public:

// Implementation:
private:
	void on_paint();
	void on_resize(int width, int height);
	void on_create();
	void on_join();
	void on_options();
	void on_about();
	void on_quit();

	CL_Slot slot_paint;
	CL_Slot slot_resize;
	CL_Slot slot_create;
	CL_Slot slot_join;
	CL_Slot slot_options;
	CL_Slot slot_about;
	CL_Slot slot_quit;

	CL_Button button_create;
	CL_Button button_join;
	CL_Button button_options;
	CL_Button button_about;
	CL_Button button_quit;
	CL_InputBox server_name;

	CL_Surface *title;
	CL_Surface *ship;

	Options options;
};

#endif
