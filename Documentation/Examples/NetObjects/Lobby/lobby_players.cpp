#ifdef WIN32
#pragma warning (disable:4355)
#pragma warning (disable:4786)
#endif

#include "lobby_players.h"
#include "lobby_player.h"

/////////////////////////////////////////////////////////////////////////////
// LobbyPlayers Construction:

LobbyPlayers::LobbyPlayers()
{
}

LobbyPlayers::~LobbyPlayers()
{
	std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
	for (it = begin(); it != end(); it++)
		delete (*it).second;

	std::list<LobbyPlayer *>::iterator itl;
	for (itl = linkdead_players.begin(); itl != linkdead_players.end(); itl++)
		delete (*itl);
}

/////////////////////////////////////////////////////////////////////////////
// LobbyPlayers Operations:

void LobbyPlayers::add(LobbyPlayer *player)
{
	insert(std::pair<CL_NetComputer, LobbyPlayer *>(player->computer, player));
}

void LobbyPlayers::remove(const CL_NetComputer &computer)
{
	std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
	it = find(computer);
	if(it != end())
	{
		erase(it);
		linkdead_players.push_back((*it).second);
	}
}

bool LobbyPlayers::resurrect(const CL_NetComputer &computer)
{
	std::list<LobbyPlayer *>::iterator it;

	for (it = linkdead_players.begin(); it != linkdead_players.end(); it++)
	{
		LobbyPlayer *player = *it;
		if (player->computer == computer)
		{
			linkdead_players.erase(it);
			add(player);
			return true;
		}
	}

	return false;
}

LobbyPlayer *LobbyPlayers::get(const CL_NetComputer &computer)
{
	std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
	it = find(computer);
	if(it != end())
		return (*it).second;

	return NULL;
}

void LobbyPlayers::update()
{
	// todo: check for player netmsg.
}
