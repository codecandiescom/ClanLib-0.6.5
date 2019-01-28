#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <ClanLib/core.h>

#include "netobjects.h"
#include "server_world.h"
#include "fighter.h"
#include "player.h"
#include "players.h"
#include "gameobject.h"
#include "Lobby/lobby_players.h"
#include "Lobby/lobby_player.h"

/////////////////////////////////////////////////////////////////////////////
// ServerWorld construction:

ServerWorld::ServerWorld(CL_NetSession *session, const LobbyPlayers &lobby_players)
:
	World(session),
	max_tick_elapsed(0.05f)
{
	CL_NetGroup all_players;

	// Create player structures for all initial players:
	int player_count = 0;
	LobbyPlayers::const_iterator it;
	for (it = lobby_players.begin(); it != lobby_players.end(); it++)
	{
		LobbyPlayer *const lobby_player = it->second;
		Player *player = new Player(lobby_player);
		player->ID = player_count++;
		players.insert(std::pair<CL_NetComputer, Player *>(player->computer, player));

		all_players.get_computers().push_back(player->computer);

		session->set_access(World::NETCHANNEL_WORLD, player->computer, ACCESS_CHANNEL_READ|ACCESS_CHANNEL_WRITE);
	}

	// Replicate player structures to all computers:
	players.replicate(session, all_players);

	// Create each players fighter object on server:
	Players::iterator player_it;
	for (player_it = players.begin(); player_it != players.end(); player_it++)
	{
		Player *player = player_it->second;
		Fighter *fighter = new Fighter(this, player);
		add_object(fighter);
	}

	// Send initial replication information:
	std::list<GameObject *> &objects = get_objects();
	std::list<GameObject *>::iterator objects_it;
	for (objects_it = objects.begin(); objects_it != objects.end(); objects_it++)
	{
		GameObject *obj = *objects_it;
		obj->send_full(NULL/*all_players*/);
	}
}

ServerWorld::~ServerWorld()
{
}

/////////////////////////////////////////////////////////////////////////////
// ServerWorld attributes:

/////////////////////////////////////////////////////////////////////////////
// ServerWorld operations:

void ServerWorld::dedicated()
{
	float last_time = CL_System::get_time();

	while (true) // todo: find a better break rule :)
	{
		float delta_time = (CL_System::get_time() - last_time);

		for (; delta_time > 0; delta_time -= max_tick_elapsed)
		{
			update(delta_time);
		}

		CL_System::keep_alive();
		CL_System::sleep(10); // todo: find a better sleeping technique.
	}
}

void ServerWorld::update(float delta_time)
{
	World::update(delta_time);
}

/////////////////////////////////////////////////////////////////////////////
// ServerWorld implementation:
