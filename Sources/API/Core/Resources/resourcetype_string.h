/*
	$Id: resourcetype_string.h,v 1.10 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="Resources"
//! header=core.h

#ifndef header_resourcetype_string
#define header_resourcetype_string

#include "resourcedata.h"
#include <string>

//: Resource Data String Class
class CL_ResourceData_String : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data String Constructor
	CL_ResourceData_String(class CL_Resource &resource);

//! Attributes:
public:
	//: The string
	std::string value;
};

#endif
