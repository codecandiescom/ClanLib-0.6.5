/*
	$Id: displaycard_opengl_win32.cpp,v 1.10 2002/02/16 16:10:42 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <windows.h>
#include "displaycard_opengl_win32.h"
#include "Display/Display/DirectDraw/display_dx.h"
#include "Core/System/Win32/init_win32.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"

extern LONG WINAPI MainMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HGLRC CL_DisplayCard_OpenGL_Win32::context = NULL;
HDC CL_DisplayCard_OpenGL_Win32::hdc = NULL;

void CL_DisplayCard_OpenGL_Win32::add_display()
{
	CL_Display::cards.push_back(
		new CL_DisplayCard_OpenGL_Win32(CL_Display::cards.size()));
}

CL_DisplayCard_OpenGL_Win32::CL_DisplayCard_OpenGL_Win32(int card_no)
: CL_GL_DisplayCard_Generic(card_no)
{
	initialized = false;
}

CL_DisplayCard_OpenGL_Win32::~CL_DisplayCard_OpenGL_Win32()
{
	if (!initialized) return;
	wglDeleteContext(context);
	ReleaseDC(get_hwnd(), hdc); hdc = NULL;
	destroy_window();

	if (is_fullscreen()) ChangeDisplaySettings(NULL, 0);

}

void CL_DisplayCard_OpenGL_Win32::flip_display(bool sync)
{
	if (!initialized) return;

	BOOL retval = SwapBuffers(hdc);
	cl_assert(retval == TRUE);
}

void CL_DisplayCard_OpenGL_Win32::put_display(const class CL_Rect &rect)
{
	cl_assert(false); // hmm - how do you do this under opengl?
}

void CL_DisplayCard_OpenGL_Win32::set_videomode(
	int width,
	int height,
	int bpp,
	bool fullscreen,
	bool allow_resize,
	bool video_memory)
{
	if (fullscreen)
	{
		DEVMODE devmode;
		memset(&devmode, 0, sizeof(DEVMODE));

		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmPelsWidth = width;
		devmode.dmPelsHeight = height;
		devmode.dmBitsPerPel = bpp;

		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		LONG err = ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

		if (err != DISP_CHANGE_SUCCESSFUL)
		{
			switch (err)
			{
			case DISP_CHANGE_RESTART:
				throw CL_Error("This OS sucks! It wants you to restart your computer to change to fullscreen!!!");

			case DISP_CHANGE_FAILED:
				throw CL_Error("The display driver failed the specified graphics mode.");

			case DISP_CHANGE_BADMODE:
				throw CL_Error("The graphics mode is not supported.");

			default:
				{
					CL_String errorstring;
					errorstring << "Could not change videomode, error " << (unsigned short)err;
					throw CL_Error(errorstring.get_string());
				}

			}
		}
	}

	if (!initialized)
	{
		create_window(width, height, bpp, fullscreen, allow_resize);

		hdc = GetDC(get_hwnd());

		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
			1,                              // version number
			// PFD_GENERIC_FORMAT | // force software because my 3d card sux
			PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL |            // support OpenGL
			PFD_DOUBLEBUFFER |              // double buffered
			PFD_DEPTH_DONTCARE,             // do you care?
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			32,                             // 32-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
		};

//		HDC hdc = GetDC(get_hwnd());
		int pixelformat = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pixelformat, &pfd);

		context = wglCreateContext(hdc);
		cl_assert(CL_DisplayCard_OpenGL_Win32::context != NULL);

		BOOL retval = wglMakeCurrent(
			hdc,
			context);

		cl_assert(retval == TRUE);

		if (get_hwnd() == NULL || hdc == NULL)
		{
			cl_assert(false);
		}

		ShowWindow(get_hwnd(), SW_SHOW);
		UpdateWindow(get_hwnd());
	}

	set_gfxmode(width, height, bpp, fullscreen, allow_resize);

	max_texture_size = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

	m_uses_gl = true;

	initialized = true;
}

void (*CL_DisplayCard_OpenGL_Win32::get_proc_address(const std::string& function_name))()
{
	return  (void (*)())wglGetProcAddress(function_name.c_str());
	
}
