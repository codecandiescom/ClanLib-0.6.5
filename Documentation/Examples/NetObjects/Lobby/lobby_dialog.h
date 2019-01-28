#ifndef header_lobby_dialog
#define header_lobby_dialog

#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/network.h>
#include "client_lobby.h"
#include "server_lobby.h"

// lobby UI, connecting with a Lobby (server or clients)
//
// person behind server has a "start game" button
//

class LobbyDialog : public CL_Frame, public CL_KeepAlive
{
// Construction:
public:
	LobbyDialog(CL_Component *parent, CL_StyleManager *style);
	~LobbyDialog();

// Enums:
public:
	typedef enum
	{
		LOBBY_NONE,
		LOBBY_QUIT,
		LOBBY_GO
	} LobbyResult;

// Attributes:
public:
	LobbyResult get_result();

// Operations:
public:
	void run(CL_Component *parent, ClientLobby *client, ServerLobby *server = NULL);

	void add_chatline(const std::string text);
	void add_team_name(std::string name, int team);

	void set_result(LobbyResult res);
	void set_login(bool login);
	void enable_join_team(bool enable);

// Callbacks:
private:
	void on_paint();
	void on_quit();
	void on_start();
	void on_addline();
	void on_join_team1();
	void on_join_team2();

	virtual void keep_alive();

// Implementation:
private:
	CL_Slot slot_paint;
	CL_Slot slot_quit;
	CL_Slot slot_start;
	CL_Slot slot_addline;
	CL_Slot slot_join_team1;
	CL_Slot slot_join_team2;
	CL_Slot slot_chat_line_enter;

	CL_Button button_join_team1;
	CL_Button button_join_team2;

	CL_Button button_quit_room;
	CL_Button button_start_game;
	
	CL_ListBox list_team1;
	CL_ListBox list_team2;

	CL_ListBox list_chat;
	CL_InputBox	input_chat;
	CL_Button button_add_chat;

	CL_Surface *title;

	ClientLobby *clientlobby;
	ServerLobby *serverlobby;

	LobbyResult result;
	bool ready;
	bool logged_in;
	bool playing_game;
};

#endif