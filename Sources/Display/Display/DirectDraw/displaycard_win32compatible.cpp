/*
	$Id: displaycard_win32compatible.cpp,v 1.7 2002/05/20 13:06:49 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <windows.h>
#include "displaycard_win32compatible.h"
#include "Core/System/Win32/init_win32.h"
#include "API/Core/Math/rect.h"
#include "API/Application.h"

CL_DisplayCard_Win32Compatible::CL_DisplayCard_Win32Compatible(int card_no)
: CL_DisplayCard_Generic(card_no)
{
	hwnd = NULL;
	
	CL_System_Win32::add_listener(this);
}

CL_DisplayCard_Win32Compatible::~CL_DisplayCard_Win32Compatible()
{
	CL_System_Win32::remove_listener(this);

	destroy_window();
}

void CL_DisplayCard_Win32Compatible::destroy_window()
{
	if (hwnd == NULL) return;

	BOOL res = DestroyWindow(hwnd);
	cl_assert(res == TRUE);
	hwnd = NULL;

	UnregisterClass("ClanApplication",CL_System_Win32::hInstance);
}

extern LONG WINAPI MainMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CL_DisplayCard_Win32Compatible::create_window(
	int width,
	int height,
	int bpp,
	bool full_screen,
	bool allow_resize)
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
	wndclass.lpszMenuName = "ClanApplication";
	wndclass.lpszClassName = "ClanApplication";

	RegisterClass(&wndclass);

	int style;
	if (full_screen)
	{
		style = WS_POPUP;
	}
	else if (allow_resize)
	{
		style = WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX /* | WS_MINIMIZEBOX*/;
	}
	else
	{
		style = WS_POPUPWINDOW | WS_CAPTION /*| WS_MINIMIZEBOX*/;
	}

	int x = 0;
	int y = 0;

	if (full_screen)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		CL_Point dimensions( width, height );
		get_window_size( &dimensions, style, false );
		width = dimensions.x;
		height = dimensions.y;
		centralize_window( &dimensions );
		x = dimensions.x;
		y = dimensions.y;
	}

	char *title = CL_ClanApplication::app->get_title();

	hwnd = CreateWindow(
		"ClanApplication",
		title,
		style,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		CL_System_Win32::hInstance,
		NULL);

	cl_assert(hwnd != NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
}

void CL_DisplayCard_Win32Compatible::show_system_cursor()
{
	ShowCursor(TRUE);
}

void CL_DisplayCard_Win32Compatible::hide_system_cursor()
{
	ShowCursor(FALSE);
}

bool CL_DisplayCard_Win32Compatible::received_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		set_resize(LOWORD(lParam), HIWORD(lParam));
		return true;

	case WM_PAINT:
		{
			HDC hdc = (HDC) wParam;
			RECT rect;
			if (GetUpdateRect(hwnd, &rect, FALSE))
			{
				PAINTSTRUCT paint;
				BeginPaint(hwnd, &paint);

				CL_Rect cl_rect;
				cl_rect.x1 = rect.left;
				cl_rect.y1 = rect.top;
				cl_rect.x2 = rect.right;
				cl_rect.y2 = rect.bottom;

				sig_paint()(cl_rect);

				EndPaint(hwnd, &paint);
			}
			else
			{
				CL_Rect cl_rect(0, 0, get_width(), get_height());
				sig_paint()(cl_rect);
			}
		}
		return true;

	default:
		return false;
	}
}

void CL_DisplayCard_Win32Compatible::get_window_size(
	CL_Point *dimensions, 
	DWORD dwStyle, 
	BOOL bMenu )
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = dimensions->x - 1;
	rect.bottom = dimensions->y - 1;
	AdjustWindowRect( &rect, dwStyle, bMenu );
	dimensions->x = rect.right - rect.left + 1;
	dimensions->y = rect.bottom - rect.top + 1;
}

void CL_DisplayCard_Win32Compatible::centralize_window( CL_Point *dimensions )
{
	int src_width = GetSystemMetrics( SM_CXFULLSCREEN );
	int x = ( src_width - dimensions->x ) / 2;
	if ( x < 0 )
		x = 0;
	dimensions->x = x;
	int src_height = GetSystemMetrics( SM_CYFULLSCREEN );
	int y = ( src_height - dimensions->y ) / 2;
	if ( y < 0 )
		y = 0;
	dimensions->y = y;
}
