/*
	$Id: directsound_recorder.h,v 1.2 2002/02/16 17:03:49 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_directsoundrecorder
#define header_directsoundrecorder

#include "API/Sound/stream_soundprovider.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include <dsound.h>

class CL_DirectSoundRecorder : public CL_StreamSoundProvider
{
//!Enums:
public:
	enum Type
	{
		type_mic,
		type_line_in
	};

//!Construction:
public:
	//: Construct sound recording provider.
	CL_DirectSoundRecorder(Type type);

	//: Stream Sound Provider Destructor
	virtual ~CL_DirectSoundRecorder();

//!Operations:
public:
	virtual CL_StreamSoundProvider_Session *begin_session();

	virtual void end_session(CL_StreamSoundProvider_Session *session);

//!Implementation:
private:
	Type type;
};

class CL_DirectSoundRecorder_Session : public CL_StreamSoundProvider_Session, CL_Runnable
{
//! Construction:
public:
	CL_DirectSoundRecorder_Session(CL_DirectSoundRecorder::Type type);

	virtual ~CL_DirectSoundRecorder_Session();

//!Operations:
public:
	virtual bool eof() const;

	virtual void stop();

	virtual bool play();

	virtual bool set_position(int pos);

//!Attributes:
public:
	virtual int get_data(void *data_ptr, int data_requested);

	virtual int get_frequency() const;

	virtual SoundFormat get_format() const;

//!Implementation:
private:
	virtual void run();

	CL_DirectSoundRecorder::Type type;

	LPDIRECTSOUNDCAPTURE8 dsoundcapture;

	LPDIRECTSOUNDCAPTUREBUFFER capturebuffer;

	LPDIRECTSOUNDNOTIFY8 notify;

	HANDLE notify_event;

	int bytes_per_sample;

	int frag_size, num_fragments;

	char *buffer;

	int buffer_size;

	CL_Mutex mutex;

	volatile bool stop_thread;

	CL_Thread thread;
};

#endif
