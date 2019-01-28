#include <ClanLib/display.h>
#include "about_dialog.h"

/////////////////////////////////////////////////////////////////////////////
// AboutDialog construction:

static char *title_string = "Brutal Entity Combat 2 - DodgeMe";

static char *credit_string =
	"Credits goes to:\n"
	"   Magnus Norddahl\n"
	"   Kenneth Gangstø\n"
	"   Jeroen Janssen\n"
	" \n"
	"Happy ClanLibbing!";

AboutDialog::AboutDialog(CL_StyleManager *style)
:
	CL_Window(
		CL_Rect::center(CL_Display::get_width()/2, CL_Display::get_height()/2, 400, 300),
//		CL_Rect(0,0,640,480),
		"About",
		NULL,
		style),

	label_title(
		CL_Rect(0, 10, get_width(), 0),
		title_string,
		get_client_area()),

	label_credits(
		CL_Point(10, 70),
		credit_string,
		get_client_area()),

	button_ok(
		CL_Rect::center(get_client_area()->get_width()/2, get_client_area()->get_height()-25, 95, 30),
		"Ok",
		get_client_area())
{
	label_title.set_alignment(true);

	slot_ok = button_ok.sig_clicked().connect(this, &AboutDialog::on_ok);
}

AboutDialog::~AboutDialog()
{
}

/////////////////////////////////////////////////////////////////////////////
// AboutDialog attributes:

/////////////////////////////////////////////////////////////////////////////
// AboutDialog operations:

/////////////////////////////////////////////////////////////////////////////
// AboutDialog implementation:

void AboutDialog::on_ok()
{
	quit();
}
