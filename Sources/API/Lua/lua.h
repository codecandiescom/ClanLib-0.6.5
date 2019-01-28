/*
	$Id: lua.h,v 1.14 2001/09/22 15:52:16 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanLua="Scripting"
//! header=lua.h

#ifndef header_lua_lib
#define header_lua_lib

extern "C" {
#include <lua.h>
#include <lualib.h>
}

/*
The original classes from Waldemer Celes adapted by Lenny Palozzi were replaced by this
to get it to work with Lua and tolua 4.0
jpsousa
*/

int tolua_clanbindings_open (lua_State* tolua_S);
void tolua_clanbindings_close (lua_State* tolua_S);

//: Lua Class
class CL_Lua{
public:
//! Variables:
	//: state
	static lua_State *state;

	//: Initialised
	static bool initialized;

//! Operations:
	//: Init
	static void init()
	{
		state=lua_open(0);
		tolua_clanbindings_open(state);
		lua_baselibopen(state);
		initialized=true;
	}

	//: DeInit
	static void deinit()
	{
		tolua_clanbindings_close(state);
		lua_close(state);
		initialized=false;
	}

	//: DoFile
	static int dofile (char *filename)
	{ 
		cl_assert(initialized);
		return lua_dofile(state,filename);
	}


	//: Open the IO library
	static void iolibopen(void) { lua_iolibopen(state); }

	//: Open the string library
	static void strlibopen(void)
	{ 
		cl_assert(initialized);
		lua_strlibopen(state);
	}

	//: Open the math library
	static void mathlibopen(void)
	{ 
		cl_assert(initialized);
		lua_mathlibopen(state);
	}

	//: Open the db library
	static void dblibopen(void)
	{ 
		cl_assert(initialized);
		lua_dblibopen(state);
	}
};
#endif








