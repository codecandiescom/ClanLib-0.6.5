         Name: LUA
       Status: Windows(Y), Linux(Y)
        Level: Advanced
      Modules: app, core, lua, display
Last modified: 16. october 2001
     Progress: Not complete
   Maintainer: Lenny Palozzi (domain@ica.net)
      Summary: LUA scripting, ClanLib bindings

Demonstrates how you can call ClanLib functions from LUA scripting code.

Requirements
------------
- A working installation of lua. Its should be installed as a shared library.

- Compile and install the clanLua module with:
	
	1. make clanLua
	2. make install 

As of this writing, the binding code has been created with the latest version of tolua, 3.2, and used in conjuction with the latest version of Lua, 3.2.

Using
-----
You simply link your application with libclanLua.

	g++ lua_test.cpp -lclanCore -lclanLua

Links
-----
Home page:  http://www.tecgraf.puc-rio.br/lua/
            http://csg.uwaterloo.ca/~lhf/lua/  
