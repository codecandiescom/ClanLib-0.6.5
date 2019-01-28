/*
	$Id: soundcard_generic.cpp,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <Sound/Sound/Generic/cardsession_manager.h>
#include <Sound/Sound/Generic/soundcard_generic.h>

CL_SoundCard_Generic::CL_SoundCard_Generic()
{
	manager = new CL_CardSessionManager();
}

CL_SoundCard_Generic::~CL_SoundCard_Generic()
{
	delete manager;
}

