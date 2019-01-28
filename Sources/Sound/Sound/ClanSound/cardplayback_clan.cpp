/*
	$Id: cardplayback_clan.cpp,v 1.2 2001/08/30 18:50:39 plasmoid Exp $

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

#ifdef USE_CLANSOUND

#include <Sound/Sound/ClanSound/cardplayback_clan.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>

CL_CardPlayback_ClanSound::CL_CardPlayback_ClanSound(
	CL_SoundCard_ClanSound *card)
{
	this->card = card;

}

CL_CardPlayback_ClanSound::~CL_CardPlayback_ClanSound()
{
	card = NULL;
}

#endif
