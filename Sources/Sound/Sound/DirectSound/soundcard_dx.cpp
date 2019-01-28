/*
	$Id: soundcard_dx.cpp,v 1.2 2001/09/08 19:24:21 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <iostream>
#include "API/Core/Display/display.h"
#include "API/Core/System/clanapp.h"
#include "soundbuffer_static_dx.h"
#include "soundbuffer_stream_dx.h"
#include "Core/Sound/Generic/cardsession_manager.h"
#include "soundcard_dx.h"
#include "Core/System/Win32/init_win32.h"
#include "Core/Display/DirectDraw/displaycard_win32compatible.h"

/**************************************************************************
                       CL_SoundCard_DX implementation
**************************************************************************/

CL_SoundCard_DX::CL_SoundCard_DX(int _card_no, std::string _name, LPGUID _guid)
{
	card_no = _card_no;
	name = _name;
	is_initialized = false;
	guid = _guid;
	soundcard_enabled = false;
	soundcard = NULL;
}

CL_SoundCard_DX::~CL_SoundCard_DX()
{
	manager->remove_all_playbacks();
	if (soundcard != NULL)
	{
		soundcard->Release();
	}
}

extern LONG WINAPI MainMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CL_SoundCard_DX::init_directsound()
{
	HRESULT err;
	err = DirectSoundCreate(guid, &soundcard, NULL);
	if (err != DS_OK) { std::cout << "Failed to create " << name.c_str() << std::endl; soundcard = NULL; }
	else
	{
		HWND hwnd = NULL;
		if (CL_Display::cards.size() > 0)
		{
			hwnd = ((CL_DisplayCard_Win32Compatible *) CL_Display::cards[0])->get_hwnd();
			if (hwnd == NULL)
			{
				((CL_DisplayCard_Win32Compatible *) CL_Display::cards[0])->create_window(640, 480, 16, false, true);
				hwnd = ((CL_DisplayCard_Win32Compatible *) CL_Display::cards[0])->get_hwnd();
			}
		}
		else
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
			wndclass.lpszMenuName = CL_ClanApplication::app->get_title();
			wndclass.lpszClassName = CL_ClanApplication::app->get_title();

			if (RegisterClass(&wndclass) != 0)
			{
				hwnd = CreateWindow(
					CL_ClanApplication::app->get_title(),
					CL_ClanApplication::app->get_title(),
					WS_POPUP /* no style */,
					0,
					0,
					1,
					1,
					NULL,
					NULL,
					CL_System_Win32::hInstance,
					NULL);
			}
		}
		if (hwnd == NULL)
		{
			soundcard->Release();
			soundcard = NULL;
			return;
		}

		err = soundcard->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
		if (err != DS_OK) 
		{
			std::cout << "Failed to create " << name.c_str() << std::endl;
			soundcard->Release();
			soundcard = NULL;
		}
		else soundcard_enabled = true;
	}

	is_initialized = true;
}

CL_CardSoundBuffer_Static *CL_SoundCard_DX::create_soundbuffer_static(
	CL_SoundBuffer_Generic *owner,
	CL_StaticSoundProvider *provider)
{
 	if (!is_initialized) init_directsound();

	return new CL_CardSoundBufferStatic_DX(this, owner, provider);
}

CL_CardSoundBuffer_Playback *CL_SoundCard_DX::create_cardsoundbuffer_playback_streamed(
	CL_SoundBuffer_Generic_Stream *soundbuffer)
{
	if (!is_initialized) init_directsound();

	return new CL_CardSoundBufferPlaybackStream_DX(this, soundbuffer);
}
