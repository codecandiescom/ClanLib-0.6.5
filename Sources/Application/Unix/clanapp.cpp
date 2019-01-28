/*
	$Id: clanapp.cpp,v 1.5 2001/05/09 15:41:36 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include <cstdlib>
#include <iostream>
#include "API/Core/System/setupcore.h"
#include "API/Application/clanapp.h"

/////////////////////////////////////////////////////////////////////////////
// main:

int main(int argc, char **argv)
{
	if (CL_ClanApplication::app == NULL)
	{
		std::cout << "ClanLib: No global CL_ClanApplication instance!!!" << std::endl;
		return 255;
	}

	CL_SetupCore::init();
	int retval = CL_ClanApplication::app->main(argc, argv);
	CL_SetupCore::deinit();

	return retval;
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
