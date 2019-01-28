/*
	$Id: progressbar_default.cpp,v 1.8 2001/12/28 00:04:31 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "progressbar_default.h"
#include "button_default.h"
#include "API/Display/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"

CL_ProgressBar_Default::CL_ProgressBar_Default(
	CL_ProgressBar *_progressbar,
	CL_StyleManager_Default *style)
:
	CL_ComponentStyle(_progressbar),
	progressbar(_progressbar)
{
	this->style = style;

	slot_paint = progressbar->sig_paint().connect(
		this, &CL_ProgressBar_Default::on_paint);
}

void CL_ProgressBar_Default::on_paint()
{
	int width = progressbar->get_width();
	int height = progressbar->get_height();
	float percent = progressbar->get_percentage();

	// Outline
	style->draw_rect(0, 0, width, height, GUICOLOR_DARK_OUTLINE);

	// Shade box
	style->draw_box(1, 1, width - 1 , height - 1, GUICOLOR_DARK_SHADE, GUICOLOR_BRIGHT_SHADE);

	// Fill
	int length = (int) ((width - 4) * percent);

	style->fill_rect(2, 2, 2 + length, height - 2, GUICOLOR_PROGRESSBAR_FILLED);
	style->fill_rect(2 + length, 2, width - 2, height - 2, GUICOLOR_PROGRESSBAR);

	// TODO: Write percentage in text
//	font->print_left(...);
}
