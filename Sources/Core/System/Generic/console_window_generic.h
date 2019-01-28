/*
	$Id: console_window_generic.h,v 1.9 2002/02/10 17:03:14 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_console_window_generic
#define header_console_window_generic

#include <string>
#include <cstdio>

class CL_ConsoleWindow_Generic
{
//!Construction:
public:
	CL_ConsoleWindow_Generic(
		const std::string &title,
		int width,
		int height);

	~CL_ConsoleWindow_Generic();

//!Operations:
public:
	void redirect_stdio();

	void redirect_stdio(const std::string &file);

	void wait_for_key();
	void display_close_message();

//!Implementation:
private:
	void close_file_handles();

#ifdef _MSC_VER
	FILE *fstdout;
	FILE *fstdin;
	FILE *fstderr;
#else
	std::FILE *fstdout;
	std::FILE *fstdin;
	std::FILE *fstderr;
#endif
};

#endif
