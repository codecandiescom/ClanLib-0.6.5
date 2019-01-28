/*
	$Id: lua.cpp,v 1.5 2001/12/05 12:57:18 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
/*
	Test wether LUA library is correctly installed.
*/

#include <lua.h>
#include <stdio.h>
#include <tolua.h>
#undef class

int main(int, char**)
{
	const char *version = LUA_VERSION;
		if (version[strlen(version)-3]!='4')
		{
		printf("you need Lua 4.0 or greater\n");
		return 1;
		}
	return 0;
}
