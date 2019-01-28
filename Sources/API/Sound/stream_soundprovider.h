/*
	$Id: stream_soundprovider.h,v 1.5 2001/09/22 15:52:18 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_stream_soundprovider
#define header_stream_soundprovider

#include "soundformat.h"

class CL_StreamSoundProvider_Session;
//: Streamed soundprovider.
//- <p>Interface to streamed sound sources. Used by the CL_SoundBuffer class to
//- start and stop playback.</p>
class CL_StreamSoundProvider
{
public:
//! Construction:
	//: Stream Sound Provider Destructor
	virtual ~CL_StreamSoundProvider() { ; }

//! Operations:
	//: Called by CL_SoundBuffer when a new session starts.
	//- Returns - The soundbuffer session to be attached to the newly
	//- Returns - started session.
	virtual CL_StreamSoundProvider_Session *begin_session()=0;

	//: Called by CL_SoundBuffer when a session has finished. After this call,
	//: <p>CL_SoundBuffer will not access the session anymore. It can safely be deleted
	//: here (and in most cases should be delete here).</p>
	virtual void end_session(CL_StreamSoundProvider_Session *session)=0;
};

//: Streamed soundprovider playback session
//- Interface used by ClanLib during playback of streamed soundbuffers.
class CL_StreamSoundProvider_Session
{
public:
//! Construction:
	//: Stream Sound Provider Session Destructor
	virtual ~CL_StreamSoundProvider_Session() { ; }

//! Operations:
	//: Returns true if no more input data is available.
	//- Returns - True if end of input data. False otherwise.
	virtual bool eof() const =0;
	
	//: Stops the current stream.
	virtual void stop()=0;

	//: Start/continue playing of the stream.
	//- Returns - True for success. False otherwise.
	virtual bool play()=0;
		
	//: Sets the position within the current stream.
	//- pos - Position to seek to.
	//- Returns - True for success. False otherwise.
	virtual bool set_position(int pos)=0;
	
//! Attributes:
	//: Called when a playback session needs more sample data.
	//- data_ptr - Points to a buffer that should be filled with
	//-            sample data.
	//- data_requested - Bytes of data requested.
	//- Returns - Number of bytes actually filled with sample data.
	virtual int get_data(void *data_ptr, int data_requested)=0;

	//: Returns the playback frequency of the input data.
	//- Returns - Playback frequency.
	virtual int get_frequency() const =0;

	//: Returns the playback sample format.
	//- Returns - The playback Sample format.
	virtual SoundFormat get_format() const =0;
};

#endif
