/*
	$Id: resourcetype_integer.h,v 1.13 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_resourcetype_integer
#define header_resourcetype_integer

#include "resourcedata.h"
#include "resource_manager.h"
#include "resource.h"
#include <string>
#include "../System/error.h"

//: Resource Data Integer Class
class CL_ResourceData_Integer : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Interger Constructor
	CL_ResourceData_Integer(class CL_Resource &resource);

//! Attributes:
public:
	//: Integer Value
	int value;
};

//: Integer Class
class CL_Integer
{
//! Construction:
public:
	//: Integer Constructor
	CL_Integer(const std::string &res_id, CL_ResourceManager *resources)
	{
		CL_Resource res = resources->get_resource(res_id);
		res.load();
		CL_ResourceData_Integer *data = (CL_ResourceData_Integer *) res.get_data("integer");
		value = data->value;
		res.unload();
	}

	//: Integer Constructor
	CL_Integer(const std::string &res_id, CL_ResourceManager *resources, int def_value)
	{
		try
		{
			CL_Resource res = resources->get_resource(res_id);
			res.load();
			CL_ResourceData_Integer *data = (CL_ResourceData_Integer *) res.get_data("integer");
			value = data->value;
			res.unload();
		}
		catch (CL_Error err)
		{
			value = def_value;
		}
	}

//! Operators:
	//: Integer operator
	operator int() const
	{
		return value;
	}

private:
	int value;
};

#endif
