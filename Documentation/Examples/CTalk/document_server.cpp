/*
	Simple GUI & Network demo application,
	Copyright (c) 2000 by Magnus Norddahl and Kenneth Gangstoe.
*/

#include "document_server.h"

// Construction:
DocumentServer::DocumentServer(int port)
: netsession("CTalk", port)
{
}

// Attributes:

// Operations:

// implementation:

void DocumentServer::keep_alive()
{
}
