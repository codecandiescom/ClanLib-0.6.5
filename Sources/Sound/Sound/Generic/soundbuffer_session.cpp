/*
	$Id: soundbuffer_session.cpp,v 1.3 2001/08/30 18:50:39 plasmoid Exp $

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

#include <API/Sound/soundbuffer_session.h>
#include <API/Sound/sound.h>
#include <Sound/Sound/Generic/cardsoundbuffer_playback.h>
#include <Sound/Sound/Generic/soundcard_generic.h>
#include <Sound/Sound/ClanSound/soundcard_clan.h>

#include "cardsession_manager.h"

CL_CardSoundBuffer_Playback *cl_find_playback(CL_SoundCard *card, void *handle)
{
	return ((CL_SoundCard_Generic *) card)->manager->get_playback(handle);
}

CL_SoundBuffer_Session::CL_SoundBuffer_Session(void *handle, CL_SoundCard *card)
{
	this->handle = handle;
	this->card = card;
}

CL_SoundBuffer_Session::CL_SoundBuffer_Session(const CL_SoundBuffer_Session &copy)
{
	handle = copy.handle;
	card = copy.card;
	
	((CL_SoundCard_Generic *) card)->manager->add_reference(handle);
}

CL_SoundBuffer_Session::~CL_SoundBuffer_Session()
{
	if (handle != NULL)
		((CL_SoundCard_Generic *) card)->manager->remove_reference(handle);
}

CL_SoundBuffer_Session& CL_SoundBuffer_Session::operator =(const CL_SoundBuffer_Session &session)
{
	if (handle != NULL)
		((CL_SoundCard_Generic *) card)->manager->remove_reference(handle);

	handle = session.handle;
	card = session.card;
	
	((CL_SoundCard_Generic *) card)->manager->add_reference(handle);
	return *this;
}

/*
void CL_SoundBuffer_Session::copy(CL_SoundBuffer_Session session)
{
}
*/
int CL_SoundBuffer_Session::get_position() const 
{
	if (handle == NULL) return 0;
	return cl_find_playback(card, handle)->get_position();
}

float CL_SoundBuffer_Session::get_position_relative() const
{
	if (handle == NULL) return 0.0f;
	return cl_find_playback(card, handle)->get_position_relative();
}

bool CL_SoundBuffer_Session::set_position(int new_pos)
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->set_position(new_pos);
}
	
bool CL_SoundBuffer_Session::set_position_relative(float new_pos)
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->set_position_relative(new_pos);
}

int CL_SoundBuffer_Session::get_length() const
{
	if (handle == NULL) return 0;
	return cl_find_playback(card, handle)->get_length();
}

int CL_SoundBuffer_Session::get_frequency() const
{
	if (handle == NULL) return 0;
	return cl_find_playback(card, handle)->get_frequency();
}
	
bool CL_SoundBuffer_Session::set_frequency(int new_freq)
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->set_frequency(new_freq);
}

float CL_SoundBuffer_Session::get_volume() const
{
	if (handle == NULL) return 0.0f;
	return cl_find_playback(card, handle)->get_volume();
}

bool CL_SoundBuffer_Session::set_volume(float new_volume)
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->set_volume(new_volume);
}

float CL_SoundBuffer_Session::get_pan() const
{
	if (handle == NULL) return 0.0f;
	return cl_find_playback(card, handle)->get_pan();
}

bool CL_SoundBuffer_Session::set_pan(float new_pan)
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->set_pan(new_pan);
}

void CL_SoundBuffer_Session::play()
{
	if (handle == NULL) return;
	cl_find_playback(card, handle)->play();
}

void CL_SoundBuffer_Session::stop()
{
	if (handle == NULL) return;
	cl_find_playback(card, handle)->stop();
}

bool CL_SoundBuffer_Session::is_playing()
{
	if (handle == NULL) return false;
	return cl_find_playback(card, handle)->is_playing();
}

void CL_SoundBuffer_Session::set_looping(bool loop)
{
	if (handle == NULL) return;
	cl_find_playback(card, handle)->set_looping(loop);
}

void CL_SoundBuffer_Session::add_filter(class CL_SoundFilter *filter, bool delete_filter)
{
	if (handle == NULL) return;
	cl_find_playback(card, handle)->add_filter(filter, delete_filter);
}

void CL_SoundBuffer_Session::remove_filter(class CL_SoundFilter *filter)
{
	if (handle == NULL) return;
	cl_find_playback(card, handle)->remove_filter(filter);
}
