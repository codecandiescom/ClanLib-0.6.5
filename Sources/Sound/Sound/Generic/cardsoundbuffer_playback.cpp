/*
	$Id: cardsoundbuffer_playback.cpp,v 1.1 2001/03/06 15:09:26 mbn Exp $

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

#include "API/Sound/soundfilter.h"
#include "cardsoundbuffer_playback.h"

CL_CardSoundBuffer_Playback::CL_CardSoundBuffer_Playback()
{
}

CL_CardSoundBuffer_Playback::~CL_CardSoundBuffer_Playback()
{
	std::list<FilterPair>::iterator it;
	for (it = filters.begin(); it != filters.end(); it++)
	{
		if (it->second == true) delete it->first;
	}
}

void CL_CardSoundBuffer_Playback::add_filter(
	CL_SoundFilter *filter,
	bool delete_filter)
{
	filters.push_back(FilterPair(filter, delete_filter));
}

void CL_CardSoundBuffer_Playback::remove_filter(CL_SoundFilter *filter)
{
	std::list<FilterPair>::iterator it;
	for (it = filters.begin(); it != filters.end(); it++)
	{
		if (it->first == filter)
		{
			filters.erase(it);
			break;
		}
	}
}

void CL_CardSoundBuffer_Playback::filter(int *sample_data, int num_samples)
{
	std::list<FilterPair>::iterator it;
	for (it = filters.begin(); it != filters.end(); it++)
	{
		it->first->filter(sample_data, num_samples);
	}
}
