/*
	$Id: cardsession_manager.cpp,v 1.4 2001/10/01 20:42:17 mbn Exp $

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
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>
#include <API/Sound/soundbuffer.h>

CL_CardSessionManager::CL_CardSessionManager()
{
}

CL_CardSessionManager::~CL_CardSessionManager()
{
	remove_all_playbacks();
}

void *CL_CardSessionManager::add_playback(
	CL_CardSoundBuffer_Playback *playback_buffer,
	CL_SoundBuffer_Generic *buffer)
{
	garbage_collect();

	SPlayBackStruct *s = new SPlayBackStruct(playback_buffer, buffer);
	playbacks.push_back(s);
	return s;
}

void CL_CardSessionManager::add_reference(void *handle)
{
	SPlayBackStruct *s = (SPlayBackStruct *) handle;
	s->reference_count++;
}

void CL_CardSessionManager::remove_reference(void *handle)
{
	SPlayBackStruct *s = (SPlayBackStruct *) handle;
	s->reference_count--;

	if (s->reference_count == 0 && !s->playback->is_playing())
	{
		playbacks.remove(s);
		delete s;
	}
}

void CL_CardSessionManager::garbage_collect()
{
	for (
		std::list<SPlayBackStruct*>::iterator it = playbacks.begin();
		it != playbacks.end();)
	{
		if ((*it)->reference_count == 0 &&
			!(*it)->playback->is_playing())
		{
			delete *it;
			it = playbacks.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CL_CardSessionManager::remove_soundbuffer_playbacks(
	CL_SoundBuffer_Generic *soundbuffer)
{
	std::list<SPlayBackStruct*>::iterator it = playbacks.begin();
	while (it != playbacks.end())
	{
		if ((*it)->playback->get_owner() == soundbuffer)
		{
			delete *it;
			it = playbacks.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CL_CardSessionManager::remove_all_playbacks()
{
	for (
		std::list<SPlayBackStruct*>::iterator it = playbacks.begin();
		it != playbacks.end();
		it++)
	{
		delete *it;
	}
	playbacks.clear();
}

CL_CardSessionManager::SPlayBackStruct::SPlayBackStruct(
	CL_CardSoundBuffer_Playback *_playback,
	CL_SoundBuffer_Generic *_buffer)
{
	playback = _playback;
	soundbuffer = _buffer;
	reference_count = 1;
	soundbuffer->add_reference();
}

CL_CardSessionManager::SPlayBackStruct::~SPlayBackStruct()
{ 
	delete playback;
	playback = NULL;
	soundbuffer->release_reference();
}

bool CL_CardSessionManager::is_soundbuffer_playing(CL_SoundBuffer *playback)
{
	std::list<SPlayBackStruct*>::iterator it = playbacks.begin();
	while (it != playbacks.end())
	{
		CL_SoundBuffer_Generic *owner = (*it)->playback->get_owner();
		bool playing = (*it)->playback->is_playing();
		if (owner == playback->impl && playing)
		{
			return true;
		}
		it++;
	}
	return false;
}
