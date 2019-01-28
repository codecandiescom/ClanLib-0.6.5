/*
	$Id: directsound.h,v 1.3 2001/09/08 19:24:21 japj Exp $
	
	ClanSound, sound mixing library written for ClanLib.
	Copyright (c)1998 Magnus Norddahl / ClanSoft.
*/

#ifndef header_directsound
#define header_directsound

#include <dsound.h>

class CL_CSOutput
{
public:
	CL_CSOutput();
	~CL_CSOutput();

	void silence();
	// Called when we have no samples to play - and wants to tell the soundcard
	// about this possible event.

	bool is_full();
	// Returns true if all fragments are filled with data.

	int get_frag_size() const { return frag_size; }
	// Returns the buffer size used by device (returned as num [stereo] samples).

	void write_fragment(short *data);
	// Writes a fragment to the soundcard.

	void wait();
	// Waits until output source isn't full anymore.

private:
	LPDIRECTSOUND directsound;
	LPDIRECTSOUNDBUFFER soundbuffer;
	int frag_size, buffer_size, bytes_per_sample;
	HANDLE sleep_event;
	LPDIRECTSOUNDNOTIFY notify;
};

#endif
