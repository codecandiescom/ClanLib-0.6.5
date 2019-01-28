#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>

#include "players.h"
#include "player.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// Players Construction:

Players::Players()
{
}

Players::~Players()
{
	iterator it;
	for (it = begin(); it != end(); it++)
		delete (*it).second;

	std::list<Player *>::iterator itl;
	for (itl = linkdead_players.begin(); itl != linkdead_players.end(); itl++)
		delete (*itl);
}

/////////////////////////////////////////////////////////////////////////////
// Players Operations:

void Players::add(Player *player)
{
	insert(std::pair<CL_NetComputer, Player *>(player->computer, player));
}

void Players::remove(const CL_NetComputer &computer)
{
	iterator it;
	it = find(computer);
	if(it != end())
	{
		erase(it);
		linkdead_players.push_back((*it).second);
	}
}

bool Players::resurrect(const CL_NetComputer &computer)
{
	std::list<Player *>::iterator it;

	for (it = linkdead_players.begin(); it != linkdead_players.end(); it++)
	{
		Player *player = *it;
		if (player->computer == computer)
		{
			linkdead_players.erase(it);
			add(player);
			return true;
		}
	}

	return false;
}

Player *Players::get(const CL_NetComputer &computer)
{
	iterator it;
	it = find(computer);
	if(it != end())
		return (*it).second;

	return NULL;
}

Player *Players::get_id(int playerID)
{
	std::cout << "Looking for ID: " << playerID << std::endl;
	for (iterator it = begin(); it != end(); it++)
	{
		Player *player = it->second;
		std::cout << "Found ID: " << player->ID << std::endl;
		if (player->ID == playerID) return player;
	}
	return NULL;
}

void Players::update(CL_NetSession *session)
{
	while(session->peek(NETCHANNEL_PLAYERS))
	{
		CL_NetMessage message = session->receive(NETCHANNEL_PLAYERS);
		CL_InputSource_Memory input(message.data);
		
		int type = input.read_int32();
		if(type == PLAYER_CREATE)
		{
			Player *player = new Player;
			player->recv_full(&input);	
			add(player);

			std::cout << "Created new player: " << player->name << std::endl;
		}
		else if(type == PLAYER_UPDATE)
		{
//			player->recv_partial(&input);	
		}
	}
}

void Players::replicate(CL_NetSession *session, const CL_NetGroup &dest)
{
	for (iterator it = begin(); it != end(); it++)
	{
		Player *player = it->second;

		// Send player replication to every player
		for (iterator it2 = begin(); it2 != end(); it2++)
		{
			Player *player2 = it2->second;

			bool owner = (player->computer == player2->computer);
			CL_OutputSource_Memory output;
			output.write_int32(PLAYER_CREATE);
			player->send_full(&output, owner);

			session->send(NETCHANNEL_PLAYERS, player2->computer, CL_NetMessage(output.get_data()));
		}
	}
}
