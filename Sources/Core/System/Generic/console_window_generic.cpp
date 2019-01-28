/*
	$Id: console_window_generic.cpp,v 1.18 2002/03/18 06:31:56 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/cl_assert.h"
#include "console_window_generic.h"

#include <iostream>
#include <cstdio>

using namespace std;

#ifdef WIN32
#include <stdio.h>
static HANDLE scrbuf = NULL;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Construction:

CL_ConsoleWindow_Generic::CL_ConsoleWindow_Generic(
	const std::string &title,
	int width,
	int height)
{
#ifdef WIN32
	AllocConsole();
	SetConsoleTitle(title.c_str());
	COORD coord;
	coord.X = width;
	coord.Y = height;
	scrbuf =
		CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL);

	cl_assert(scrbuf != INVALID_HANDLE_VALUE);

	SetConsoleActiveScreenBuffer(scrbuf);
	SetConsoleScreenBufferSize(scrbuf, coord);

	fstdout = 0;
	fstdin = 0;
	fstderr = 0;
#endif
}

CL_ConsoleWindow_Generic::~CL_ConsoleWindow_Generic()
{
#ifdef WIN32
	close_file_handles();
	CloseHandle(scrbuf);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Operations:

void CL_ConsoleWindow_Generic::redirect_stdio()
{
#ifdef WIN32
	close_file_handles();
	fstdin = freopen("CONIN$","rt",stdin);
	fstderr = freopen("CONOUT$", "wt", stderr);
	fstdout = freopen("CONOUT$", "wt", stdout);
#endif
}

void CL_ConsoleWindow_Generic::redirect_stdio(const std::string &file)
{
	close_file_handles();
	fstdout = freopen(file.c_str(),"wt",stdout);
	fstderr = freopen(file.c_str(),"wt",stderr);
}

void CL_ConsoleWindow_Generic::wait_for_key()
{
#ifdef WIN32
	// If your application crashes here, you are linking with a single threaded
	// libc in your application! -- mbn 13. Jan 2001.
  #ifdef __BORLANDC__
	std::string foo;
	cin >> foo;
  #else
	while (!kbhit()) Sleep(250);
  #endif
#endif
}

void CL_ConsoleWindow_Generic::display_close_message()
{
#ifdef WIN32
	std::cout << std::endl << "(press any key to close this console window)";
	std::cout.flush();

	wait_for_key();
#endif
}

void CL_ConsoleWindow_Generic::close_file_handles()
{
#ifdef WIN32
	if (fstdin)
	{
		fclose(fstdin); fstdin = 0;
	}
	if (fstderr)
	{
		fclose(fstderr); fstderr = 0;
	}
	if (fstdout)
	{
		fclose(fstdout); fstdout = 0;
	}
#endif
}
