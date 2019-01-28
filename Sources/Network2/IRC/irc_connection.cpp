
#include "API/Network2/IRC/irc_connection.h"
#include "irc_connection_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection construction:

CL_IRCConnection::CL_IRCConnection() : impl(0)
{
}

CL_IRCConnection::CL_IRCConnection(const std::string &server, const std::string &port) :
	impl(new CL_IRCConnection_Generic(server, port))
{
	impl->add_ref();
}

CL_IRCConnection::CL_IRCConnection(const CL_IRCConnection &copy) : impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_IRCConnection::~CL_IRCConnection()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection attributes:

const std::string &CL_IRCConnection::get_nick() const
{
	return impl->nick;
}

const std::string &CL_IRCConnection::get_username() const
{
	return impl->username;
}

const std::string &CL_IRCConnection::get_hostname() const
{
	return impl->hostname;
}

const std::string &CL_IRCConnection::get_servername() const
{
	return impl->servername;
}

const std::string &CL_IRCConnection::get_realname() const
{
	return impl->realname;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection operations:

CL_IRCConnection &CL_IRCConnection::operator =(const CL_IRCConnection &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_IRCConnection::send_command(const std::string &prefix, const std::string &command, const std::vector<std::string> &params)
{
	std::string line;
	if (!prefix.empty())
	{
		line.append(":");
		line.append(prefix);
		line.append(" ");
	}
	
	line.append(command);
	
	int size = params.size();
	for (int i=0; i<size;i++)
	{
		line.append(" ");
		if (i == size-1) line.append(":");
		line.append(params[i]);
	}
	
	line.append("\r\n");
	impl->sock.send(line);
}

void CL_IRCConnection::send_pass(const std::string &password)
{
	std::vector<std::string> params;
	params.push_back(password);
	send_command("", "PASS", params);
}

void CL_IRCConnection::send_nick(const std::string &nick, int hopcount)
{
	std::vector<std::string> params;
	params.push_back(nick);
	// if (hopcount > 0) params.push_back(CL_String::from_int(hopcount));
	send_command("", "NICK", params);

	impl->nick = nick;
}

void CL_IRCConnection::send_user(const std::string &username, const std::string &hostname, const std::string &servername, const std::string &realname)
{
	std::vector<std::string> params;
	params.push_back(username);
	params.push_back(hostname);
	params.push_back(servername);
	params.push_back(realname);
	send_command("", "USER", params);

	impl->username = username;
	impl->hostname = hostname;
	impl->servername = servername;
	impl->realname = realname;
}

void CL_IRCConnection::send_oper(const std::string &user, const std::string &password)
{
	std::vector<std::string> params;
	params.push_back(user);
	params.push_back(password);
	send_command("", "OPER", params);
}

void CL_IRCConnection::send_quit(const std::string &quitmessage)
{
	std::vector<std::string> params;
	if (!quitmessage.empty()) params.push_back(quitmessage);
	send_command("", "QUIT", params);
}

void CL_IRCConnection::send_join(const std::string &channel, const std::string &key)
{
	std::list<std::string> channels;
	std::list<std::string> keys;
	
	channels.push_back(channel);
	keys.push_back(key);
	
	send_join(channels, keys);
}

void CL_IRCConnection::send_join(const std::list<std::string> &channels, const std::list<std::string> &keys)
{
	std::list<std::string>::const_iterator it, it_keys;
	std::string channel_list, key_list;
	bool first = true;
	
	// Add channels with keys first. Limitation in IRC protocol.
	for (it = channels.begin(), it_keys = keys.begin(); it != channels.end() && it_keys != keys.end(); ++it, ++it_keys)
	{
		if (it_keys->empty()) continue;
		
		if (!first)
		{
			channel_list.append(",");
			key_list.append(",");
		}
		first = false;
		channel_list.append(*it);
		key_list.append(*it_keys);
	}

	// Add channels without keys.
	for (it = channels.begin(), it_keys = keys.begin(); it != channels.end() && it_keys != keys.end(); ++it, ++it_keys)
	{
		if (!it_keys->empty()) continue;
		
		if (!first)
		{
			channel_list.append(",");
		}
		first = false;
		channel_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(channel_list);
	if (!key_list.empty()) params.push_back(key_list);
	send_command("", "JOIN", params);
}

void CL_IRCConnection::send_part(const std::string &channel)
{
	std::list<std::string> channels;
	channels.push_back(channel);
	send_part(channels);
}

void CL_IRCConnection::send_part(const std::list<std::string> &channels)
{
	std::list<std::string>::const_iterator it;
	std::string channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(channel_list);
	send_command("", "PART", params);
}

void CL_IRCConnection::send_mode(const std::string &channel_or_nick, const std::string &mode, const std::vector<std::string> &args)
{
	std::vector<std::string> params;
	params.push_back(channel_or_nick);
	params.push_back(mode);
	int size = args.size();
	for (int i=0; i<size; i++) params.push_back(args[i]);
	send_command("", "MODE", params);
}

void CL_IRCConnection::send_topic(const std::string &channel, const std::string &topic)
{
	std::vector<std::string> params;
	params.push_back(channel);
	params.push_back(topic);
	send_command("", "TOPIC", params);
}

void CL_IRCConnection::send_topic(const std::string &channel)
{
	std::vector<std::string> params;
	params.push_back(channel);
	send_command("", "TOPIC", params);
}

void CL_IRCConnection::send_names(const std::string &channel)
{
	std::list<std::string> channels;
	channels.push_back(channel);
	send_names(channels);
}

void CL_IRCConnection::send_names(const std::list<std::string> &channels)
{
	std::list<std::string>::const_iterator it;
	std::string channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(channel_list);
	send_command("", "NAMES", params);
}

void CL_IRCConnection::send_list(const std::string &channel, const std::string &server)
{
	std::list<std::string> channels;
	channels.push_back(channel);
	send_list(channels, server);
}

void CL_IRCConnection::send_list(const std::list<std::string> &channels, const std::string &server)
{
	std::list<std::string>::const_iterator it;
	std::string channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(channel_list);
	if (!server.empty()) params.push_back(server);
	send_command("", "LIST", params);
}

void CL_IRCConnection::send_invite(const std::string &nickname, const std::string &channel)
{
	std::vector<std::string> params;
	params.push_back(nickname);
	params.push_back(channel);
	send_command("", "INVITE", params);
}

void CL_IRCConnection::send_kick(const std::string &channel, const std::string &user, const std::string &comment)
{
	std::list<std::string> channels, users;
	channels.push_back(channel);
	users.push_back(user);
	send_kick(channels, users, comment);
}

void CL_IRCConnection::send_kick(const std::list<std::string> &channels, const std::list<std::string> &users, const std::string &comment)
{
	std::list<std::string>::const_iterator it;
	std::string channel_list, user_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}
	
	first = true;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (!first) user_list.append(",");
		first = false;
		user_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(channel_list);
	params.push_back(user_list);
	if (!comment.empty()) params.push_back(comment);
	send_command("", "KICK", params);
}

void CL_IRCConnection::send_version(const std::string &server)
{
	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "VERSION", params);
}

void CL_IRCConnection::send_stats(const std::string &query, const std::string &server)
{
	std::vector<std::string> params;
	if (!query.empty()) params.push_back(query);
	if (!server.empty()) params.push_back(server);
	send_command("", "STATS", params);
}

void CL_IRCConnection::send_links(const std::string &remote_server, const std::string &server_mask)
{
	std::vector<std::string> params;
	if (!remote_server.empty()) params.push_back(remote_server);
	if (!server_mask.empty()) params.push_back(server_mask);
	send_command("", "LINKS", params);
}

void CL_IRCConnection::send_time(const std::string &server)
{
	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "TIME", params);
}

void CL_IRCConnection::send_connect(const std::string &target_server, int port, const std::string &remote_server)
{
	std::vector<std::string> params;
	params.push_back(target_server);
	// if (port) params.push_back(CL_String::from_int(port));
	if (!remote_server.empty()) params.push_back(remote_server);
	send_command("", "CONNECT", params);
}

void CL_IRCConnection::send_trace(const std::string &server)
{
	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "TRACE", params);
}

void CL_IRCConnection::send_admin(const std::string &server)
{
	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "ADMIN", params);
}

void CL_IRCConnection::send_info(const std::string &server)
{
	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "INFO", params);
}

void CL_IRCConnection::send_privmsg(const std::string &receiver, const std::string &text)
{
	std::list<std::string> receivers;
	receivers.push_back(receiver);
	send_privmsg(receivers, text);
}

void CL_IRCConnection::send_privmsg(const std::list<std::string> &receivers, const std::string &text)
{
	std::list<std::string>::const_iterator it;
	std::string receiver_list;
	bool first = true;

	for (it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (!first) receiver_list.append(",");
		first = false;
		receiver_list.append(*it);
	}

	std::vector<std::string> params;
	params.push_back(receiver_list);
	params.push_back(text);
	send_command("", "PRIVMSG", params);
}

void CL_IRCConnection::send_notice(const std::string &nickname, const std::string &text)
{
	std::vector<std::string> params;
	params.push_back(nickname);
	params.push_back(text);
	send_command("", "NOTICE", params);
}

void CL_IRCConnection::send_who(const std::string &name, bool oper)
{
	std::vector<std::string> params;
	params.push_back(name);
	if (oper) params.push_back("o");
	send_command("", "WHO", params);
}

void CL_IRCConnection::send_whois(const std::string &nickmask)
{
	std::vector<std::string> params;
	params.push_back(nickmask);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whois(const std::string &server, const std::string &nickmask)
{
	std::vector<std::string> params;
	params.push_back(server);
	params.push_back(nickmask);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whois(const std::list<std::string> &nickmasks)
{
	send_whois("", nickmasks);
}

void CL_IRCConnection::send_whois(const std::string &server, const std::list<std::string> &nickmasks)
{
	std::list<std::string>::const_iterator it;
	std::string nickmask_list;
	bool first = true;

	for (it = nickmasks.begin(); it != nickmasks.end(); ++it)
	{
		if (!first) nickmask_list.append(",");
		first = false;
		nickmask_list.append(*it);
	}

	std::vector<std::string> params;
	if (!server.empty()) params.push_back(server);
	params.push_back(nickmask_list);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whowas(const std::string &nickname, int count, const std::string &server)
{
	std::vector<std::string> params;
	params.push_back(nickname);
	// if (count > 0) params.push_back(CL_String::from_int(count));
	if (!server.empty()) params.push_back(server);
	send_command("", "WHOWAS", params);
}

void CL_IRCConnection::send_kill(const std::string &nickname, const std::string &comment)
{
	std::vector<std::string> params;
	params.push_back(nickname);
	if (!comment.empty()) params.push_back(comment);
	send_command("", "KILL", params);
}

void CL_IRCConnection::send_ping(const std::string &server, const std::string &server2)
{
	std::vector<std::string> params;
	params.push_back(server);
	if (!server2.empty()) params.push_back(server2);
	send_command("", "PING", params);
}

void CL_IRCConnection::send_pong(const std::string &daemon, const std::string &daemon2)
{
	std::vector<std::string> params;
	params.push_back(daemon);
	if (!daemon2.empty()) params.push_back(daemon2);
	send_command("", "PONG", params);
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection signals:

CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &CL_IRCConnection::sig_command_received()
{
	return impl->sig_command_received;
}

CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &CL_IRCConnection::sig_unknown_command_received()
{
	return impl->sig_unknown_command_received;
}

CL_Signal_v3<const std::string &, int, const std::vector<std::string> &> &CL_IRCConnection::sig_numeric_reply()
{
	return impl->sig_numeric_reply;
}
	
CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &CL_IRCConnection::sig_name_reply()
{
	return impl->sig_name_reply;
}

CL_Signal_v2<const std::string &, const std::string &> &CL_IRCConnection::sig_nick()
{
	return impl->sig_nick;
}

CL_Signal_v2<const std::string &, const std::string &> &CL_IRCConnection::sig_join()
{
	return impl->sig_join;
}
	
CL_Signal_v2<const std::string &, const std::string &> &CL_IRCConnection::sig_part()
{
	return impl->sig_part;
}
	
CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::vector<std::string> &> &CL_IRCConnection::sig_mode()
{
	return impl->sig_mode;
}

CL_Signal_v3<const std::string &, const std::string &, const std::string &> &CL_IRCConnection::sig_topic()
{
	return impl->sig_topic;
}

CL_Signal_v3<const std::string &, const std::string &, const std::string &> &CL_IRCConnection::sig_invite()
{
	return impl->sig_invite;
}

CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> &CL_IRCConnection::sig_kick()
{
	return impl->sig_kick;
}

CL_Signal_v3<const std::string &, const std::string &, const std::string &> &CL_IRCConnection::sig_privmsg()
{
	return impl->sig_privmsg;
}

CL_Signal_v3<const std::string &, const std::string &, const std::string &> &CL_IRCConnection::sig_notice()
{
	return impl->sig_notice;
}
	
CL_Signal_v2<const std::string &, const std::string &> &CL_IRCConnection::sig_ping()
{
	return impl->sig_ping;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection implementation:

