/*
	$Id: directsound.cpp,v 1.6 2001/09/08 19:24:21 japj Exp $
	
	ClanSound, sound mixing library written for ClanLib.
	Copyright (c)1998 Magnus Norddahl / ClanSoft.
*/

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include "directsound.h"
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <Core/System/Win32/init_win32.h>
#include <iostream>

extern LONG WINAPI MainMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool has_sound = true;

CL_CSOutput::CL_CSOutput()
: directsound(NULL), soundbuffer(NULL), sleep_event(NULL)
{
	HRESULT err;
	err = DirectSoundCreate(NULL, &directsound, NULL);
	
	if (err != DS_OK)
	{
		// throw CL_Error("Cannot open sound device.");
		frag_size = 22050/2;
		has_sound = false;
		return;
	}

	// We need a hwnd for our directsound interface:
	HWND hwnd = NULL;
//	if (CL_Display::cards.size() > 0) hwnd = ((CL_DisplayCard_Win32Compatible *) CL_Display::cards[0])->get_hwnd();
	if (hwnd == NULL)
	{
		WNDCLASS wndclass;

		wndclass.style = 0;
		wndclass.lpfnWndProc = (WNDPROC) MainMessageHandler;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = CL_System_Win32::hInstance;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wndclass.lpszMenuName = "ClanSound";
		wndclass.lpszClassName = "ClanSound";

		RegisterClass(&wndclass);

		hwnd = CreateWindow(
			"ClanSound",
			"ClanSound",
			0, // WS_POPUP,
			0,
			0,
			1,
			1,
			NULL,
			NULL,
			CL_System_Win32::hInstance,
			NULL);
	}
	cl_assert(hwnd != NULL);

	err = directsound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
	cl_assert(err == DS_OK);


	// Create mixing buffer.
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
	int num_fragments = 8;

	DSBUFFERDESC desc;
	desc.dwSize = sizeof(DSBUFFERDESC); 
	desc.dwFlags = /*DSBCAPS_PRIMARYBUFFER |*/ DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS;

	desc.dwBufferBytes = frag_size * bytes_per_sample * num_fragments;
	desc.dwReserved = 0;
	desc.lpwfxFormat = &format; 

	err = directsound->CreateSoundBuffer(&desc, &soundbuffer, NULL);
	if (err != DS_OK) throw CL_Error("Cannot get primary sound buffer.");

	// Find size of buffer:

	DSBCAPS caps;
	memset(&caps, 0, sizeof(DSBCAPS));
	caps.dwSize = sizeof(DSBCAPS);
	
	err = soundbuffer->GetCaps(&caps);
	cl_assert(err == DS_OK);

	buffer_size = caps.dwBufferBytes;
	cl_assert(buffer_size == frag_size * bytes_per_sample * num_fragments);

	// Clear buffer at beginning (good for debugging):
	DWORD size1, size2;
	void *ptr1;
	void *ptr2;
	err = soundbuffer->Lock(0, buffer_size, &ptr1, &size1, &ptr2, &size2, 0);
	cl_assert(err == DS_OK);

	memset(ptr1, 0, size1);
	if (ptr2 != NULL) memset(ptr2, 0, size2);

	soundbuffer->Unlock(ptr1, size1, ptr2, size2);

	// Setup some sleeping :)
	sleep_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	err = soundbuffer->QueryInterface(IID_IDirectSoundNotify, (void **) &notify);
//	cl_assert(err == DS_OK);
	if (err != DS_OK) // directsoundnotify doesnt exist on nt4 with dx3. No sound avail.
	{
		// throw CL_Error("Cannot open sound device.");
		frag_size = 22050/2;
		has_sound = false;
		return;
	}

	DSBPOSITIONNOTIFY *notify_pos = new DSBPOSITIONNOTIFY[num_fragments];
	for (int i=0; i<num_fragments; i++)
	{
		notify_pos[i].dwOffset = i*frag_size*bytes_per_sample;
		notify_pos[i].hEventNotify = sleep_event;
	}
	notify->SetNotificationPositions(num_fragments, notify_pos);
	delete[] notify_pos;

	err = soundbuffer->Play(0, 0, DSBPLAY_LOOPING);
	cl_assert(err == DS_OK);
}

CL_CSOutput::~CL_CSOutput()
{
	if (soundbuffer != NULL) soundbuffer->Release();
	if (directsound != NULL) directsound->Release();
	if (sleep_event != NULL) CloseHandle(sleep_event);
}

void CL_CSOutput::silence()
{
}

bool CL_CSOutput::is_full()
{
	if (!has_sound) return false;
	
	cl_info(info_sound, "CL_CSOutput::is_full() is not implemented! Program will assert now.");
	cl_assert(false);
	return false;
}

void CL_CSOutput::write_fragment(short *data)
{
	if (!has_sound) return;
	
	HRESULT err;

	DWORD play, write;
	err = soundbuffer->GetCurrentPosition(&play, &write);
	cl_assert(err == DS_OK);

	int frag_bytes = frag_size*bytes_per_sample;

	int pos = (write / frag_bytes + 1) * frag_bytes;
	if (pos >= buffer_size) pos -= buffer_size;

	DWORD size1, size2;
	void *ptr1 = NULL, *ptr2 = NULL;
	err = soundbuffer->Lock(
		pos,
		frag_bytes,
		(void **) &ptr1,
		&size1,
		(void **) &ptr2,
		&size2,
		0);
	cl_assert(err == DS_OK);
/*
	std::cout
		<< "#" << pos / frag_bytes
		<< " play ptr: " << play
		<< ", write ptr: " << write
		<< ", pos: " << pos
		<< ", size1: " << size1
		<< ", size2: " << size2
		<< std::endl;
*/
	char *_data = (char *) data;
	if (ptr1 != NULL) memcpy(ptr1, _data, size1);
	if (ptr2 != NULL) memcpy(ptr2, _data+size1, size2);

	err = soundbuffer->Unlock(ptr1, size1, ptr2, size2);
	cl_assert(err == DS_OK);
}

void CL_CSOutput::wait()
{
	if(!has_sound)
	{
		CL_System::sleep(100);
		return;
	}
	
	WaitForSingleObject(sleep_event, INFINITE);
	ResetEvent(sleep_event);
}
