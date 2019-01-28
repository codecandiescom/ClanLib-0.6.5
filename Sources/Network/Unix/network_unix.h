/*
	$Id: network_unix.h,v 1.2 2001/09/08 19:24:20 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_network_win32
#define header_network_win32

#include "Core/System/Unix/init_linux.h"

class CL_Network_Generic;

char *network_identify();
char *network_abbreviation();
void network_init();


class CL_Network_Unix
{
public:
	static CL_Network_Generic *network;
};

class CL_Implementation_Network
{
public:
	static void add_network();
	static void remove_network();
};

#endif
