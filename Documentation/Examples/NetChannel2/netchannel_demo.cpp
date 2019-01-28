// Netchannel Demo
// Modified Nov 1, 2002 to work with clanNetwork2 (ClanLib 0.7)
//  by Ryan Robinson

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
	CL_ConsoleWindow console("Netchannel demo console");
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
	CL_NetPacket *msg;
	netgame = NULL;

	CL_Timer ping_timer;
	CL_Slot ping_slot;

	try
	{
		//Create a new netsession
		netgame = new CL_NetSession("netchannel demo");
		//connect the connect and disconnect signals to some slots
		slot_connect = netgame->sig_computer_connected().connect(this, 
			&NetChannelDemo::on_server_connect);
		slot_disconnect = netgame->sig_computer_disconnected().connect(this, 
			&NetChannelDemo::on_server_disconnect);
		//start the server listening for client activity
		netgame->start_listen("7667");

		ping_timer = CL_Timer(4000);	//ping all clients every 4 seconds
		ping_slot = ping_timer.sig_timer().connect(this, &NetChannelDemo::send_ping);
		ping_timer.enable();
	}
	catch(CL_Error err)
	{
		cout << "Could not create server: " << err.message.c_str() << endl;
		cout << "We will try to be client instead." << endl;
		return false;
	}

	try
	{
		cout << "*** server running ***" << endl;
		float start_time = CL_System::get_time() / 1000.0f;

		while (true)
		{
			// Send a message to clients each and every while:
			float cur_time = CL_System::get_time() / 1000.0f;
			if (cur_time - start_time > 2) // every two sec
			{
				string msg_string = "Hello, this is your server speaking. ";
			
				//compile a new net packet with various forms of data
				msg = new CL_NetPacket();		
				msg->output.write_string(msg_string);
				msg->output.write_int32(rand());
				msg->output.write_uchar8('x');
				msg->output.write_float32(3.14159f);

				//do the actual sending, to all the computers connected
				netgame->get_all().send("message", *msg);

				delete msg;	// discard sent message		
				
				cout << "Sent message" << endl;

				start_time = cur_time; // reset timer.
			}
		
			// check network events (new computers, etc.):
			// check for joining computers - give full access to channel zero.
			// (both moved to on_server_connect)
			
			// keep ClanLib happy:
			CL_System::keep_alive();
			
			// Save some cpu power:
			CL_System::sleep(100);
		}
	}
	catch (CL_Error err)
	{
		cout << "Fatal server error: " << err.message.c_str() << endl;
	}
	
	delete netgame;

	return true;
}

void NetChannelDemo::send_ping()
{
	string ping_string = "Ping!...";
	static int ping_num = 0;

	//create a ping packet
	CL_NetPacket ping_msg;		
	ping_msg.output.write_string("Ping number ");
	ping_msg.output.write_int32(ping_num++);

	//do the actual sending, to all the computers connected
	netgame->get_all().send("ping", ping_msg);

	cout << "Sent ping" << endl;
}

void NetChannelDemo::run_client()
{
	CL_NetSession *netgame = NULL;

	try
	{
		//create a new netsession
		netgame = new CL_NetSession("netchannel demo");
		//connect the disconnect and receive signals to some slots
		slot_disconnect = netgame->sig_computer_disconnected().connect(this, 
			&NetChannelDemo::on_client_disconnect);
		
		//we have two different slots depending on the packet channel received
		slot_receive_ping = netgame->sig_netpacket_receive("ping").connect(this, 
			&NetChannelDemo::on_ping_receive);
		slot_receive_message = netgame->sig_netpacket_receive("message").connect(this, 
			&NetChannelDemo::on_message_receive);

		//connect to the server (running on the local machine in this case)
		CL_IPAddress server_ip;
		server_ip.set_address("localhost", "7667");
		netgame->connect(server_ip);

	}
	catch(CL_Error err)
	{
		cout << "Could not create client: " << err.message.c_str() << endl;
		return;
	}

	try
	{
		cout << "*** client running ***" << endl;
		
		while (true)
		{
			//network signals now handled by slots now... so do nothing
		
			// keep ClanLib happy:
			CL_System::keep_alive();

			// Save some cpu power:
			CL_System::sleep(100);
		}
		cout << "Game closed" << endl;
	}
	catch (CL_Error err)
	{
		cout << "Fatal client error: " << err.message.c_str() << endl;
	}
	
	delete netgame;
}

void NetChannelDemo::on_server_connect(CL_NetComputer &computer)
{
	cout << "Client joined." << endl;
}

void NetChannelDemo::on_server_disconnect(CL_NetComputer &computer)
{
	cout << "Client disconnected." << endl;
}

void NetChannelDemo::on_client_disconnect(CL_NetComputer &computer)
{
	cout << "Lost connection to server." << endl;
}

void NetChannelDemo::on_message_receive(CL_NetPacket &packet, CL_NetComputer &computer)
{
	string message = packet.input.read_string();
	int mnum = packet.input.read_int32();
	unsigned char mchar = packet.input.read_uchar8();
	float mfloat = packet.input.read_float32();
	cout << "Server message: " << message << endl 
		<< "  " <<  mnum << ", " << mchar 
		<< ", " << mfloat << endl;
}

void NetChannelDemo::on_ping_receive(CL_NetPacket &packet, CL_NetComputer &computer)
{
	string message = packet.input.read_string();
	int pnum = packet.input.read_int32();
	cout << "Server ping: " << message << pnum << endl;
}

