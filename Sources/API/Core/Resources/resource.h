/*
	$Id: resource.h,v 1.18 2001/12/16 17:19:33 starch Exp $

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

#ifndef header_resource
#define header_resource

#include "../../signals.h"
#include <string>

class CL_ResourceOptions;
class CL_ResourceManager;
class CL_ResourceData;
class CL_InputSource;
class CL_OutputSource;
class CL_InputSourceProvider;
class CL_OutputSourceProvider;
class CL_Resource_Generic;

//: Interface representing a resource in the resource manager.
class CL_Resource
{
//! Construction:
public:
	//: Resource Constructor
	CL_Resource(
		const std::string &type,
		const std::string &name,
		const std::string &location,
		const CL_ResourceOptions &options,
		const CL_ResourceManager &manager);

	//: Resource Constructor
	CL_Resource(const CL_Resource &copy);

	//: Resource Constructor
	CL_Resource();

	//: Resource Destructor
	virtual ~CL_Resource();

//! Attributes:
public:
	//: Returns the type of the resource.
	const std::string &get_type() const;

	//: Returns the name of the resource.
	const std::string &get_name() const;

	//: Returns the location of the resource (relative to resource definition file).
	//: Use this function if file is to be opened with CL_ResourceManager::get_resource_provider().
	const std::string &get_location() const;

	//: Returns the full path location of the resource.
	//: Use this function if file is to be opened without ClanLib input sources.
	std::string get_full_location() const;

	//: Returns the resource options.
	CL_ResourceOptions &get_options();

	//: Returns the resource manager.
	CL_ResourceManager get_manager();

	//: Returns the data with the matching name.
	CL_ResourceData *get_data(const std::string &name);

	//: Returns the current reference count.
	int get_reference_count() const;

//! Signals:
public:
	//: Signal invoked when a call to load_file is made.
	CL_Signal_v0 &sig_load_file();

	//: Signal invoked when a call to load_datafile is made.
	CL_Signal_v1<CL_InputSourceProvider *> &sig_load_datafile();

	//: Signal invoked when a call to save_datafile is made.
	CL_Signal_v1<CL_OutputSourceProvider *> &sig_save_datafile();

	//: Signal invoked when a call to unload is made.
	CL_Signal_v0 &sig_unload();

//! Operators:
public:
	//: Copy a resource.
	void operator = (const CL_Resource &copy);

//! Operations:
public:

	//: Attach some data to the resource.
	void attach_data(const std::string &name, CL_ResourceData *data);

	//: Detach some data from the resource.
	void detach_data(CL_ResourceData *data);

	//: Unloads the resource from memory.
	void unload();

	//: <p>Loads the resource, using the prefered source as specified by
	//: the resource manager.</p>
	void load();

	//: Loads the resource from file.
	void load_file();

	//: Loads the resource from the input source.
	void load_datafile(CL_InputSourceProvider *input_provider);

	//: Saves the resource to the output source.
	void save_datafile(CL_OutputSourceProvider *output_provider);

//! Implementation:
public:
	//: Resource Constructor
	CL_Resource(class CL_Resource_Generic *impl);

	//: Pointer to implementation
	CL_Resource_Generic *impl;
};

#endif
