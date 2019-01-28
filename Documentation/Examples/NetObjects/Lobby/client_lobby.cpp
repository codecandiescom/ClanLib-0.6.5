#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "client_lobby.h"
#include "lobby_dialog.h"
#include <string>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// ClientLobby construction:

ClientLobby::ClientLobby(const CL_NetSession &_session,const Options &_options) 
: session(_session), options(_options), dialog(0)
{

}

ClientLobby::~ClientLobby()
{
}

/////////////////////////////////////////////////////////////////////////////
// ClientLobby Operations:

void ClientLobby::update()
{
	if(session.peek(STC_NEW_LOGIN))
	{
		CL_NetMessage message = session.receive(STC_NEW_LOGIN);
		CL_InputSource_Memory input(message.data);
		
		bool result = input.read_bool8();
		std::string text = input.read_string();
		dialog->add_chatline(text);
		dialog->set_login(result);
	}
	
	if (session.peek(STC_CHAT_LINE))
	{
		CL_NetMessage message = session.receive(STC_CHAT_LINE);
		CL_InputSource_Memory input(message.data);
		
		std::string text = input.read_string();
		dialog->add_chatline(text);
	}

	if (session.peek(STC_CHAT_GLOBAL))
	{
		CL_NetMessage message = session.receive(STC_CHAT_GLOBAL);
		CL_InputSource_Memory input(message.data);
		
		std::string text = input.read_string();
		dialog->add_chatline(text);
	}

	if (session.peek(STC_START_GAME))
	{
		CL_NetMessage message = session.receive(STC_START_GAME);
		CL_InputSource_Memory input(message.data);
		
		std::string text = input.read_string();

		dialog->add_chatline(text);		
		dialog->set_result(LobbyDialog::LOBBY_GO);
	}

	if (session.peek(STC_JOIN_TEAM))
	{
		CL_NetMessage message = session.receive(STC_JOIN_TEAM);
		CL_InputSource_Memory input(message.data);

		int team = input.read_int32();
		std::string name = input.read_string();

		dialog->add_team_name(name, team);
	}
}

void ClientLobby::login()
{
	CL_OutputSource_Memory output;
	output.write_string(options.get_player_name());

	session.send(CTS_NEW_LOGIN, session.get_server(), output.get_data());
}

void ClientLobby::request_start_game(bool go)
{
	// send to server that we want to start game	
	CL_OutputSource_Memory output;
	
	output.write_bool8(go);
	session.send(CTS_PLAYER_READY, session.get_server(), output.get_data());
}

void ClientLobby::join_team(int number)
{
	// send to server that I want to join a specific team
	CL_OutputSource_Memory output;
	output.write_int32(number);
	session.send(CTS_JOIN_TEAM,session.get_server(), output.get_data());
}

void ClientLobby::say_line(const std::string text)
{
	CL_OutputSource_Memory output;
	output.write_string(text);
	
	session.send(CTS_CHAT_LINE, session.get_server(), output.get_data());
}

void ClientLobby::set_lobby_dialog(LobbyDialog *dlg)
{
	dialog = dlg;
}
