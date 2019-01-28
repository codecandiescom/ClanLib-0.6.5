#include <ClanLib/core.h>

#include "server_lobby.h"
#include "lobby_player.h"

/////////////////////////////////////////////////////////////////////////////
// ServerLobby construction:

ServerLobby::ServerLobby(const CL_NetSession &_session)
: session(_session)
{
}

/////////////////////////////////////////////////////////////////////////////
// ServerLobby operations:

void ServerLobby::update()
{
	update_connects();
	update_traffic();
}

/////////////////////////////////////////////////////////////////////////////
// ServerLobby implementation:

void ServerLobby::update_connects()
{
	try {
		CL_NetComputer computer;
		
		computer = session.receive_computer_join();
		if(!(computer == NULL))
		{
			// Give access
			session.set_access(CTS_NEW_LOGIN, computer, ACCESS_CHANNEL_WRITE);
			session.set_access(STC_NEW_LOGIN, computer, ACCESS_CHANNEL_READ);
		}

		computer = session.receive_computer_leave();
		if(!(computer == NULL))
			players.remove(computer);
	}
	catch(CL_Error err)
	{
	}
}

void ServerLobby::update_traffic()
{
	// Receiving username/password
	if(session.peek(CTS_NEW_LOGIN))
	{
		CL_NetMessage message = session.receive(CTS_NEW_LOGIN);

		// Player already logged in ?
		if(players.get(message.from))
		{
			// Player already logged in; send back error message
			CL_OutputSource_Memory output;
			output.write_bool8(false);
			output.write_string("Player already logged in");
			session.send(STC_NEW_LOGIN, message.from, output.get_data());
		}
		else
		{
			// Read logon information
			CL_InputSource_Memory input(message.data);
			LobbyPlayer *player = new LobbyPlayer;
			player->computer = message.from;
			player->name = input.read_string();

			// Register player
			players.add(player);

			// Give lobby access
			session.set_access(CTS_CHAT_LINE, message.from, ACCESS_CHANNEL_WRITE);
			session.set_access(STC_CHAT_LINE, message.from, ACCESS_CHANNEL_READ);
			session.set_access(STC_CHAT_GLOBAL, message.from, ACCESS_CHANNEL_READ);
			session.set_access(CTS_PLAYER_READY, message.from, ACCESS_CHANNEL_WRITE);
			session.set_access(STC_PLAYER_READY, message.from, ACCESS_CHANNEL_READ);
			session.set_access(STC_START_GAME, message.from, ACCESS_CHANNEL_READ);

			// Send login was accepted
			CL_OutputSource_Memory output;
			output.write_bool8(true);
			std::string player_name = player->name;
			player_name += " joined the lobby";
			output.write_string(player_name);
			session.send(STC_NEW_LOGIN, message.from, output.get_data());

			// Auto join the least populated team
			int team1 = 0, team2 = 0;
			get_team_count(team1, team2);
			set_team(player, team1 > team2 ? 2 : 1);

			// let's make sure the player joining receives team and ready settings
			std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
			for (it = players.begin(); it != players.end(); it++)
			{
				LobbyPlayer *player = (*it).second;
				
				// don't send info about the player himself, he just joined for christ sake
				if (!(player->computer == message.from))
				{
					if (player->ready_lobby)
						send_player_ready(player, message.from);

					send_player_jointeam(player, message.from);
				}
			}
		}
	}

	if(session.peek(CTS_CHAT_LINE))
	{
		CL_NetMessage message = session.receive(CTS_CHAT_LINE);
		CL_InputSource_Memory input(message.data);

		LobbyPlayer *player = players.get(message.from);
		std::string chat_line = input.read_string();

		add_chatline(player, chat_line);
	}

	if(session.peek(CTS_PLAYER_READY))
	{
		CL_NetMessage message = session.receive(CTS_PLAYER_READY);
		CL_InputSource_Memory input(message.data);

		LobbyPlayer *player = players.get(message.from);
		bool ready = input.read_bool8();

		set_ready_status(player, ready);
	}

	if (session.peek(CTS_JOIN_TEAM))
	{
		CL_NetMessage message = session.receive(CTS_JOIN_TEAM);
		CL_InputSource_Memory input(message.data);

		LobbyPlayer *player = players.get(message.from);
		int new_team = input.read_int32();

		set_team(player, new_team);
	}
}

void ServerLobby::set_ready_status(LobbyPlayer *player, bool ready)
{
	player->ready_lobby = ready;

	send_player_ready(player, session.get_all());

	// check if everyone is ready
	bool everybody_ready = true;
	std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
	for(it = players.begin(); it != players.end(); it++)
	{
		if (!(*it).second->ready_lobby)
		{
			everybody_ready = false;
			break;
		}
	}
	
	if(everybody_ready)
	{
		CL_OutputSource_Memory output;
		output.write_string("Let the games begin!");
		session.send(STC_START_GAME, session.get_all(), output.get_data());			
	}
}

void ServerLobby::set_team(LobbyPlayer *player, int team)
{
	// check if someone is trying to join a team that he's already in!
	if (player->team != team)
	{		
		player->team = team;

		char team_c = '1' + player->team - 1;
		std::string chat_line = player->name + std::string(" joined team #") + team_c;

		CL_OutputSource_Memory output;
		output.write_string(chat_line);
		session.send(STC_CHAT_GLOBAL, session.get_all(), output.get_data());

		send_player_jointeam(player, session.get_all());
	}
}

void ServerLobby::add_chatline(LobbyPlayer *player, const std::string &chatline)
{
	// Create chat line with player name
	std::string message = player->name + ": " + chatline;

	CL_OutputSource_Memory output;
	output.write_string(message);
	session.send(STC_CHAT_LINE, session.get_all(), output.get_data());
}

void ServerLobby::send_player_ready(LobbyPlayer *player, const CL_NetGroup &who)
{
	std::string chat_line;
	if(player->ready_lobby)
		chat_line = player->name + " is ready to play";
	else
		chat_line = player->name + " is not ready to play";

	CL_OutputSource_Memory output;
	output.write_string(chat_line);
	session.send(STC_CHAT_GLOBAL, who, output.get_data());
}

void ServerLobby::send_player_jointeam(LobbyPlayer *player, const CL_NetGroup &who)
{
	// only write team if someone already joined
	if (player->team)
	{
		CL_OutputSource_Memory output2;
		output2.write_int32(player->team);
		output2.write_string(player->name);
		session.send(STC_JOIN_TEAM, who, output2.get_data());	
	}
}

void ServerLobby::get_team_count(int &team1, int &team2)
{
	std::map<CL_NetComputer, LobbyPlayer *>::iterator it;
	for(it = players.begin(); it != players.end(); it++)
	{
		if ((*it).second->team == 1)
			team1++;
		if ((*it).second->team == 2)
			team2++;
	}
}
