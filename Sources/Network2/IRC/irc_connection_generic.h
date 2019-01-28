
#ifndef header_irc_connection_generic
#define header_irc_connection_generic

#include "API/Network2/Socket/socket.h"
#include "API/Network2/IRC/irc_connection.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/mutex.h"

class CL_IRCConnection_Generic : private CL_Runnable, private CL_KeepAlive
{
//! Construction:
public:
	CL_IRCConnection_Generic(const std::string &server, const std::string &port);

	~CL_IRCConnection_Generic();

//! Attributes:
public:
	//: sig_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_command_received;

	//: sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_unknown_command_received;

	//: sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const std::string &, int, const std::vector<std::string> &> sig_numeric_reply;
	
	//: sig_name_reply(self, channel, users)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_name_reply;

	//: sig_nick(old_nick, new_nick)
	CL_Signal_v2<const std::string &, const std::string &> sig_nick;

	//: sig_join(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> sig_join;
	
	//: sig_part(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> sig_part;
	
	//: sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::vector<std::string> &> sig_mode;

	//: sig_topic(prefix, channel, topic)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_topic;
	
	//: sig_invite(prefix, nick, channel)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_invite;

	//: sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> sig_kick;

	//: sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_privmsg;

	//: sig_notice(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_notice;
	
	//: sig_ping(daemon1, daemon2)
	CL_Signal_v2<const std::string &, const std::string &> sig_ping;

	CL_Thread thread;
	
	CL_Mutex mutex;
	
	CL_Socket sock;

	std::queue<std::string> received_queue;

	std::string nick, username, hostname, servername, realname;

//! Operations:
public:
	void add_ref();

	void release_ref();

//! Implementation:
private:
	virtual void run();
	
	virtual void keep_alive();
	
	int ref_count;
};

#endif
