/*
	$Id: resourcetype_raw.h,v 1.13 2001/12/15 02:06:50 mbn Exp $

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

#ifndef header_resourcetype_raw
#define header_resourcetype_raw

#include "resourcedata.h"
#include "resource_manager.h"
#include "resource.h"
#include "../IOData/inputsource_memory.h"
#include <string>
#include <vector>

//: Resource Data Raw Class
class CL_ResourceData_Raw : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Raw Constructor
	CL_ResourceData_Raw(class CL_Resource &resource);

//! Attributes:
public:
	//: value (dox to be written)
	std::vector<unsigned char> value;

//! Implementation:
private:
	void on_load_file();
	void on_load_datafile(class CL_InputSourceProvider *provider);
	void on_save_datafile(class CL_OutputSourceProvider *provider);
	void on_unload();

	CL_Slot slot_load_file;
	CL_Slot slot_load_datafile;
	CL_Slot slot_save_datafile;
	CL_Slot slot_unload;
};

//: Raw Class
class CL_Raw
{
//! Construction:
public:
	//: Raw Class Constructor
	CL_Raw(const std::string &res_id, CL_ResourceManager *resources)
	{
		res = resources->get_resource(res_id);
		res.load();
		data = (CL_ResourceData_Raw *) res.get_data("raw");
	}
	
	//: Raw Class Destructor
	~CL_Raw()
	{
		res.unload();
	}

	//: Get Data
	std::vector<unsigned char> &get_data()
	{
		return data->value;
	}

	//: Get Data
	const std::vector<unsigned char> &get_data() const
	{
		return data->value;
	}

private:
	CL_Resource res;
	CL_ResourceData_Raw *data;
};

//: Input source interface for 'raw' resource types.
class CL_InputSource_Raw : public CL_Raw, public CL_InputSource_Memory
{
public:
	//: Construct input source reading from a 'raw' resource type.
	CL_InputSource_Raw(const std::string &res_id, CL_ResourceManager *resources)
		: CL_Raw(res_id, resources), CL_InputSource_Memory(std::string((char *) &CL_Raw::get_data()[0], CL_Raw::get_data().size()))
	{
	}
};

#endif
