/*
	$Id: network.h,v 1.9 2001/12/11 20:44:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Network component header file.


*/

//! clanNetwork="System"
//! header=network.h

#ifndef header_network
#define header_network

#include <string>
class CL_NetSession;

//: Network component class. Starting place for all net activity in ClanLib.
class CL_Network
{
public:
//! Operations:
	//: Searchs for games using UDP broadcasting at the specified 'port'. 
	//: Only games with the same 'game_id' are added to CL_Network::netsessions.
	//- game_id - The game's ID used to locate this type of game with.
	//- port - Broadcast UDP port.
	static void find_sessions_broadcast(
		std::string game_id,
		int port);
	
	//: Searchs for games at the specified host address and port ('port').
	//- host_address - IP address or hostname of the computer to be contacted.
	//- port - Port which the computer contacted listens on.
	static void find_session_at(
		std::string app_id,
		const std::string &host_address,
		int port);
	
	//: <p>Returns true if any netsessions have been found. Use receive_game_found() to
	//: get the individual games found.</p>
	//- Returns - True if any netsessions have been found.
	static bool peek_session_found();

	//: <p>Returns the first netsession found. It is then removed from the list of games
	//: found. Next time it is called it will return the next in available. </p>
	//: <p>The function is blocking until the specified timeout runs out.
	//: If set to zero, it will block forever until a game has been found.</p>
	//: Remember to delete the netsession returned after usage.
	//- timeout_millis - Timeout before returning if no game was found.
	//- Returns - The netsession found, or NULL if timed out. Remember to delete the
	//- Returns - netsession after usage.
	static CL_NetSession *receive_session_found(int timeout_millis);

	//: Clears the list of sessions found.
	static void clear_sessions_found();

	//: Creates a new network game.
	//- app_id - The app ID is used to locate netsessions of the same type.
	//- port - Port the game accepts connections on - and listens for broadcasts.
	//- Returns - The created network game.
	static CL_NetSession *create_session(
		std::string app_id,
		int port);
};

#endif
