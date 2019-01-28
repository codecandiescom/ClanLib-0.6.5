/*
	$Id: frame_default.cpp,v 1.35 2002/01/16 18:58:13 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <cstdio>
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "frame_default.h"

CL_Frame_Default::CL_Frame_Default(
	CL_Frame *_frame,
	CL_StyleManager_Default *style) 
: CL_ComponentStyle(_frame), frame(_frame)
{
	this->style = style;

	resources = style->get_resources();
	sur_surface = NULL;
	mode = tile;

	slot_set_options = frame->sig_set_options().connect(
		this, &CL_Frame_Default::on_set_options);
	slot_paint = frame->sig_paint().connect(
		this, &CL_Frame_Default::on_paint);
}

CL_Frame_Default::~CL_Frame_Default()
{
	if(sur_surface)
		delete sur_surface;
}

void CL_Frame_Default::on_set_options(const CL_ComponentOptions &options)
{
	if(options.exists("surface"))
	{
		std::string surface = options.get_value("surface");
		sur_surface = CL_Surface::load(surface.c_str(), resources);
	}
	
	if (options.exists("mode"))
	{
		std::string v = options.get_value("mode");
		
		if (v == "center") mode = center;
		else if (v == "stretch") mode = stretch;
		else if (v == "tile") mode = tile;
		else throw CL_Error("unknown frame mode");
	}
}

void CL_Frame_Default::on_paint()
{
	int width = frame->get_width();
	int height = frame->get_height();

	if (sur_surface)
	{
		switch (mode)
		{
		case center:
			sur_surface->put_screen(
				width / 2 - sur_surface->get_width() / 2,
				height / 2 - sur_surface->get_height() / 2);
			break;
			
		case stretch:
			sur_surface->put_screen(0, 0, width, height);
			break;

		case tile:
			{
			for (int y = 0; y < height; y += sur_surface->get_height())
				for (int x = 0; x < width; x += sur_surface->get_width())
					sur_surface->put_screen(x, y);
			}
		}
	}
	else
	{
		style->draw_box(0, 0, width, height, GUICOLOR_DARK_SHADE, GUICOLOR_BRIGHT_SHADE);
		if(frame->is_fill_enabled())
			style->fill_rect(1, 1, width - 1, height -1, GUICOLOR_MEDIUM_SHADE);
	}
}
