/*
	$Id: cl_midi.h,v 1.6 2001/09/22 15:52:16 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanMIDI="System"
//! header=midi.h

#ifndef cl_midiH
#define cl_midiH
#endif

#include <midi.h>

//: Midi Class
class CL_MIDI
{
public:
//! Construction:
	//: Midi Constructor
	CL_MIDI() { return;};

	//: Midi Destructor
	virtual ~CL_MIDI() { return; }

//! Operations:
	//: Start Playing
	virtual bool start_playing()=0;

	//: Stop Playing
	virtual bool stop_playing()=0;

	//: Load File
	virtual bool load_file(wchar_t* filename)=0;
};

