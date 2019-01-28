/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "document.h"

// Construction:
Document::Document(
	const std::string &hostname, int port,
	const std::string &username, const std::string &password)
//: netsession("CTalk", hostname, port)
: netsession("CTalk", port) // note: this is wrong, but then we dont need to start a server :)
{
	// send username & password to server.
}

Document::~Document()
{
}

// Attributes:

// Operations:
void Document::update_text(const std::string &text)
{
	// send text to server.
}

// implementation:

void Document::keep_alive()
{
	// check for incoming text or system messages.
	// check for new users added.
	// check for users removed.
}
