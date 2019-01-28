// Run this example twice to create both a server and a client

#include "netchannel_demo.h"

NetChannelDemo app;

char *NetChannelDemo::get_title()
{
	return "NetChannel demo.";
}

int NetChannelDemo::main(int argc, char **argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("My console");
	console.redirect_stdio();

	CL_SetupCore::init();
	CL_SetupNetwork::init();

	// try server - if already one, go as client.
	if (run_server() == false) run_client();

	CL_SetupNetwork::deinit();
	CL_SetupCore::deinit();

	// Display console close message and wait for a key
	console.display_close_message();

	return 1;
}
	
bool NetChannelDemo::run_server()
{
	CL_NetSession *netgame = NULL;

	try
	{
		netgame = new CL_NetSession("netchannel demo", 7667);
	}
	catch(CL_Error err)
	{
		std::cout << "Could not create server: " << err.message.c_str() << std::endl;
		std::cout << "We will try to be client instead." << std::endl;
		return false;
	}

	try
	{
		std::cout << "*** server running ***" << std::endl;
		float start_time = CL_System::get_time() / 1000.0f;
		while (true)
		{
			// Send Hello ping to clients each and every while:
			float cur_time = CL_System::get_time() / 1000.0f;
			if (cur_time - start_time > 2) // every two sec
			{
				static char *string = "Ping!...";
			
				CL_NetMessage msg;
				msg.data = string;

				netgame->send(
					0,
					netgame->get_all(),
					msg);

				start_time = cur_time; // reset timer.
			}
		
			// check network events (new computers, etc.):

			// check for joining computers - give full access to channel zero.
			try
			{
				CL_NetComputer computer = netgame->receive_computer_join();

				std::cout << "Computer joined." << std::endl;
				netgame->set_access(0, computer);
			}
			catch (CL_Error err)
			{
			}
			
			// keep ClanLib happy:
			CL_System::keep_alive();
			
			// Save some cpu power:
			CL_System::sleep(100);
		}
	}
	catch (CL_Error err)
	{
		std::cout << "Fatal server error: " << err.message.c_str() << std::endl;
	}
	
	delete netgame;

	return true;
}

void NetChannelDemo::run_client()
{
	CL_NetSession *netgame = NULL;

	try
	{
		netgame = new CL_NetSession("netchannel demo", "localhost", 7667);
//		CL_Network::find_game_at("netchannel demo", "localhost", 7667);
//		CL_Network::find_games_broadcast("netchannel demo", 7667);
//		netgame = CL_Network::receive_game_found(5000); // 5 sec timeout.

		std::cout << "*** client running ***" << std::endl;
		
		while (!netgame->receive_session_closed())
		{
			try
			{
				CL_NetMessage msg = netgame->receive(0);
				std::cout << "Server said: " << msg.data.c_str() << std::endl;
			}
			catch (...)
			{
			}
		
			// keep ClanLib happy:
			CL_System::keep_alive();

			// Save some cpu power:
			CL_System::sleep(100);
		}
		std::cout << "Game closed" << std::endl;
	}
	catch (CL_Error err)
	{
		std::cout << "Fatal client error: " << err.message.c_str() << std::endl;
	}
	
	delete netgame;
}
