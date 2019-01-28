/*
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Core/Resources/resource_manager.h>
#include <ClanLib/Display/Font/font.h>
#include <ClanLib/GUI/button.h>

#include "button_opengl.h"
#include "stylemanager_opengl.h"

CL_Button_OpenGL::CL_Button_OpenGL(
	CL_Button *button,
	CL_StyleManager_OpenGL *style)
: CL_ComponentStyle(button)
{
	this->style = style;
	this->button = button;

	resources = style->get_resources();
	font = CL_Font::load("Button/font", resources);

	if(button->get_width() == 0)
		button->set_width(4 + font->get_text_width(button->get_text().c_str()));
	if(button->get_height() == 0)
		button->set_height(4 + font->get_height());

	highlighted = false;

	slot_paint = button->sig_paint().connect(this, &CL_Button_OpenGL::on_paint);
	slot_mouse_enter = button->sig_mouse_enter().connect(this, &CL_Button_OpenGL::on_mouse_enter);
	slot_mouse_leave = button->sig_mouse_leave().connect(this, &CL_Button_OpenGL::on_mouse_leave);
}

CL_Button_OpenGL::~CL_Button_OpenGL()
{
//	resources->get_resource("Button/font")->unload();
}

void CL_Button_OpenGL::on_mouse_enter()
{
	highlighted = true;
}

void CL_Button_OpenGL::on_mouse_leave()
{
	highlighted = false;
}

void CL_Button_OpenGL::on_paint()
{
	int text_width = font->get_text_width(button->get_text().c_str());
	int button_width = button->get_width();
	int text_height = font->get_height();
	int button_height = button->get_height();

	int x_pos = (button_width - text_width) / 2;
	int y_pos = (button_height - text_height) / 2;

	if(button->is_down())
	{
		CL_Display::draw_rect(1,1, button->get_width() - 1, button->get_height() - 1, 0.0f, 1.0f, 0.0f, 0.7f);
		CL_Display::fill_rect(2,2, button->get_width() - 2, button->get_height() - 2, 0.0f, 1.0f, 0.0f, 0.5f);
		font->print_left(x_pos, y_pos, button->get_text().c_str());
	}
	else
	{
		CL_Display::draw_rect(0,0, button->get_width(), button->get_height(), 0.0f, 1.0f, 0.0f, 0.7f);
		if(highlighted)
			CL_Display::fill_rect(1,1, button->get_width() - 1, button->get_height() - 1, 0.0f, 1.0f, 0.0f, 0.5f);
		else
			CL_Display::fill_rect(1,1, button->get_width() - 1, button->get_height() - 1, 0.0f, 1.0f, 0.0f, 0.4f);
		font->print_left(x_pos, y_pos, button->get_text().c_str());
	}
}
