#ifndef header_server_lobby
#define header_server_lobby

#include "lobby.h"

class CL_NetSession;

// server lobby network code

class ServerLobby: public Lobby
{
// Construction:
public:
	ServerLobby(const CL_NetSession &session);
	~ServerLobby() {};

// Attributes:
public:

// Operations:
public:
	void update();

// Implementation:
public:
	void get_team_count(int &team1, int &team2);

	void update_connects();
	void update_traffic();

	void send_player_ready(LobbyPlayer *player, const CL_NetGroup &who);
	void send_player_jointeam(LobbyPlayer *player, const CL_NetGroup &who);

	void set_ready_status(LobbyPlayer *player, bool ready);
	void set_team(LobbyPlayer *player, int team);

	void add_chatline(LobbyPlayer *player, const std::string &chatline);

	CL_NetSession session;
};

#endif