#ifdef WIN32
#pragma warning (disable:4355)
#endif

#include <ClanLib/display.h>

#include "optionsmenu.h"
#include "options.h"

/////////////////////////////////////////////////////////////////////////////
// OptionsMenu construction:

OptionsMenu::OptionsMenu(CL_Component *parent, CL_StyleManager *style, Options *_options)
:
	CL_Frame(
		CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()),
		parent,
		style),
	button_ok(CL_Rect(10, 440, 190, 475), "Ok", this, style),
	button_cancel(CL_Rect(10, 400, 190, 435), "Cancel", this, style),
	label_name(CL_Rect(10, 58 + 75, 100, 93 + 75), "Player name:", this, style),
	input_name(CL_Rect(120, 50 + 75, 630, 85 + 75), "Player", this, style),
	checkbox_intro(CL_Point(20, 200), "Show intro on startup", this, style),
	label_message(CL_Rect(125, 100, 630, 125), "You need to set a player name before playing!", this, style),
	options(_options)
{
	slot_paint = sig_paint().connect(this, &OptionsMenu::on_paint);
	slot_ok = button_ok.sig_clicked().connect(this, &OptionsMenu::on_quit, true);
	slot_cancel = button_cancel.sig_clicked().connect(this, &OptionsMenu::on_quit, false);
	slot_name_enter=input_name.sig_return_pressed().connect(this, &OptionsMenu::on_quit,true);

	input_name.set_text(options->get_player_name());
//	input_name.set_focus();

	if(options->get_player_name() != "")
		label_message.show(false);

	title = new CL_Surface("Titles/options", style->get_resources());
}

OptionsMenu::~OptionsMenu()
{
}

/////////////////////////////////////////////////////////////////////////////
// OptionsMenu attributes:

bool OptionsMenu::get_result()
{
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// OptionsMenu operations:

/////////////////////////////////////////////////////////////////////////////
// OptionsMenu callbacks:

void OptionsMenu::on_paint()
{
	title->put_screen(20, 20);
}

void OptionsMenu::on_quit(bool save)
{
	if(save)
	{
		if(input_name.get_length())
			options->set_player_name(input_name.get_text());
		else
		{
			CL_MessageBox::info("You need to fill in a player name", get_gui_manager());
			return;
		}
	}

	result = save;

	quit();
}

/////////////////////////////////////////////////////////////////////////////
// OptionsMenu implementation:
