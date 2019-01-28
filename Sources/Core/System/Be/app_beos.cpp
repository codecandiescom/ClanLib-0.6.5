/*
	$Id: app_beos.cpp,v 1.2 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		main. Initialization of the BeOS version of ClanLib.

*/

#include "app_beos.h"
#include "API/Core/System/clanapp.h"
#include "Core/Display/Be/clanwindowscreen.h"

#include <OS.h>
#include <Alert.h>

extern "C"
{
	#include <Hermes/Hermes.h>
}

// Minor hack. The datafile compiler calls this function. Under win32 it will
// create a console window for cout.
void redirect_to_console(const char *title)
{
}

int main()
{
	DBG("initializing Hermes...");
	Hermes_Init();
	
	LibApplication* lapp = new LibApplication();
	lapp->Run();
	delete lapp;
}

LibApplication::LibApplication() : BApplication("application/x-vnd.ClanSoft-ClanLib")
{
}

void LibApplication::ReadyToRun()
{
	if (CL_ClanApplication::app == NULL)
	{
		BAlert* noProgram = new BAlert("ClanLib", "No program instance found", "Kick ass!", "Use Be!", "Have fun!", B_WIDTH_AS_USUAL, B_STOP_ALERT);
		noProgram->Go();
		PostMessage(B_QUIT_REQUESTED);
		return;
	}

	DBG("starting app's main");
	CL_ClanApplication::app->main(largc, largv);

	DBG("quitting");
	PostMessage(B_QUIT_REQUESTED);
}

void LibApplication::ArgvReceived(int32 argc, char **argv)
{
	largc = argc;
	largv = new char*[argc];
	for (int i=0; i<argc; i++)
	{
		largv[i] = new char[strlen(argv[i])];
		strcpy( largv[i], argv[i] );
	}
}

bool LibApplication::QuitRequested()
{
	DBG("QuitRequested()");
	is_quitting = true;
	return true;
}
