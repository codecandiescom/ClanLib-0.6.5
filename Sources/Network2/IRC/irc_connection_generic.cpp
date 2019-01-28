
#include "irc_connection_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic construction:

CL_IRCConnection_Generic::CL_IRCConnection_Generic(const std::string &server, const std::string &port) :
	sock(CL_Socket::tcp), ref_count(0)
{
	sock.connect(CL_IPAddress(server, port));

	thread = CL_Thread(this);
	thread.start();
}

CL_IRCConnection_Generic::~CL_IRCConnection_Generic()
{
	thread.wait();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic operations:

void CL_IRCConnection_Generic::add_ref()
{
	ref_count++;
}

void CL_IRCConnection_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic implementation:

void CL_IRCConnection_Generic::run()
{
	std::string last_line;

	char buffer[512];
	while (true)
	{
		int received = sock.recv(buffer, 512);
		if (received == 0) break;
		
		int start = 0;
		for (int i=0; i<received; i++)
		{
			if (buffer[i] == '\n')
			{
				last_line.append(buffer+start, i-start+1);
				start = i+1;
				
				CL_MutexSection mutex_lock(&mutex);
				received_queue.push(last_line);
				
				last_line = std::string();
			}
		}
		last_line.append(buffer+start, received-start);
	}
}
	
void CL_IRCConnection_Generic::keep_alive()
{
	CL_MutexSection mutex_lock(&mutex);
	while (!received_queue.empty())
	{
		std::string line = received_queue.front();
		received_queue.pop();

		// Parse line:
		std::string prefix, command;
		std::vector<std::string> args;
		line = line.substr(0, line.length()-2); // cut off CR-LF
		line.append(" "); // whitespace at end. Makes parsing easier.
		
		int pos = 0;
		int endpos;
		if (line[0] == ':') // prefix
		{
			pos = line.find(" ", 1)+1;
			prefix = line.substr(1, pos-1);
		}
		
		endpos = line.find(" ", pos);
		command = line.substr(pos, endpos-pos);
		pos = endpos+1;
		
		while (pos != line.length())
		{
			if (line[pos] == ':')
			{
				args.push_back(line.substr(pos+1, line.length()-pos-2));
				break;
			}
		
			endpos = line.find(" ", pos);
			args.push_back(line.substr(pos, endpos-pos));
			pos = endpos+1;
		}
		
		// Handle command:
		int numeric_value = atoi(command.c_str());
		if (numeric_value != 0)
		{
			sig_numeric_reply(prefix, numeric_value, args);
			
			if (numeric_value == RPL_NAMREPLY)
			{
				const std::string &self = args[0];
				const std::string &channel = args[2];
				const std::string &users = args[3];
				std::vector<std::string> nicks;

				int pos = 0;
				while (pos < users.length())
				{
					int new_pos = users.find(" ", pos);
					if (new_pos == users.npos) new_pos = users.length();
					nicks.push_back(users.substr(pos, new_pos-pos));
					pos = new_pos+1;
				}
				
				sig_name_reply(self, channel, nicks);
			}
			
			continue;
		}
		
		sig_command_received(prefix, command, args);

		int num_args = args.size();
		if (command == "NICK" && num_args == 1) sig_nick(prefix, args[0]);
		else if (command == "JOIN" && num_args == 1) sig_join(prefix, args[0]);
		else if (command == "PART" && num_args == 1) sig_part(prefix, args[0]);
		else if (command == "MODE")
		{
			std::vector<std::string> params;
			for (int i=2; i<num_args; i++) params.push_back(args[i]);
			sig_mode(prefix, args[0], args[1], params);
		}
		else if (command == "TOPIC" && num_args == 2) sig_topic(prefix, args[0], args[1]);
		else if (command == "INVITE" && num_args == 2) sig_invite(prefix, args[0], args[1]);
		else if (command == "KICK" && num_args == 2) sig_kick(prefix, args[0], args[1], "");
		else if (command == "KICK" && num_args == 3) sig_kick(prefix, args[0], args[1], args[2]);
		else if (command == "PRIVMSG" && num_args == 2) sig_privmsg(prefix, args[0], args[1]);
		else if (command == "NOTICE" && num_args == 2) sig_notice(prefix, args[0], args[1]);
		else if (command == "PING" && num_args == 1) sig_ping(args[0], "");
		else if (command == "PING" && num_args == 2) sig_ping(args[0], args[1]);
		else sig_unknown_command_received(prefix, command, args);
	}
}
