/*
	$Id: init_win32.cpp,v 1.39 2002/05/02 09:00:22 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		This file is the WinMain entry point. It will setup the clanCore
		win32 environment.

		This file also contain the win32 specific implementations
		of the CL_System class.

		The win32 versions of CL_SetupCore functions are also defined here.
*/

#include "Core/precomp.h" // visual c++: precompiled header file MUST be mentioned FIRST.

#ifdef BORLAND
#include <stdio.h>
#pragma hdrstop
#endif

#include "init_win32.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/setupcore.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"

class CL_Win32Event_Dispatcher : public CL_KeepAlive
{
public:
	virtual void keep_alive();
};

static void calc_commandline(int *argc, char ***argv);
int datafile_main(int argc, char **argv);

// Setup a CL_System::keep_alive() listener that will read win32 events
// and dispatch them.
CL_Win32Event_Dispatcher *event_dispatcher = NULL;

void CL_SetupCore::set_instance(HINSTANCE hInstance)
{
	CL_System_Win32::hInstance = hInstance;
}

static int init_ref_count = 0;
void init_system()
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	event_dispatcher = new CL_Win32Event_Dispatcher;

	// if you get this assertion, you forgot to call CL_SetupCore::set_instance()
	// prior to CL_SetupCore::init().
	cl_assert(CL_System_Win32::hInstance != NULL);

	// Redirect C++ output streams to the output window in developer studio:
//	std::cout = iostream(&debug_buf);
//	cerr = iostream(&debug_buf);
}

void deinit_system()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	delete event_dispatcher;
	event_dispatcher = NULL;

	// And then shutdown core anyways - we assume the developer is an idiot!
//	CL_SetupCore::deinit_display();
//	CL_SetupCore::deinit_sound();
}

void CL_Win32Event_Dispatcher::keep_alive()
{
	// Check for win32 events and dispatch them to MainMessageHandler().

	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CL_System::sig_quit().call();
			break;
		}
	}
}

LONG WINAPI MainMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Route the win32 events through our Win32 event listener list.
	// If no listener handles the event, pass it on to the default window procedure.

	bool handled = false;

	for (
		std::list<CL_Win32EventListener *>::iterator it = CL_System_Win32::listeners.begin();
		it != CL_System_Win32::listeners.end();
		it++)
	{
		if ((*it)->received_event(uMsg, wParam, lParam)) handled = true;
	}

	if (handled) return TRUE;

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;

			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_SYSCOMMAND:
		{
			// Do not allow screensaver to start.
			if ( wParam != SC_SCREENSAVE )
			{
				return DefWindowProc( hWnd, uMsg, wParam, lParam );
			}
		}
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}

void CL_System_Win32::add_listener(CL_Win32EventListener *listener)
{
	listeners.push_back(listener);
}

void CL_System_Win32::remove_listener(CL_Win32EventListener *listener)
{
	listeners.remove(listener);
}

// Win32 implementation of CL_System functions:

unsigned int CL_System::get_time()
{
	static LARGE_INTEGER perf_frequency, perf_counter;
	static bool first_time = true;

	if (first_time)
	{
		QueryPerformanceFrequency(&perf_frequency);
		perf_frequency.QuadPart /= 1000;
		first_time = false;
	}

	QueryPerformanceCounter(&perf_counter);
	return (unsigned int) perf_counter.QuadPart / perf_frequency.QuadPart;
}

void CL_System::sleep(int millis)
{
	Sleep(millis);
}

// Global vars:

HINSTANCE CL_System_Win32::hInstance = NULL;
std::list<CL_Win32EventListener*> CL_System_Win32::listeners;
