/*
	$Id: resource_manager.h,v 1.16 2001/12/15 01:30:07 mbn Exp $

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

#ifndef header_resource_manager
#define header_resource_manager

#include <list>
#include <string>

class CL_Resource;
class CL_InputSourceProvider;
class CL_ResourceManager_File;

class CL_ResourceManager
//: The ClanLib resource manager.
//- The resource manager is used to retrieve resources from a given resource source.
//- <p>This can either be a resource script file (used as input to the
//- datafile compiler), or a datafile with all the resources included into
//- one large gzipped resource file.</p>
//-
//- <p>To speedup loading of resources in a game, you can load entire sections
//- of resources at once. When a resource in the section is requested, it is
//- returned instantly without having to access the disk. This is especially
//- useful to make sure all the game resources are loaded before the game is
//- started.</p>
//-
//- <p>Resources are normally not retrieved using the get_resource() function.
//- Instead, you should load the resource using the appropiate resource type
//- class. For instance, a surface is easiest loaded like this:</p>
//-
//- CL_ResourceManager res_manager("my_datafile.dat", true);
//- CL_Surface my_surface("my_surface", res_manager);
//-
//- <p>Getting the same resource twice won't create a new instance of the
//- resource; they are reference counted.</p>
{
//! Construction:
public:
	//: Resource Manager constructor.
	//- config_file - the name of the file in which the resources are defined
	//- provider - the optional inputprovider in which, the resource file is stored
	//- read_directly_from_source - if true, any resources are loaded directly from their source, meaning that any datafile-directive is ignored.
	CL_ResourceManager(
		const std::string &config_file,
		CL_InputSourceProvider *provider = NULL,
		bool read_directly_from_source=false,
		bool delete_inputsource_provider=false);

	//: Resource Manager constructor.
	//- file_name - the name of the file to open
	//- is_datafile - indicates if the file is a scriptfile or a datafile
	CL_ResourceManager(
		const std::string &config_file,
		const bool is_datafile );

	//: Resource Manager constructor.
	//- file_name - the name of the file to open
	//- is_datafile - indicates if the file is a scriptfile or a datafile
	//- additional_resources - additional resources to be included into the resource set.
	CL_ResourceManager(
		const std::string &config_file,
		const bool is_datafile,
		CL_ResourceManager &additional_resources);

	//: Resource Manager Constructor
	CL_ResourceManager(const CL_ResourceManager &copy);

	//: Construct empty resource manager.
	CL_ResourceManager();

	//: Resource Manager Destructor
	~CL_ResourceManager();

//! Attributes:
public:
	//: Returns a pointer to the CL_Resource representing the given resource
	CL_Resource &get_resource(const std::string &res_id);

	//: Returns a list of all resources available.
	//: Primarily used by the datafile compiler to build datafiles from resources.
	//- Returns - The list of resources available. You'll have to delete the list returned.
	std::list<std::string> *get_all_resources();

	//: Returns a list of all resources available matching a given type. 
	//: Primarily used by the ClanCompiler to build datafiles from resources.
	//- Returns - The list of resources available. You'll have to delete the list returned.
	std::list<std::string> *get_resources_of_type(const std::string &type_id);

	//: Returns a pointer to the inputsourceprovider, in which all resources are stored
	//: (this can be a fileprovider or a datafileprovider depending on method used to load the script file)
	//- Returns - Pointer to inputsourceprovider containing resource data.
	CL_InputSourceProvider *get_resource_provider() const;

	//: Returns true if the resources are loaded from source. False if loaded from a datafile.
	bool is_from_source();

//! Operations:
public:
	//: Copy a resource manager.
	void operator = (const CL_ResourceManager &copy);

	//: Add resources from an other resource manager.
	void add_resources(const CL_ResourceManager &additional_resources);

	//: Remove resources from an other resource manager.
	void remove_resources(const CL_ResourceManager &additional_resources);

	//: Loads all resources into memory.
	void load_all();

	//: Unloads all resources from memory.
	void unload_all();

	//: Loads all resources in a given section into memory.
	void load_section(const std::string &section_name);

	//: Unloads all resources in a given section into memory.
	void unload_section(const std::string &section_name);

//! Implementation:
public:
	//: Resource Manager Constructor
	CL_ResourceManager(class CL_ResourceManager_File *impl);

	//: Pointer to the implementation
	CL_ResourceManager_File *impl;
};
#endif
