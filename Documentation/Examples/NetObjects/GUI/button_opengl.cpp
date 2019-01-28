#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "button_opengl.h"
#include "stylemanager_opengl.h"

CL_Button_OpenGL::CL_Button_OpenGL(
	CL_Button *button,
	CL_StyleManager_OpenGL *style)
:
	CL_ComponentStyle(button),
	style(style),
	button(button),
	timer(10)
{
	resources = style->get_resources();

	font = CL_Font::load("Button/font", resources);

	if(button->get_width() == 0)
		button->set_width(4 + font->get_text_width(button->get_text().c_str()));
	if(button->get_height() == 0)
		button->set_height(4 + font->get_height());

	if(button->get_height() == 35)
	{
		int width = button->get_width() / 16;
		button->set_width(width * 16);

		surface_up_left = new CL_Surface("Button/up_left", resources);
		surface_up_center = new CL_Surface("Button/up_center", resources);
		surface_up_right = new CL_Surface("Button/up_right", resources);

		surface_down_left = new CL_Surface("Button/down_left", resources);
		surface_down_center = new CL_Surface("Button/down_center", resources);
		surface_down_right = new CL_Surface("Button/down_right", resources);
	}
	else
	{
		surface_up_left = surface_up_center = surface_up_right = NULL;
		surface_down_left = surface_down_center = surface_down_right = NULL;

		slot_mouse_enter = button->sig_mouse_enter().connect(
			this, &CL_Button_OpenGL::on_mouse_enter);
		slot_mouse_leave = button->sig_mouse_leave().connect(
			this, &CL_Button_OpenGL::on_mouse_leave);
		slot_timer = timer.sig_timer().connect(
			this, &CL_Button_OpenGL::on_timer);
	}

	highlighted = false;
	timer.disable();

	slot_paint = button->sig_paint().connect(
		this, &CL_Button_OpenGL::on_paint);
}

CL_Button_OpenGL::~CL_Button_OpenGL()
{
	resources->get_resource("Button/font").unload();
}

void CL_Button_OpenGL::on_mouse_enter()
{
	timer.enable();
	highlighted = true;

	highlightSizeLower = button->get_height() - 8;
	highlightSizeUpper = 0;

	highlightUpperRGB[0] = 128/255.0f;
	highlightUpperRGB[1] = 153/255.0f;
	highlightUpperRGB[2] = 188/255.0f;

	highlightLowerRGB[0] = 72/255.0f;
	highlightLowerRGB[1] = 95/255.0f;
	highlightLowerRGB[2] = 128/255.0f;

	int frames;
	frames = (button->get_height() - 8) / 1;
	highlightUpperDeltaRGB[0] = 128.0f / frames / 255.0f;
	highlightUpperDeltaRGB[1] = 103.0f / frames / 255.0f;
	highlightUpperDeltaRGB[2] = 68.0f / frames / 255.0f;

	frames = (button->get_height() - 8) / 2;
	highlightLowerDeltaRGB[0] = 40.0f / frames / 255.0f;
	highlightLowerDeltaRGB[1] = 45.0f / frames / 255.0f;
	highlightLowerDeltaRGB[2] = 51.0f / frames / 255.0f;
}

void CL_Button_OpenGL::on_mouse_leave()
{
	timer.disable();
	highlighted = false;
}

void CL_Button_OpenGL::on_timer()
{
	if(highlightSizeLower >= 2)
	{
		highlightSizeLower -= 2;

		highlightLowerRGB[0] -= highlightLowerDeltaRGB[0];
		highlightLowerRGB[1] -= highlightLowerDeltaRGB[1];
		highlightLowerRGB[2] -= highlightLowerDeltaRGB[2];
	}

	if(highlightSizeUpper <= button->get_height() - 8 - 1)
	{
		highlightSizeUpper += 1;

		highlightUpperRGB[0] -= highlightUpperDeltaRGB[0];
		highlightUpperRGB[1] -= highlightUpperDeltaRGB[1];
		highlightUpperRGB[2] -= highlightUpperDeltaRGB[2];
	}
}

void CL_Button_OpenGL::on_paint()
{
	if(surface_up_left)
		on_paint_texture();
	else
		on_paint_anim();
}

void CL_Button_OpenGL::on_paint_texture()
{
	if(button->is_down())
	{
		surface_down_left->put_screen(0, 0);

		int x;
		for(x = 16; x <= button->get_width() - 32; x += 16)
			surface_down_center->put_screen(x, 0);

		surface_down_right->put_screen(button->get_width() - 16, 0);
	}
	else
	{
		surface_up_left->put_screen(0, 0);

		int x;
		for(x = 16; x <= button->get_width() - 32; x += 16)
			surface_up_center->put_screen(x, 0);

		surface_up_right->put_screen(button->get_width() - 16, 0);
	}

	int text_width = font->get_text_width(button->get_text().c_str());
	int button_width = button->get_width();
	int text_height = font->get_height();
	int button_height = button->get_height();

	int x_pos = (button_width - text_width) / 2;
	int y_pos = (button_height - text_height) / 2 - 4;

	font->print_left(x_pos, y_pos, button->get_text().c_str());
}

void CL_Button_OpenGL::on_paint_anim()
{
	int text_width = font->get_text_width(button->get_text().c_str());
	int button_width = button->get_width();
	int text_height = font->get_height();
	int button_height = button->get_height();

	int x_pos = (button_width - text_width) / 2;
	int y_pos = (button_height - text_height) / 2;

	// Draw black frame
	CL_Display::draw_rect(0, 0, button_width - 0, button_height - 0, 30/255.0f, 30/255.0f, 30/255.0f);
	CL_Display::draw_rect(1, 1, button_width - 1, button_height - 1, 25/255.0f, 25/255.0f, 50/255.0f);
	CL_Display::draw_rect(2, 2, button_width - 2, button_height - 2, 51/255.0f, 51/255.0f, 51/255.0f);
	CL_Display::fill_rect(3, 3, button_width - 3, button_height - 3, 0.0f, 0.0f, 0.0f);

	if(button->is_down())
	{
/*		style->fill_rect(
			1, 1, button_width - 1, button_height - 1,
			0.3f, 0.5f, 1.0f,
			0.8f, 0.8f, 0.2f, 0.2f);
*/	}
	else
	{
		if(highlighted)
		{
			int height = button_height - 10;
			int ypos;

			// Draw lower
			ypos = (button_height - highlightSizeLower) / 2;
			CL_Display::fill_rect(4, ypos, button_width - 4, ypos + highlightSizeLower, 
				highlightLowerRGB[0], highlightLowerRGB[1], highlightLowerRGB[2]);

			// Draw upper
			ypos = (button_height - highlightSizeUpper) / 2;
			CL_Display::fill_rect(4, ypos, button_width - 4, ypos + highlightSizeUpper, 
				highlightUpperRGB[0], highlightUpperRGB[1], highlightUpperRGB[2]);
		}
	}
	font->print_left(x_pos, y_pos, button->get_text().c_str());
}
