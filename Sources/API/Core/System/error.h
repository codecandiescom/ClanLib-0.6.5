/*
	$Id: error.h,v 1.10 2001/12/11 22:16:06 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_error
#define header_error

#include <string>

//: Exception class in ClanLib.
//- <p>If something goes wrong in a clanlib function, it will throw an exception of
//- this class.</p>
class CL_Error
{
public:
//! Construction:
	//: Error Constructor
	CL_Error(const char *message) { this->message = message; }

	//: Error Constructor
	CL_Error(const std::string &message) { this->message = message; }

	//: The error message
	std::string message;
};

void cl_throw_error(const char *message);
void cl_throw_error(const std::string &message);

#endif
