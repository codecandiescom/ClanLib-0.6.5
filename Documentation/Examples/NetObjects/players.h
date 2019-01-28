#ifndef header_players
#define header_players

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "network.h"

#include <map>
#include <ClanLib/network.h>

class Player;

class Players : public std::multimap<CL_NetComputer, Player *>
{
// Enums:
public:
	enum netchannels
	{
		NETCHANNEL_PLAYERS = NETCHANNELS_PLAYERS
	};

	enum netmessage_type
	{
		PLAYER_CREATE, 
		PLAYER_UPDATE, 
		PLAYER_DESTROY
	};

// Construction:
public:
	Players();
	~Players();

// Operations:
public:
	void add(Player *player);
	// Registers new player object on server. Replicates it to all clients in the game.

	void remove(const CL_NetComputer &computer);
	// Removes a player with the matching computer identification from the game,
	// and replicates it to all clients in the game.

	bool resurrect(const CL_NetComputer &computer);
	// Look for old player that was previously in the game. If found, re-register the
	// Player object once connected to it.
	// Returns true if successful, false if none was found.

	void update(CL_NetSession *session);
	// Parse any incoming messages from the player netchannel.

	void replicate(CL_NetSession *session, const CL_NetGroup &group);
	// Replicate all players to the passed group of computers.

	Player *get_id(int playerID);
	// Search for player matching player ID.

	Player *get(const CL_NetComputer &computer);
	// Return a player if it exists.

// Implementation:
private:
	std::list<Player *> linkdead_players;
};

#endif
