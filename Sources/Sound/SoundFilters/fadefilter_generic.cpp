/*
	$Id: fadefilter_generic.cpp,v 1.1 2001/03/06 15:09:26 mbn Exp $

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

#include "fadefilter_generic.h"

CL_FadeFilter::CL_FadeFilter(int initial_volume)
{
	impl = new CL_FadeFilter_Generic;
	impl->cur_volume = initial_volume/100.0f;
	impl->new_volume = initial_volume/100.0f;
	impl->speed = 0;
}

CL_FadeFilter::~CL_FadeFilter()
{
	delete impl;
}

void CL_FadeFilter::fade_to_volume(int new_volume, int duration)
{
	impl->new_volume = new_volume/100.0f;

	float delta_volume = impl->new_volume - impl->cur_volume;
	impl->speed = delta_volume / 22.05f / duration;
}

void CL_FadeFilter::filter(int *data, int size)
{
	float &cur_volume = impl->cur_volume;
	float &new_volume = impl->new_volume;
	float &speed = impl->speed;

	for (int i=0; i<size*2; i++)
	{
		data[i] = (int) (data[i] * cur_volume);

		if ((i&1) == 1) // change volume for every second sample (because data is in stereo).
		{
			cur_volume += speed;
			if (
				(speed > 0 && cur_volume > new_volume) ||
				(speed < 0 && cur_volume < new_volume))
			{
				cur_volume = new_volume;
				speed = 0;
			}
		}
	}
}
