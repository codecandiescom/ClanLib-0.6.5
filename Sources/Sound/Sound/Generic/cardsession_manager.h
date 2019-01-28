/*
	$Id: cardsession_manager.h,v 1.2 2001/05/14 19:19:42 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardsession_manager
#define header_cardsession_manager

#include "API/Core/System/cl_assert.h"
#include "cardsoundbuffer_playback.h"
#include "soundbuffer_generic.h"

#include <vector>

class CL_CardSessionManager
{
public:
	CL_CardSessionManager();
	virtual ~CL_CardSessionManager();

	void *add_playback(CL_CardSoundBuffer_Playback *playback_buffer, CL_SoundBuffer_Generic *buffer);
	void add_reference(void *handle);
	void remove_reference(void *handle);
	void remove_soundbuffer_playbacks(CL_SoundBuffer_Generic *playback);
	void remove_all_playbacks();
	bool is_soundbuffer_playing(CL_SoundBuffer *playback);

	CL_CardSoundBuffer_Playback *get_playback(void *handle)
	{
		// Totally safe type checking (not)
		SPlayBackStruct *s = (SPlayBackStruct *) handle;
		return s->playback;
	}

private:
	struct SPlayBackStruct
	{
		CL_CardSoundBuffer_Playback *playback;
		int reference_count;
		CL_SoundBuffer_Generic *soundbuffer;

		SPlayBackStruct(CL_CardSoundBuffer_Playback *_playback, CL_SoundBuffer_Generic *buffer);
		virtual ~SPlayBackStruct();
	};

	std::list<SPlayBackStruct*> playbacks;
	void garbage_collect();
};

#endif
