
#ifndef header_player
#define header_player

#include <string>
#include <ClanLib/network.h>
#include "Lobby/lobby_player.h"

class Player
{
// Construction:
public:
	Player(LobbyPlayer const *player);
	// Server-wise constructor.

	Player();
	// Client-wise constructor.

// Attributes:
public:
	CL_NetComputer computer;
	// Computer identifier, only valid on server.

	int ID;
	// Player ID used over network.

	bool owner;
	// True if this world object is the owner of this player.

	std::string name;
	// Name of the player inside game.

	int team;
	// Current team

	int score;
	// Current score.

// Operations:
public:
	void send_full(class CL_OutputSource *output, bool owner);
	// Send replication info.

	void send_partial(class CL_OutputSource *output);
	// Send update info.

	void recv_full(class CL_InputSource *input);
	// Read replication info.

	void recv_partial(class CL_InputSource *input);
	// Read update info.
};

#endif
