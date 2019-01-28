/*
ClanLib and Lua Example

Created by:
	Lenny Palozzi
	domain@ica.net
	
Oct. 22, 1999.
*/

#include <ClanLib/core.h>
#include <ClanLib/lua.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <iostream>

using namespace std;

class LuaTestApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Lua Script Example Application"; }
	
	virtual int main(int argc, char **argv)
	{
		try
		{
			// Create a console window for text-output if not available
			CL_ConsoleWindow console("Console");
			console.redirect_stdio();

			CL_SetupCore::init();
			CL_SetupDisplay::init();
			//CL_Display::set_videomode(640,480,16,false);
			CL_Lua::init();
			CL_Lua::strlibopen();
			CL_Lua::mathlibopen();
			//int result=CL_Lua::dofile("ex2.lua");
			int result=CL_Lua::dofile("script.lua");
			if (result)
			{
			
			}
			cout << result << endl;
		
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
			return -1;
		}
		catch (const string s)
		{
			std::cout << "Exception caught: " << s  << std::endl;
			return -1;
		}
		catch (const char *s)
		{
			std::cout << "Exception caught: " << s  << std::endl;
			return -1;
		}
		catch (...)
		{
			std::cout << "Unknown exception caught: " << std::endl;
		}
		CL_Lua::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();

		return 0;
	}
} app;
