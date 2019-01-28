/*
	$Id: setupdisplay_unix.h,v 1.3 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_setupdisplay_unix
#define header_setupdisplay_unix

#include <string>

class CL_Force_DispTarget
// A library should create one single instance of this class to force
// selection of a particular display target.
// Example: libclanGL has a single global instance forcing the "glx" target.
{
public:
	CL_Force_DispTarget(const char *name);

	// I need this for checking whether we link against clanGL or clanCore
	static std::string get_name() { return name; }
	
private:
	static std::string name;

	friend class CL_System_Unix;
};

#endif
