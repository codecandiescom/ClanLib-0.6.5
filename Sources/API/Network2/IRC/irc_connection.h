
//! clanNetwork="Internet Relay Chat"
//! header=network.h

#ifndef header_irc_connection
#define header_irc_connection

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <string>
#include <vector>
#include <list>
#include <queue>

#include "../../signals.h"
#include "irc_numerics.h"

class CL_IRCConnection_Generic;

class CL_IRCConnection
{
//! Construction:
public:
	CL_IRCConnection();

	CL_IRCConnection(const std::string &server, const std::string &port = "6667");
	
	CL_IRCConnection(const CL_IRCConnection &copy);

	~CL_IRCConnection();
	
//! Attributes:
public:
	const std::string &get_nick() const;

	const std::string &get_username() const;

	const std::string &get_hostname() const;

	const std::string &get_servername() const;

	const std::string &get_realname() const;

//! Operations:
public:
	CL_IRCConnection &operator =(const CL_IRCConnection &copy);

	void send_command(const std::string &prefix, const std::string &command, const std::vector<std::string> &params);
	
	void send_pass(const std::string &password);
	
	void send_nick(const std::string &nick, int hopcount = -1);
	
	void send_user(const std::string &username, const std::string &hostname, const std::string &servername, const std::string &realname);

	void send_oper(const std::string &user, const std::string &password);
	
	void send_quit(const std::string &quitmessage = "");
	
	void send_join(const std::string &channel, const std::string &key = "");
	
	void send_join(const std::list<std::string> &channels, const std::list<std::string> &keys);
	
	void send_part(const std::string &channel);

	void send_part(const std::list<std::string> &channels);
	
	void send_mode(const std::string &channel_or_nick, const std::string &mode, const std::vector<std::string> &args);

	void send_topic(const std::string &channel, const std::string &topic);
	
	void send_topic(const std::string &channel);

	void send_names(const std::string &channel = "");
	
	void send_names(const std::list<std::string> &channels);
	
	void send_list(const std::string &channel = "", const std::string &server = "");
	
	void send_list(const std::list<std::string> &channels, const std::string &server = "");
	
	void send_invite(const std::string &nickname, const std::string &channel);
	
	void send_kick(const std::string &channel, const std::string &user, const std::string &comment = "");
	
	void send_kick(const std::list<std::string> &channels, const std::list<std::string> &users, const std::string &comment = "");

	void send_version(const std::string &server = "");

	void send_stats(const std::string &query = "", const std::string &server = "");
	
	void send_links(const std::string &remote_server = "", const std::string &server_mask = "");
	
	void send_time(const std::string &server = "");
	
	void send_connect(const std::string &target_server, int port = 0, const std::string &remote_server = "");
	
	void send_trace(const std::string &server = 0);
	
	void send_admin(const std::string &server = 0);
	
	void send_info(const std::string &server = 0);
	
	void send_privmsg(const std::string &receiver, const std::string &text);
	
	void send_privmsg(const std::list<std::string> &receivers, const std::string &text);
	
	void send_notice(const std::string &nickname, const std::string &text);
	
	void send_who(const std::string &name, bool oper = false);
	
	void send_whois(const std::string &nickmask);
	
	void send_whois(const std::string &server, const std::string &nickmask);
	
	void send_whois(const std::list<std::string> &nickmasks);
	
	void send_whois(const std::string &server, const std::list<std::string> &nickmasks);
	
	void send_whowas(const std::string &nickname, int count = 0, const std::string &server = "");
	
	void send_kill(const std::string &nickname, const std::string &comment);
	
	void send_ping(const std::string &server, const std::string &server2 = "");

	void send_pong(const std::string &daemon, const std::string &daemon2 = "");

//! Signals:
public:
	//: sig_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_command_received();

	//: sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_unknown_command_received();

	//: sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const std::string &, int, const std::vector<std::string> &> &sig_numeric_reply();
	
	//: sig_name_reply(self, channel, users)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_name_reply();

	//: sig_nick(old_nick, new_nick)
	CL_Signal_v2<const std::string &, const std::string &> &sig_nick();

	//: sig_join(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> &sig_join();
	
	//: sig_part(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> &sig_part();
	
	//: sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::vector<std::string> &> &sig_mode();

	//: sig_topic(prefix, channel, topic)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_topic();
	
	//: sig_invite(prefix, nick, channel)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_invite();

	//: sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> &sig_kick();

	//: sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_privmsg();

	//: sig_notice(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_notice();
	
	//: sig_ping(daemon1, daemon2)
	CL_Signal_v2<const std::string &, const std::string &> &sig_ping();

//! Implementation:
private:
	CL_IRCConnection_Generic *impl;
};

#endif
