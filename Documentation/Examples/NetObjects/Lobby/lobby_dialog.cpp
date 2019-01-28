#ifdef WIN32
#pragma warning (disable:4355)
#pragma warning (disable:4786)
#endif

#include <ClanLib/display.h>
#include <ClanLib/png.h>
#include <ClanLib/gl.h>
#include <ClanLib/network.h>

#include "lobby_dialog.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

LobbyDialog::LobbyDialog(CL_Component *parent, CL_StyleManager *style)
:
	CL_Frame(
		CL_Rect(0, 0, CL_Display::get_width(), CL_Display::get_height()),
		parent,
		style),
	button_join_team1(CL_Rect(420, 10, 620, 45), "Join Team 1", this, style),
	button_join_team2(CL_Rect(420, 220, 620, 255), "Join Team 2", this, style),
	button_start_game(CL_Rect(420, 430, 620, 465), "I'm Ready", this, style),
	button_quit_room(CL_Rect(20, 430, 220, 465), "Quit Game", this, style),
	list_team1(CL_Rect(420, 45, 620, 195), this, style),
	list_team2(CL_Rect(420, 255, 620, 395), this, style),
	list_chat(CL_Rect(20, 80, 400, 365), this, style),
	input_chat(CL_Rect(20, 370, 320, 405), "Some Text", this, style),
	button_add_chat(CL_Rect(325, 370, 400, 400), "Add Line", this, style),
	result(LOBBY_NONE),
	clientlobby(NULL),
	serverlobby(NULL),
	ready(false),
	logged_in(false),
	playing_game(false)
{
	slot_paint = sig_paint().connect(this, &LobbyDialog::on_paint);
	slot_quit = button_quit_room.sig_clicked().connect(this, &LobbyDialog::on_quit);
	slot_start = button_start_game.sig_clicked().connect(this, &LobbyDialog::on_start);
	slot_addline = button_add_chat.sig_clicked().connect(this, &LobbyDialog::on_addline);
	slot_join_team1 = button_join_team1.sig_clicked().connect(this,&LobbyDialog::on_join_team1);
	slot_join_team2 = button_join_team2.sig_clicked().connect(this,&LobbyDialog::on_join_team2);
	slot_chat_line_enter = input_chat.sig_return_pressed().connect(this,&LobbyDialog::on_addline);

	title = new CL_Surface("Titles/lobby", style->get_resources());
}

LobbyDialog::~LobbyDialog()
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

LobbyDialog::LobbyResult LobbyDialog::get_result()
{
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void LobbyDialog::run(CL_Component *parent, ClientLobby *client, ServerLobby *server)
{
	clientlobby = client;
	serverlobby = server;

	clientlobby->set_lobby_dialog(this);

	// if we're already logged in that means we came back during gameplay.
	// if we're not logged in yet, then try to login to the server
	if (!logged_in) 
		clientlobby->login();
	else
	{
		// already logged in
		playing_game = true;
		button_start_game.set_text("Back to Game");
	}

	CL_Frame::run(parent);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void LobbyDialog::keep_alive()
{
	if(clientlobby)
		clientlobby->update();
	if(serverlobby)
		serverlobby->update();
}

void LobbyDialog::on_paint()
{
	title->put_screen(20, 20);
}

void LobbyDialog::on_quit()
{
	int res = CL_MessageBox::info(
		"Quit ?",
		"Are you sure you want to quit ?",
		"Yes",
		"No",
		"",
		get_gui_manager()
	);

	if(res == 0)
	{
		result = LOBBY_QUIT;
		quit();
	}
}

void LobbyDialog::on_start()
{
	// if we're already playing a game, the start button is resume game
	if (playing_game)
		set_result(LOBBY_GO);
	else
	{
		// not playing game yet, so let's see if I'm ready
		if (ready)
		{
			ready = false;
			clientlobby->request_start_game(ready);
			enable_join_team(!ready);
			button_start_game.set_text("I'm Ready");
		}
		else
		{
			ready = true;
			enable_join_team(!ready);
			clientlobby->request_start_game(ready);
			button_start_game.set_text("I'm Not Ready");
		}
	}
}

void LobbyDialog::on_addline()
{
	clientlobby->say_line(input_chat.get_text());
	input_chat.clear();
}

void LobbyDialog::add_chatline(const std::string text)
{
	int current = list_chat.insert_item(text) - list_chat.get_max_visible_items() + 1;
	list_chat.set_top_item(list_chat.get_count());
}

void LobbyDialog::on_join_team1()
{
	clientlobby->join_team(1);
}

void LobbyDialog::on_join_team2()
{
	clientlobby->join_team(2);
}

void LobbyDialog::set_result(LobbyResult res)
{
	result = res;
	if (result != LOBBY_NONE)
		quit();
}

void LobbyDialog::set_login(bool login)
{
	logged_in = login;
	enable_join_team(login);
}

void LobbyDialog::add_team_name(std::string name,int team)
{
	if (team==1)
	{
		list_team1.insert_item(name);

		std::vector<CL_ListItem> items = list_team2.get_items();
		std::vector<CL_ListItem>::iterator i;
		
		int found_at = -1;
		int j = 0;
		for (i = items.begin(); i < items.end(); i++, j++)
		{
			if ((*i).str == name)
				found_at = j;
		}
		if (found_at >= 0)
			list_team2.remove_item(found_at);
	}
	else
	{
		list_team2.insert_item(name);

		std::vector<CL_ListItem> items = list_team1.get_items();
		std::vector<CL_ListItem>::iterator i;
		
		int found_at = -1;
		int j = 0;
		for (i = items.begin(); i < items.end(); i++, j++)
		{
			if ((*i).str == name)
				found_at = j;
		}
		if (found_at >= 0)
			list_team1.remove_item(found_at);
	}
}

void LobbyDialog::enable_join_team(bool enable)
{
	button_join_team1.show(enable);
	button_join_team2.show(enable);
}
