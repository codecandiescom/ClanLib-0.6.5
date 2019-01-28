
#ifndef header_lobby_players
#define header_lobby_players

#include <map>
#include <ClanLib/network.h>

class LobbyPlayer;

class LobbyPlayers : public std::map<CL_NetComputer, LobbyPlayer *>
{
// Construction:
public:
	LobbyPlayers();
	~LobbyPlayers();

// Operations:
public:
	void add(LobbyPlayer *player);
	// Registers new player object on server. Replicates it to all clients in the game.

	void remove(const CL_NetComputer &computer);
	// Removes a player with the matching computer identification from the game,
	// and replicates it to all clients in the game.

	bool resurrect(const CL_NetComputer &computer);
	// Look for old player that was previously in the game. If found, re-register the
	// LobbyPlayer object once connected to it.
	// Returns true if successful, false if none was found.

	void update();
	// Parse any incoming messages from the lobby player netchannel.

	LobbyPlayer *get(const CL_NetComputer &computer);
	// Return a player if it exists.

// Implementation:
private:
	std::list<LobbyPlayer *> linkdead_players;
};

#endif
