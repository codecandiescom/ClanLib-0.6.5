#ifndef header_lobby
#define header_lobby

#include "lobby_players.h"
#include "../network.h"

#include <ClanLib/network.h>

// Provide generic interface to talk with UI
//
// chatlines (join, leave, talking)
//
// persons join/leave team (update list on UI needed)
//
// + provide a list that can be used by the rest of the network code
//   to determine teams

class Lobby
{
// Enums:
public:
	enum netchannels
	{
		CTS_NEW_LOGIN = NETCHANNELS_LOBBY,	// Client logs in with player name
		STC_NEW_LOGIN,						// Server returns with login status
		CTS_PLAYER_READY,					// Player sets if he is ready or not
		STC_PLAYER_READY,					// Server tells a player is ready
		CTS_JOIN_TEAM,						// Client requests team join
		STC_JOIN_TEAM,						// Server tells a player joined a team
		CTS_CHAT_LINE,						// Player adds a chatline
		STC_CHAT_LINE,						// Server tells about new chatline
		STC_CHAT_GLOBAL,					// Server tells a global message
		STC_START_GAME,						// Server tells the game should start
	};

// Construction:
public:
	Lobby() { };
	virtual ~Lobby() {};

// Attributes:
public:
	LobbyPlayers players;

// Operations:
public:
	
// Implementation:
public:
};

#endif