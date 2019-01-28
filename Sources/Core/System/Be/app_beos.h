/*
	$Id: app_beos.h,v 1.2 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Header file to the BeOS initialization.

*/

#ifndef header_app_beos
#define header_app_beos

#define USE_DBG

#ifdef USE_DBG
	#include <iostream>
	#define DBG(x) cout << x << endl
#else
	#define DBG(x)
#endif

class ClanWindowScreen;

#include <Application.h>

class LibApplication : public BApplication
{
public:
	LibApplication();
	bool is_quitting;

	ClanWindowScreen* clanscreen;
	
private:
	bool QuitRequested();
	void ReadyToRun();
	void ArgvReceived(int32 argc, char **argv);

	int32 largc;
	char** largv;
};

#endif
