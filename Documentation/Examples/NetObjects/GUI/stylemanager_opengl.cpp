#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include "stylemanager_opengl.h"
#include "button_opengl.h"
#include "checkbox_opengl.h"
#include "frame_opengl.h"
#include "inputbox_opengl.h"
#include "window_opengl.h"
#include "listbox_opengl.h"
#include "scrollbar_opengl.h"

// Construction:

CL_StyleManager_OpenGL::CL_StyleManager_OpenGL(CL_ResourceManager *resources)
: CL_StyleManager_Default(resources)
{
}

CL_StyleManager_OpenGL::~CL_StyleManager_OpenGL()
{
}

// Attributes:

// Operations:

void CL_StyleManager_OpenGL::fill_rect(
	int x1, int y1, int x2, int y2,
	float r, float g, float b,
	float a0, float a1, float a2, float a3)
{
	CL_OpenGL::begin_2d();

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glColor4f(r,g,b,a0);
		glVertex2i(x1, y1);
		glColor4f(r,g,b,a1);
		glVertex2i(x2, y1);
		glColor4f(r,g,b,a2);
		glVertex2i(x2, y2);
		glColor4f(r,g,b,a3);
		glVertex2i(x1, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);

	CL_OpenGL::end_2d();
}

void CL_StyleManager_OpenGL::fill_rect(
	int x1, int y1, int x2, int y2,
	float r0, float g0, float b0, 
	float r1, float g1, float b1, 
	float r2, float g2, float b2, 
	float r3, float g3, float b3,
	float a)
{
	CL_OpenGL::begin_2d();

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glColor4f(r0,g0,b0,a);
		glVertex2i(x1, y1);
		glColor4f(r1,g1,b1,a);
		glVertex2i(x2, y1);
		glColor4f(r2,g2,b2,a);
		glVertex2i(x2, y2);
		glColor4f(r3,g3,b3,a);
		glVertex2i(x1, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);

	CL_OpenGL::end_2d();
}

// Overridables:

void CL_StyleManager_OpenGL::connect_styles(
	const std::string &type,
	CL_Component *owner)
{
	if (type == "button")
	{
		CL_Button *button = (CL_Button *) owner;
		button->attach_style(new CL_Button_OpenGL(button, this));
	}
	else if (type == "window")
	{
		CL_Window *window = (CL_Window *) owner;
		window->attach_style(new CL_Window_OpenGL(window, this));
	}
	else if (type == "frame")
	{
		CL_Frame *frame = (CL_Frame *) owner;
		frame->attach_style(new CL_Frame_OpenGL(frame, this));
	}
	else if (type == "inputbox")
	{
		CL_InputBox *inputbox = (CL_InputBox *) owner;
		inputbox->attach_style(new CL_InputBox_OpenGL(inputbox, this));
	}
	else if (type == "listbox")
	{
		CL_ListBox *listbox = (CL_ListBox *) owner;
		listbox->attach_style(new CL_ListBox_OpenGL(listbox, this));
	}
	else if (type == "scrollbar")
	{
		CL_ScrollBar *scrollbar = (CL_ScrollBar *) owner;
		scrollbar->attach_style(new CL_ScrollBar_OpenGL(scrollbar, this));
	}
	else if (type == "checkbox")
	{
		CL_CheckBox *checkbox = (CL_CheckBox *) owner;
		checkbox->attach_style(new CL_CheckBox_OpenGL(checkbox, this));
	}
	else
	{
		CL_StyleManager_Default::connect_styles(type, owner);
	}
}
