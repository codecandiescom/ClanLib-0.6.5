/*
	$Id: messagebox_generic.cpp,v 1.20 2002/01/16 19:16:51 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "messagebox_generic.h"
#include "API/GUI/component_options.h"
#include "API/Display/Display/display.h"
/*
CL_ComponentOptions CL_MessageBox_Generic::create_options(
	const std::string &title,
	const std::string &text,
	const std::string &button1,
	const std::string &button2,
	const std::string &button3)
{
	CL_ComponentOptions options;

	// TODO: Calculate proper size
	int width = 300;
	int height = 150;
	int x = (CL_Display::get_width() - width) / 2;
	int y = (CL_Display::get_height() - height) / 2;

	options.add_option("x", x);
	options.add_option("y", y);
	options.add_option("width", width);
	options.add_option("height", height);
	options.add_option("title", title);
	options.add_option("text", text);
	options.add_option("button1", button1);
	options.add_option("button2", button2);
	options.add_option("button3", button3);

	return options;
}
*/

CL_MessageBox_Generic::CL_MessageBox_Generic(
	CL_MessageBox *self,
	const std::string &title,
	const std::string &text,
	const std::string &button1,
	const std::string &button2,
	const std::string &button3)
: text(text), messagebox(self)
{
	// TODO: Calculate proper size
	int width = 300;
	int height = 150;
	int x = (CL_Display::get_width() - width) / 2;
	int y = (CL_Display::get_height() - height) / 2;
	messagebox->set_position(CL_Rect(x, y, x+width, y+height));
	messagebox->set_title(title);

	const CL_Rect &pos = messagebox->get_client_area()->get_position();
	width = pos.get_width();
	height = pos.get_height();

	// Add the text
	CL_Point label_pos(0, 10);
	label_text = new CL_Label(label_pos, text, messagebox->get_client_area());

	result_button = -1;

	// Button size
	int button_width = 75;
	int button_height = 25;

	std::string button_text[3];

	// Get button texts
	button_text[0] = button1;
	button_text[1] = button2;
	button_text[2] = button3;

	// Find out total numbers of button
	if(!button_text[2].empty())
		button_count = 3;
	else if(!button_text[1].empty())
		button_count = 2;
	else if(!button_text[0].empty())
		button_count = 1;

	// Center buttons
	int cur_pos_x = (width - ((button_width * button_count) + (10 * (button_count - 1)))) / 2;
	int cur_pos_y = height - button_height - 5;

	// Create and position all the buttons
	for(int i = 0; i < button_count; i++)
	{
		CL_Rect rect(cur_pos_x, cur_pos_y, cur_pos_x + button_width, cur_pos_y + button_height);
		button[i] = new CL_Button(
			rect,
			button_text[i],
			messagebox->get_client_area());

		slot_button[i] = button[i]->sig_clicked().connect(
			this, &CL_MessageBox_Generic::on_button, i);

		cur_pos_x += button_width + 10;
	}

	slot_set_options = messagebox->sig_set_options().connect(
		this, &CL_MessageBox_Generic::on_set_options);
}

CL_MessageBox_Generic::~CL_MessageBox_Generic()
{
	delete label_text;
	for(int i = 0; i < button_count; i++)
		delete button[i];
}

void CL_MessageBox_Generic::on_set_options(const CL_ComponentOptions &options)
{
	// TODO: Fix this (this doesnt work at all)

	std::string button_text[3];

	if(options.exists("text"))
		text = options.get_value("text");
	if(options.exists("button1"))
		button_text[0] = options.get_value("button1");
	if(options.exists("button2"))
		button_text[1] = options.get_value("button2");
	if(options.exists("button3"))
		button_text[2] = options.get_value("button3");
}

void CL_MessageBox_Generic::on_button(int button)
{
	result_button = button;
	sig_button[button]();
	messagebox->quit();
}

