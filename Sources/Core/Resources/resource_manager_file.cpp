/*
	$Id: resource_manager_file.cpp,v 1.22 2002/01/09 09:54:31 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include <API/Core/Resources/resource.h>
#include "Core/precomp.h"
#include "resource_manager_file.h"
#include "resource_tokenizer.h"
#include <API/Core/System/clanstring.h>
#include <API/Core/System/error.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/IOData/inputsource_provider_file.h>
#include <API/Core/IOData/inputsource.h>
#include <API/Core/IOData/inputsource.h>
#include <API/Core/Resources/resourcetype.h>
#include <algorithm>
#include <ctype.h>

CL_ResourceManager_File::CL_ResourceManager_File()
: resource_provider(0), ref_count(1), delete_resource_provider(false)
{
}

CL_ResourceManager_File::CL_ResourceManager_File(
	const std::string &config_file,
	CL_InputSourceProvider *provider,
	bool read_directly_from_source,
	bool delete_inputsource_provider)
: ref_count(1)
{
	filename = config_file;
	resource_provider = provider;
	delete_resource_provider = delete_inputsource_provider;
	from_source = read_directly_from_source;
	
	if (provider == NULL)
	{
		if (read_directly_from_source)
		{
			resource_provider = CL_InputSourceProvider::create_file_provider(get_path(filename).c_str());
			filename = get_filename(filename); // cut off path of filename
		}
		else
		{
			resource_provider = CL_InputSourceProvider::create_datafile_provider(filename.c_str());
		}
		
		delete_resource_provider = true;
	}

	parse();
}

CL_ResourceManager_File::CL_ResourceManager_File(
	const std::string &file_name,
	const bool is_datafile)
: ref_count(1)
{
	resource_provider = NULL;
	filename = file_name;
	delete_resource_provider = true;

	if (is_datafile)
	{
		resource_provider = CL_InputSourceProvider::create_datafile_provider(filename.c_str());
		from_source = false;
	}
	else
	{
		resource_provider = CL_InputSourceProvider::create_file_provider(get_path(filename).c_str());
		filename = get_filename(filename); // cut off path of filename
		from_source = true;
	}
	
	parse();
}

CL_ResourceManager_File::~CL_ResourceManager_File()
{
	std::list<CL_ResourceManager_File *>::iterator it;
	for (it = additional_resources.begin(); it != additional_resources.end(); it++)
	{
		CL_ResourceManager_File *cur = *it;
		cur->release_ref();
	}

	if (delete_resource_provider)
	{
		delete resource_provider;
		resource_provider = NULL;
	}
/*
	bool first_msg = true;
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != resources.end();
		it++)
	{
		CL_Resource &res = *it;

		if (res.get_reference_count() != 0)
		{
			if (first_msg)
			{
				std::cout << std::endl;
				std::cout << "Not all resources in " << filename.c_str() << " was unloaded!" << std::endl;
				first_msg = false;
			}

			std::cout << res.get_name().c_str() << " had load count " << res.get_reference_count() << std::endl;
		}
	}

	if (!first_msg)
	{
		std::cout << std::endl;
		std::cout << "Please check your program for memory leaks!" << std::endl;
	}
*/
}

void CL_ResourceManager_File::add_ref()
{
	ref_count++;
}

void CL_ResourceManager_File::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

void CL_ResourceManager_File::add_resources(CL_ResourceManager_File *resources)
{
	resources->add_ref();
	additional_resources.push_back(resources);
}

void CL_ResourceManager_File::remove_resources(CL_ResourceManager_File *resources)
{
	std::list<CL_ResourceManager_File *>::iterator it = std::find(additional_resources.begin(), additional_resources.end(), resources);
	if (it != additional_resources.end())
	{
		additional_resources.erase(it);
		resources->release_ref();
	}
}

void CL_ResourceManager_File::load_all()
{
	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		it->load();
	}

	std::list<CL_ResourceManager_File *>::iterator it_additional;
	for (it_additional = additional_resources.begin(); it_additional != additional_resources.end(); it_additional++)
	{
		CL_ResourceManager_File *cur = *it_additional;
		cur->load_all();
	}
}

void CL_ResourceManager_File::unload_all()
{
	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		it->unload();
	}

	std::list<CL_ResourceManager_File *>::iterator it_additional;
	for (it_additional = additional_resources.begin(); it_additional != additional_resources.end(); it_additional++)
	{
		CL_ResourceManager_File *cur = *it_additional;
		cur->unload_all();
	}
}

void CL_ResourceManager_File::load_section(const std::string &section_name)
{
	CL_String prefix = section_name;
	prefix << "/";
	
	int prefix_len = prefix.get_length();

	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		CL_String name = it->get_name();
		if (name.mid(0, prefix_len) == prefix)
		{
			it->load();
		}
	}

	std::list<CL_ResourceManager_File *>::iterator it_additional;
	for (it_additional = additional_resources.begin(); it_additional != additional_resources.end(); it_additional++)
	{
		CL_ResourceManager_File *cur = *it_additional;
		cur->load_section(section_name);
	}
}

void CL_ResourceManager_File::unload_section(const std::string &section_name)
{
	CL_String prefix = section_name;
	prefix << "/";
	
	int prefix_len = prefix.get_length();

	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		CL_String name = it->get_name();
		if (name.mid(0, prefix_len) == prefix)
		{
			it->unload();
		}
	}

	std::list<CL_ResourceManager_File *>::iterator it_additional;
	for (it_additional = additional_resources.begin(); it_additional != additional_resources.end(); it_additional++)
	{
		CL_ResourceManager_File *cur = *it_additional;
		cur->unload_section(section_name);
	}
}

char tol(char ch)
{
	if('A' <= ch && ch <= 'Z')
		return ch += 0x20;
  else
  	return ch;
}

CL_Resource &CL_ResourceManager_File::find_resource(const std::string &res_id, bool &found)
{
	static CL_Resource null_resource;

	std::string lower_res_id = res_id;
	std::transform(lower_res_id.begin(), lower_res_id.end(), lower_res_id.begin(), tol);
	//std::transform(lower_res_id.begin(), lower_res_id.end(), lower_res_id.begin(), tolower);

	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		found = true;
		if (it->get_name() == lower_res_id) return *it;
	}

	found = false;
	return null_resource;
}

CL_Resource &CL_ResourceManager_File::get_resource(const std::string &res_id)
{
	bool found = false;
	CL_Resource &local_resource = find_resource(res_id, found);
	if (found) return local_resource;

	std::list<CL_ResourceManager_File *>::iterator it_additional;
	for (it_additional = additional_resources.begin(); it_additional != additional_resources.end(); it_additional++)
	{
		CL_ResourceManager_File *cur = *it_additional;
		CL_Resource &additional_resource = cur->find_resource(res_id, found);
		if (found) return additional_resource;
	}

	CL_String err;
	err << "Resource " << res_id.c_str() << " not found.";
	throw CL_Error(err.get_string());

	static CL_Resource null_resource;
	return null_resource;
}

std::list<std::string> *CL_ResourceManager_File::get_all_resources()
{
	std::list<std::string> *retval = new std::list<std::string>;

	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		retval->push_back(it->get_name());
	}
	
	return retval;
}

std::list<std::string> *CL_ResourceManager_File::get_resources_of_type(std::string type_id)
{
	std::list<std::string> *retval = new std::list<std::string>;

	std::list<CL_Resource>::iterator it_end(resources.end());
	for (
		std::list<CL_Resource>::iterator it = resources.begin();
		it != it_end;
		++it)
	{
		if (it->get_type() == type_id)
			retval->push_back(it->get_name());
	}

	return retval;
}

CL_InputSourceProvider *CL_ResourceManager_File::get_resource_provider()
{
	return resource_provider;
}

void CL_ResourceManager_File::parse()
{
	CL_InputSource *input = NULL;

	if (from_source) input = resource_provider->open_source(filename.c_str());
	else input = resource_provider->open_source(default_scriptfile_id);

	try
	{
		CL_ResourceTokenizer lexer(filename, input);

		while (true)
		{
			std::string token = lexer.get_next_token();
			if (token == "") break;
			
			if (token == "location") // obsolete. Will read it but issues a warning.
			{
				token = lexer.get_next_token();
				if (token != "=")
					throw CL_Error(lexer.write_error("Missing '=' following global 'location'-declaration"));
				
				token = lexer.get_next_token();
				if (token != "datafile")
					throw CL_Error(lexer.write_error("Syntax error following global 'location'-declaration"));
				
				token = lexer.get_next_token();
				if (token != "(")
					throw CL_Error(lexer.write_error("Missing '(' following 'datafile' keyword"));

				token = lexer.get_next_token();

				token = lexer.get_next_token();
				if (token != ")")
					throw CL_Error(lexer.write_error("Missing ')' following datafile filename"));

				token = lexer.get_next_token();
				if (token != ";")
					throw CL_Error(lexer.write_error("Missing ';' following datafile filename"));

				cl_info(0, "The location keyword is obsolete in resource files. Please don't use it anymore.");
			}
			else // assume it is a section body (resource or section)
			{
				parse_section_body(token, lexer, "");
			}
		}
	}
	catch (...)
	{
		delete input;
		throw;
	}

	delete input;
}

void CL_ResourceManager_File::parse_include(CL_ResourceTokenizer &lexer)
{
/*	std::string input_file = lexer.get_next_token();
	
	std::string token = lexer.get_next_token();
	if (token != ";")
		throw CL_Error(lexer.write_error("Missing ';' following include"));

	CL_ResourceManager *manager = CL_ResourceManager::create(input_file.c_str(), NULL, true);
*/	
/*    std::list<std::string> *resource_list = manager->get_all_resources();

	for (
		std::list<std::string>::iterator it = resource_list->begin();
		it != resource_list->end();
		it++)
	{
		CL_Resource *res = manager->get_resource(*it);

		if (res != NULL)
		{
			resources.push_back(res);
		}
	}

	delete resource_list;
	delete manager;
*/
}

void CL_ResourceManager_File::parse_section_body(
	std::string token,
	CL_ResourceTokenizer &lexer,
	std::string prefix)
{
	if (token == "section")
	{
		CL_String section_prefix = prefix;
		section_prefix << lexer.get_next_token();
		section_prefix << "/";

		token = lexer.get_next_token();
		if (token != "{")
			throw CL_Error(lexer.write_error("Missing '{'"));
			
		while (true)
		{
			token = lexer.get_next_token();
			if (token == "}") break;
			parse_section_body(token, lexer, section_prefix.get_string());
		}
	}
	else // must be a resource then
	{
		CL_String name = prefix;
		name << token;

		token = lexer.get_next_token();
		if (token != "=")
		{
			CL_String err;
			err << "Missing '=' following declaration of resource '" << name << "'";
			throw CL_Error(lexer.write_error(err.get_string()));
		}

		// read the location (if available):
		std::string location;
		token = lexer.get_next_token();
		if(token == "(")
			location = "";
		else {
			location = token;
			token = lexer.get_next_token();
		}

		// read resource options:
		if (token != "(")
		{
			CL_String err;
			err << "Missing '(' following declaration of resource '" << name << "'";
			throw CL_Error(lexer.write_error(err.get_string()));
		}

		CL_ResourceOptions options;
		while (true)
		{
			token = lexer.get_next_token();
			if (token == ")") break;
			
			std::string option_name = token;
			
			token = lexer.get_next_token();
			if (token != "=")
			{
				CL_String err;
				err << "Syntax error in options following declaration of resource '" << name << "'";
				throw CL_Error(lexer.write_error(err.get_string()));
			}
			
			token = lexer.get_next_token();
			if (token == "(") // it is a value list
			{
				CL_ResourceOption option(option_name);

				while (true)
				{
					token = lexer.get_next_token();
					if (token == ")") break;

					option.add_value(token);
					
					token = lexer.get_next_token();
					if (token == ")") break;
					if (token != ",")
					{
						CL_String err;
						err << "Missing ',' or ')' following declaration of multiple option values in option '" <<
							   option_name << "' in resource '" << name << "' (found '" << token << "')";
						throw CL_Error(lexer.write_error(err.get_string()));
					}
				}
				
				options.add(option);
			}
			else
			{
				std::string option_value = token;
				
				options.add(CL_ResourceOption(option_name, option_value));
			}

			token = lexer.get_next_token();
			if (token == ")") break;
			if (token != ",")
			{
				CL_String err;
				err << "Syntax error in options following declaration of resource '" << name << "'";
				throw CL_Error(lexer.write_error(err.get_string()));
			}
		}
		
		token = lexer.get_next_token();
		if (token != ";")
		{
			CL_String err;
			err << "Missing ';' following declaration of resource '" << name << "'";
			throw CL_Error(lexer.write_error(err.get_string()));
		}
		
		if (options.exists("type") == false)
		{
			CL_String err;
			err << "No type declared for resource '" << name << "'";
			throw CL_Error(lexer.write_error(err.get_string()));
		}
		
		CL_Resource res = create_resource(name.get_string(), location, options);
		resources.push_back(res);
	}
}

CL_Resource CL_ResourceManager_File::create_resource(
	std::string name,
	std::string location,
	const CL_ResourceOptions &options)
{
	std::string type = options.get_option("type").get_value();

  CL_ResourceOptions op;
  CL_ResourceManager manage(this);

	CL_Resource resource(type, name, location, options, manage);

	bool found_type = false;
	for (
		std::list<CL_ResourceType*>::iterator it = CL_ResourceType::resource_types.begin();
		it != CL_ResourceType::resource_types.end();
		it++)
	{
		if ((*it)->get_type() == type)
		{
			(*it)->connect_data(resource);
			found_type = true;
		}
	}

	if (found_type == false)
	{
		CL_String err;
		err << "Unknown type '" << options.get_option("type").get_value().c_str() << "' declared for resource '" << name << "'";
//		throw CL_Error(lexer.write_error(err));
		throw CL_Error(err.get_string());
	}

	return resource;
}

std::string CL_ResourceManager_File::get_filename(const std::string &pathname)
{
	std::string::size_type pos1 = pathname.find_last_of('/');
	std::string::size_type pos2 = pathname.find_last_of('\\');

	if (pos1 == std::string::npos && pos2 == std::string::npos)
	{
		return pathname;
	}
	else if (pos1 != std::string::npos && pos2 == std::string::npos)
	{
		return pathname.substr(pos1 + 1);
	}
	else if (pos1 == std::string::npos && pos2 != std::string::npos)
	{
		return pathname.substr(pos2 + 1);
	}
	else // (pos1 != std::string::npos && pos2 != std::string::npos)
	{
		return pathname.substr(((pos1>pos2) ? pos1 : pos2) + 1);
	}
}

std::string CL_ResourceManager_File::get_path(const std::string &pathname)
{
	std::string::size_type length = pathname.length();
 	std::string::size_type pos1 = pathname.find_last_of('/');
	std::string::size_type pos2 = pathname.find_last_of('\\');
	if (pos1 == std::string::npos) pos1 = length;
	if (pos2 == std::string::npos) pos2 = length;
	if (pos1 < 0) pos1 = length;
	if (pos2 < 0) pos2 = length;

	if (pos1 == length && pos2 == length) return std::string();

	int pos = (pos1 < pos2) ? pos1 : pos2;
	return pathname.substr(0, pos);
}
