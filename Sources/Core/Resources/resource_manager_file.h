/*
	$Id: resource_manager_file.h,v 1.8 2001/12/15 01:25:02 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_resource_manager_file
#define header_resource_manager_file

#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resourceoptions.h"
#include "API/Core/Resources/resource.h"
#include "resource_tokenizer.h"

#define default_scriptfile_id "_script_section"

class CL_ResourceManager_File
{
public:
	CL_ResourceManager_File(
		const std::string &config_file,
		CL_InputSourceProvider *provider = NULL,
		bool read_directly_from_source=false,
		bool delete_inputsource_provider=false);

	CL_ResourceManager_File(
		const std::string &file_name,
		const bool is_datafile);

	CL_ResourceManager_File();

	~CL_ResourceManager_File();

	void add_ref();
	void release_ref();

	void add_resources(CL_ResourceManager_File *resources);
	void remove_resources(CL_ResourceManager_File *resources);

	void load_all();
	void unload_all();

	void load_section(const std::string &section_name);
	void unload_section(const std::string &section_name);

	CL_Resource &get_resource(const std::string &res_id);
	CL_Resource &find_resource(const std::string &res_id, bool &found);
	std::list<std::string> *get_all_resources();
	std::list<std::string> *get_resources_of_type(std::string type_id);

	CL_InputSourceProvider *get_resource_provider();

	bool from_source;

private:
	void parse();

	void parse_include(CL_ResourceTokenizer &lexer);
	
	void parse_section_body(
		std::string token,
		CL_ResourceTokenizer &lexer,
		std::string prefix);

	CL_Resource create_resource(
		std::string name,
		std::string location,
		const CL_ResourceOptions &options);

	// Returns the filename part of a pathname. Eg. returns "filename.tga" if given string "/blabla/filename.tga".
	static std::string get_filename(const std::string &pathname);

	// Returns the path part of a pathname. Eg. returns "/blabla/" if given string "/blabla/filename.tga".
	static std::string get_path(const std::string &pathname);

	CL_InputSourceProvider *resource_provider;

	int ref_count;
	std::list<CL_Resource> resources;
	std::string filename;
	bool delete_resource_provider;
	std::list<CL_ResourceManager_File *> additional_resources;
};

#endif
