/*
	$Id: clanapp.cpp,v 1.6 2001/09/25 18:49:59 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include <windows.h>
#include <vector>
#include "API/Core/System/setupcore.h"
#include "API/Application/clanapp.h"
#include <crtdbg.h>

static void calc_commandline(int *argc, char ***argv);

/////////////////////////////////////////////////////////////////////////////
// WinMain:

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	int retval;
	int argc;
	char **argv;

#if defined(_DEBUG) && defined(_MSC_VER)
	// Visual C++ memoryleak debugging. By setting the _CRTDBG_LEAK_CHECK_DF
	// flag, we produce a memory leak dump at exit in the visual c++ debug output
	// window.
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif

	// Did the game developer remember to create one global application
	// interface?
	if (CL_ClanApplication::app == NULL)
	{
		MessageBox(NULL, "No program instance found", "ClanLib/Win32", 32);
		return 0;
	}

	// Get commandline arguments.
	calc_commandline(&argc, &argv);

	// Initialize clanlib and call clanapp main:
	CL_SetupCore::set_instance(hInstance);
	CL_SetupCore::init();
	retval = CL_ClanApplication::app->main(argc, argv);
	CL_SetupCore::deinit();

	// calc_commandline() doesn't clean up after itself. tsk tsk:
	delete[] argv;

	return retval;
}

/////////////////////////////////////////////////////////////////////////////
// calc_commandline:

static void calc_commandline(int *argc, char ***argv)
{
	char *command_line = GetCommandLine();
	static std::vector<char *> pos;
	bool state_gaaseoejne = false;
	bool new_arg = true;

	for (;*command_line;command_line++) 
	{
		if (*command_line == '"')
		{
			new_arg = true;
			command_line++;
			char *start_arg = command_line;
			for (;*command_line && *command_line!='"';command_line++)
			{
				if (!isspace(*command_line)) new_arg = false;
			}
			if (new_arg == false) pos.push_back(start_arg);
			if (*command_line == '"') *command_line = 0;
			else if (*command_line == 0) break;
			command_line++;
			if (*command_line == 0) break;
			new_arg = true;
		}
		
		if (new_arg && !isspace(*command_line))
		{
			new_arg = false;
			pos.push_back(command_line);
		}
		else if (!new_arg && isspace(*command_line))
		{
			new_arg = true;
			*command_line = 0;
		}
	}
	int num_words = pos.size();

	char **words = new char*[num_words + 1];

	int i;
	for (i=0; i<num_words; i++)
	{
		words[i] = pos[i];
	}
	words[i] = NULL;

	*argc = num_words;
	*argv = words;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ClanApplication:

CL_ClanApplication *CL_ClanApplication::app = NULL;

CL_ClanApplication::CL_ClanApplication()
{
	CL_ClanApplication::app = this;
}

CL_ClanApplication::~CL_ClanApplication()
{
	CL_ClanApplication::app = NULL;
}
