/*
	$Id: cardsoundbuffer_playback.h,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardsoundbuffer_playback
#define header_cardsoundbuffer_playback

#include <utility>
#include <list>

class CL_SoundPlayBackDesc;
class CL_SoundBuffer;
class CL_SoundFilter;
class CL_CardSoundBuffer_Playback
{
public:
	CL_CardSoundBuffer_Playback();
	virtual ~CL_CardSoundBuffer_Playback();
	
	virtual class CL_SoundBuffer_Generic *get_owner()=0;

	virtual int get_position()=0;
	virtual float get_position_relative()=0;

	virtual bool set_position(int new_pos)=0;
	virtual bool set_position_relative(float new_pos)=0;

	virtual int get_length()=0; // -1 if unknown

	virtual int get_frequency()=0;
	virtual bool set_frequency(int new_freq)=0;

	virtual float get_volume()=0;
	virtual bool set_volume(float new_volume)=0;

	virtual float get_pan()=0;
	virtual bool set_pan(float new_pan)=0;

	virtual void play()=0;
	virtual void stop()=0;

	virtual bool is_playing()=0;
	virtual void set_looping(bool loop)=0;
	
	virtual void add_filter(CL_SoundFilter *filter, bool delete_filter);
	virtual void remove_filter(CL_SoundFilter *filter);
	
protected:
	void filter(int *sample_data, int num_samples);

private:
	typedef std::pair<CL_SoundFilter *, bool> FilterPair;
	std::list<FilterPair> filters;
};

#endif
