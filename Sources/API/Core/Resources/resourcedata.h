/*
	$Id: resourcedata.h,v 1.8 2001/10/10 11:50:27 sphair Exp $

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

#ifndef header_resourcedata
#define header_resourcedata

#include "resource.h"

class CL_ResourceData_Generic;

//: Resource Data Class
class CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Constructor
	CL_ResourceData(CL_Resource &resource);

	//: Resource Data Destructor
	virtual ~CL_ResourceData();

//! Attributes:
public:
	//: Get Resource
	CL_Resource get_resource();

//! Implementation:
private:
	//: Pointer to the implementation
	CL_ResourceData_Generic *impl;
};

#endif
