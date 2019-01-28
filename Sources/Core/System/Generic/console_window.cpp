/*
	$Id: console_window.cpp,v 1.4 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/console_window.h"
#include "console_window_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow construction:

CL_ConsoleWindow::CL_ConsoleWindow(
	const std::string &title,
	int width,
	int height)
: impl(NULL)
{
	impl = new CL_ConsoleWindow_Generic(title, width, height);
}

CL_ConsoleWindow::~CL_ConsoleWindow()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow operations:

void CL_ConsoleWindow::redirect_stdio()
{
	impl->redirect_stdio();
}

void CL_ConsoleWindow::redirect_stdio(const std::string &filename)
{
	impl->redirect_stdio(filename);
}

void CL_ConsoleWindow::wait_for_key()
{
	impl->wait_for_key();
}

void CL_ConsoleWindow::display_close_message()
{
	impl->display_close_message();
}
