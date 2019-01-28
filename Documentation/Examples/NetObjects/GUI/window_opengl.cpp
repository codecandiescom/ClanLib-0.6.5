#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "window_opengl.h"
#include "stylemanager_opengl.h"

CL_Window_OpenGL::CL_Window_OpenGL(
	CL_Window *window,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(window),
	window(window),
	move_handler(CL_Rect(18, 0, window->get_width()-18, 20), window, style),
	style(style),
	resources(style->get_resources())
{
	slot_paint = window->sig_paint().connect(this, &CL_Window_OpenGL::on_paint);

	button_quit = new CL_Button(CL_Rect(3,3,17,17), "", window, style);

	slot_quit = button_quit->sig_clicked().connect(this, &CL_Window_OpenGL::on_quit);
}

CL_Window_OpenGL::~CL_Window_OpenGL()
{
}

void CL_Window_OpenGL::on_paint()
{
	// Window
	style->fill_rect(
		1, 19, window->get_width() - 1, window->get_height() - 1,
		0.0f, 0.0f, 0.0f,
		0.7f, 0.7f, 0.7f, 0.7f);

	// Titlebar
	style->fill_rect(
		0, 0, window->get_width(), 19,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.61f, 0.67f,
		0.0f, 0.61f, 0.67f,
		0.9f);

	// Left line
	CL_Display::draw_line(0, 18, 0, window->get_height() - 1, 0.0f, 0.61f, 0.67f, 1.0f);

	// Right line
	CL_Display::draw_line(window->get_width() - 1, 18, window->get_width() - 1, window->get_height() - 1, 0.0f, 0.61f, 0.67f, 1.0f);

	// Bottom line
	CL_Display::draw_line(0, window->get_height() - 2, window->get_width() - 1, window->get_height() - 2, 0.0f, 0.61f, 0.67f, 1.0f);
}

void CL_Window_OpenGL::on_quit()
{
	window->close();
}
