/*
	$Id: resourcetype_boolean.h,v 1.13 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_resourcetype_boolean
#define header_resourcetype_boolean

#include "resourcedata.h"
#include "resource_manager.h"
#include "resource.h"
#include <string>
#include "../System/error.h"

//: Resource Data Boolean Class
class CL_ResourceData_Boolean : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Boolean Constructor
	CL_ResourceData_Boolean(class CL_Resource &resource);

//! Attributes:
public:
	//: Value
	bool value;
};

//: Boolean Class
class CL_Boolean
{
//! Construction:
public:
	//: Boolean Constructor
	CL_Boolean(const std::string &res_id, CL_ResourceManager *resources)
	{
		CL_Resource res = resources->get_resource(res_id);
		res.load();
		CL_ResourceData_Boolean *data = (CL_ResourceData_Boolean *) res.get_data("boolean");
		value = data->value;
		res.unload();
	}

	//: Boolean Constructor
	CL_Boolean(const std::string &res_id, CL_ResourceManager *resources, bool def_value)
	{
		try
		{
			CL_Resource res = resources->get_resource(res_id);
			res.load();
			CL_ResourceData_Boolean *data = (CL_ResourceData_Boolean *) res.get_data("boolean");
			value = data->value;
			res.unload();
		}
		catch (CL_Error err)
		{
			value = def_value;
		}
	}

//! Operators:
	//: Operator bool
	operator bool() const
	{
		return value;
	}

private:
	bool value;
};

#endif
