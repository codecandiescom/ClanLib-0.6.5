/*
	$Id: resourcetype.h,v 1.12 2001/09/22 15:52:11 plasmoid Exp $

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

#ifndef header_resourcetype
#define header_resourcetype

#include <string>
#include <list>

class CL_Resource;

//: The Resource Type Class
class CL_ResourceType
{
//! Construction:
public:
	//: Constructs an registers a resource type.
	CL_ResourceType(const std::string &type);

	//: Destructs and unregisters a resource type.
	virtual ~CL_ResourceType();

//! Attributes:
public:
	//: Returns the resource type name.
	const std::string &get_type() const { return type; }

	//: Global static list of all registered resource types.
	static std::list<CL_ResourceType*> resource_types;

//! Operations:
public:
	//: Connect Data
	//: <p>The resource manager calls this function just after a resource object
	//: has been created. The resource type should attach its resource data
	//: to the resource in this function.</p>
	virtual void connect_data(CL_Resource &resource)=0;

//! Implementation:
private:
	std::string type;
};

template<class Resource>
//: The Register ResourseType Class
//- <p>The CL_RegisterResourceType template class is a quick and convient way
//- to register a resource type without doing a full inheritance of
//- CL_ResourceType.</p>
class CL_RegisterResourceType : public CL_ResourceType
{
public:
	//: Register Resource Type Contructor
	CL_RegisterResourceType(const std::string &type) : CL_ResourceType(type)
	{
	}

	//: Connect Data
	virtual void connect_data(CL_Resource &resource)
	{
		new Resource(resource); // attach resource data.
	}
};

#endif
