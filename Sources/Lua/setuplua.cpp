/*
	$Id: setuplua.cpp,v 1.2 2001/09/08 19:12:54 japj Exp $
	
	 ------------------------------------------------------------------------
	 ClanLib, the platform independent game SDK.

	 This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	 version 2. See COPYING for details.

	 For a total list of contributers see CREDITS.

	 ------------------------------------------------------------------------
*/

#include <API/core.h>
#include <API/lua.h>

lua_State	*CL_Lua::state;
bool CL_Lua::initialized=false;
