
#ifndef header_lobby_player
#define header_lobby_player

#include <string>
#include <ClanLib/network.h>

class LobbyPlayer
{
// Construction:
public:

	LobbyPlayer() : team(0), ready_lobby(false) {}
	
// Attributes:
public:
	std::string name;
	// Name of the player inside game.

	int team;
	// Current team

	CL_NetComputer computer;
	// Computer identifier, only valid on server.

	bool ready_lobby;
	// True is player is ready to start game (from lobby)
};

#endif
