#include "gui_project_window.h"

GUI_Project_Window::GUI_Project_Window(CL_Component *parent, CL_StyleManager *style)
:
	CL_Window(
		CL_Rect(10, 200,  420, 400),
		"GUI Project Window",
		parent,
		style)
{
}

GUI_Project_Window::~GUI_Project_Window()
{

}
