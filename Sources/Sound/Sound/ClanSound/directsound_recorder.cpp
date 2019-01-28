/*
	$Id: directsound_recorder.cpp,v 1.3 2002/02/17 00:45:23 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#include "directsound_recorder.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder construction:

CL_DirectSoundRecorder::CL_DirectSoundRecorder(Type type)
: type(type)
{
}

CL_DirectSoundRecorder::~CL_DirectSoundRecorder()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder operations:

CL_StreamSoundProvider_Session *CL_DirectSoundRecorder::begin_session()
{
	return new CL_DirectSoundRecorder_Session(type);
}

void CL_DirectSoundRecorder::end_session(CL_StreamSoundProvider_Session *session)
{
	delete session;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder implementation:


/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session construction:

CL_DirectSoundRecorder_Session::CL_DirectSoundRecorder_Session(CL_DirectSoundRecorder::Type type)
: type(type), dsoundcapture(0), buffer(0), buffer_size(0)
{
	HRESULT result;
	
	// Create sound capture:
	result = DirectSoundCaptureCreate8(0, &dsoundcapture, 0);
	if (result != DS_OK) throw CL_Error("Failed to create DirectSound capture!");

	DSCCAPS caps;
	memset(&caps, 0, sizeof(DSCCAPS));
	caps.dwSize = sizeof(DSCCAPS);
	result = dsoundcapture->GetCaps(&caps);
	if (result != DS_OK)
	{
		dsoundcapture->Release();
		throw CL_Error("Failed to query for DirectSound capture capabilities!");
	}

	// Setup sound format and create capture buffer:
	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM; 
	format.nChannels = 2;
	format.nSamplesPerSec = 22050;
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8; 
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign; 
	format.cbSize = 0;

	bytes_per_sample = format.nBlockAlign;
	frag_size = 2500; // 0.12 sec latency
	num_fragments = 8;

	DSCBUFFERDESC bufferdesc;
	memset(&bufferdesc, 0, sizeof(DSCBUFFERDESC));
	bufferdesc.dwSize = sizeof(DSCBUFFERDESC);
	bufferdesc.dwBufferBytes = bytes_per_sample * frag_size * num_fragments;
	bufferdesc.lpwfxFormat = &format;

	result = dsoundcapture->CreateCaptureBuffer(&bufferdesc, &capturebuffer, 0);
	if (result != DS_OK)
	{
		dsoundcapture->Release();
		throw CL_Error("Failed to create capture buffer!");
	}

	// Setup notify events for each record fragment:
	result = capturebuffer->QueryInterface(IID_IDirectSoundNotify8, (void **) &notify);
	if (result != DS_OK)
	{
		capturebuffer->Release();
		dsoundcapture->Release();
		throw CL_Error("Failed to create notify events for capture buffer!");
	}

	notify_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	DSBPOSITIONNOTIFY *notify_pos = new DSBPOSITIONNOTIFY[num_fragments];
	for (int i=0; i<num_fragments; i++)
	{
		notify_pos[i].dwOffset = i*frag_size*bytes_per_sample;
		notify_pos[i].hEventNotify = notify_event;
	}
	result = notify->SetNotificationPositions(num_fragments, notify_pos);
	if (result != DS_OK)
	{
		delete[] notify_pos;
		notify->Release();
		capturebuffer->Release();
		dsoundcapture->Release();
		throw CL_Error("Failed to create notification positions for capture buffer!");
	}
	delete[] notify_pos;

	// Start worker thread:
	stop_thread = false;
	thread = CL_Thread(this);
	thread.start();
}

CL_DirectSoundRecorder_Session::~CL_DirectSoundRecorder_Session()
{
	// Shut down worker thread.
	stop_thread = true;
	SetEvent(notify_event);
	thread.wait();

	// Clean up.
	delete[] buffer;
	notify->Release();
	capturebuffer->Release();
	dsoundcapture->Release();
	CloseHandle(notify_event);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session operations:

bool CL_DirectSoundRecorder_Session::eof() const
{
	return false;
}

void CL_DirectSoundRecorder_Session::stop()
{
	capturebuffer->Stop();
}

bool CL_DirectSoundRecorder_Session::play()
{
	HRESULT result = capturebuffer->Start(DSCBSTART_LOOPING);
	return (result == DS_OK);
}

bool CL_DirectSoundRecorder_Session::set_position(int pos)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session attributes:

#define cl_min(a,b) ((a < b) ? a : b)

int CL_DirectSoundRecorder_Session::get_data(void *data_ptr, int data_requested)
{
	CL_MutexSection mutex_section(&mutex);
	int send_size = cl_min(data_requested, buffer_size);
	if (send_size > 0)
	{
		memcpy(data_ptr, buffer, send_size);
		buffer_size -= send_size;
		if (buffer_size == 0)
		{
			delete[] buffer;
			buffer = 0;
		}
		else
		{
			char *old_buffer = buffer;
			buffer = new char[buffer_size];
			memcpy(buffer, old_buffer+send_size, buffer_size);
			delete[] old_buffer;
		}
	}
	return send_size;
}

int CL_DirectSoundRecorder_Session::get_frequency() const
{
	return 22050;
}

SoundFormat CL_DirectSoundRecorder_Session::get_format() const
{
	return sf_16bit_signed_stereo;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectSoundRecorder_Session implementation:

void CL_DirectSoundRecorder_Session::run()
{
	HRESULT result;
	DWORD cur_pos = 0;
	DWORD capturebuffer_size = frag_size * num_fragments * 4;

	while (true)
	{
		// Wait for incoming data or quit request:
		WaitForSingleObject(notify_event, INFINITE);
		ResetEvent(notify_event);
		if (stop_thread) break;

		CL_MutexSection mutex_section(&mutex);

		// Figure out what to copy:
		DWORD capture_pos, read_pos;
		result = capturebuffer->GetCurrentPosition(&capture_pos, &read_pos);
		cl_assert(result == DS_OK);

		DWORD avail_bytes = read_pos - cur_pos;
		if (read_pos <= cur_pos) avail_bytes = capturebuffer_size - cur_pos + read_pos;

		LPVOID buf1 = 0;
		LPVOID buf2 = 0;
		DWORD size1 = 0;
		DWORD size2 = 0;

		result = capturebuffer->Lock(
			cur_pos, avail_bytes, &buf1, &size1, &buf2, &size2, 0);
		cl_assert(result == DS_OK);

		// Append new data to buffer:
		char *old_buffer = buffer;
		buffer = new char[buffer_size + avail_bytes];
		if (old_buffer) memcpy(buffer, old_buffer, buffer_size);
		delete[] old_buffer;
		memcpy(buffer+buffer_size, buf1, size1);
		if (buf2) memcpy(buffer+buffer_size+size1, buf2, size2);
		buffer_size += avail_bytes;

		result = capturebuffer->Unlock(buf1, size1, buf2, size2);
		cl_assert(result == DS_OK);

		cur_pos += avail_bytes;
		if (cur_pos >= capturebuffer_size) cur_pos -= capturebuffer_size;
	}
}
