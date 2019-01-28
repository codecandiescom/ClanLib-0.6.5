#ifndef header_client_lobby
#define header_client_lobby

#include "lobby.h"
#include "../options.h"

class LobbyDialog;

// client lobby network code

class ClientLobby: public Lobby
{
// Construction:
public:
	ClientLobby(const CL_NetSession &session, const Options &options);
	~ClientLobby();

// Attributes:
public:

// Operations:
public:
	void set_lobby_dialog(LobbyDialog *dlg);

	void update();

	// login to game
	void login();

	// request start game (when access to the 'start game channel')
	void request_start_game(bool go=true);

	// request to join a team (number)
	void join_team(int number);

	// say some text in the chat
	void say_line(const std::string text);
	
// Implementation:
public:
	CL_NetSession session;
	Options options;
	LobbyDialog *dialog;
};

#endif