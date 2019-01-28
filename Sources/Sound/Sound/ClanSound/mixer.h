/*
	$Id: mixer.h,v 1.2 2001/09/08 19:24:21 japj Exp $
	
	ClanSound, sound mixing library written for ClanLib.
	Copyright (c)1998 Magnus Norddahl / ClanSoft.
*/

#ifndef header_mixer
#define header_mixer

#ifdef WIN32
#include "directsound.h"
#else
#include "oss.h"
#endif

#include <list>

class CL_CardPlayback_ClanSound;

class CL_CSMixer
{
public:
	CL_CSMixer();
	~CL_CSMixer();

	void mix(class CL_Mutex *mutex);
	// Mix one fragment.
	
	void wait();
	// Waits until output source isn't full anymore.
	
	void add(CL_CardPlayback_ClanSound *playback);
	// Adds a playback session to the mixer.

	void remove(CL_CardPlayback_ClanSound *playback);
	// Removes a playback session from the mixer.

private:
	std::list<CL_CardPlayback_ClanSound*> playbacks;
	CL_CSOutput output;

	short *mix_buffer;
	int *mix_buffer2;
	int mix_buffer_size;
};

#endif
