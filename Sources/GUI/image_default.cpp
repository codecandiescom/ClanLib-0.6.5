/*
	$Id: image_default.cpp,v 1.4 2001/12/27 23:30:08 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <stdio.h>
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "image_default.h"

CL_Image_Default::CL_Image_Default(
	CL_Image *_image,
	CL_StyleManager_Default *style) 
: CL_ComponentStyle(_image), image(_image)
{
	resources = style->get_resources();

	slot_paint = image->sig_paint().connect(this, &CL_Image_Default::on_paint);
}

void CL_Image_Default::on_paint()
{
	CL_Surface *surface = image->get_surface();

	if(surface == NULL)
		return;

	int width = image->get_width();
	int height = image->get_height();

	CL_Image::Mode mode = image->get_mode();

	switch (mode)
	{
	case CL_Image::normal:
		surface->put_screen(0, 0);
		break;

	case CL_Image::center:
		surface->put_screen(
			(width - surface->get_width()) / 2,
			(height - surface->get_height()) / 2);
		break;
		
	case CL_Image::stretch:
		surface->put_screen(0, 0, width, height);
		break;

	case CL_Image::tile:
		{
			for (int y = 0; y < height; y += surface->get_height())
				for (int x = 0; x < width; x += surface->get_width())
					surface->put_screen(x, y);
		}
		break;
	}
}
