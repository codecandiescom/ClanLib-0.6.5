/*
	$Id: inputsource_provider_file.cpp,v 1.10 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <API/Core/IOData/inputsource_provider_file.h>
#include <API/Core/IOData/inputsource_file.h>

CL_InputSourceProvider_File::CL_InputSourceProvider_File(const std::string &path)
{
	char cwd[1026];
	if (getcwd(cwd, 1024) == NULL) throw CL_Error("Working dir is more than 1024 characters!");
	int len = strlen(cwd);
	if (cwd[len-1] != '/' && cwd[len-1] != '\\') strcat(cwd, "/");

	// try to figure out if path is absolute.
	if (path.length() == 0)
	{
		// path is relative

		provider_path = std::string(cwd) + std::string(path);
	}
	else if (path[0] == '\\' || path[0] == '/' || path[1] == ':')
	{
		// path is absolute
		provider_path = std::string(path);
	}
	else
	{
		// path is relative
		provider_path = std::string(cwd) + std::string(path);
	}

	// make sure the resulting path that we create ends with a '/'
	// this is *also* explicitly needed for everything that tries to load
	// something with inputprovider(".") ==> when "creating" surfaces without using
	// the resource system

	len=provider_path.length();
	if (provider_path[len-1] != '/' && provider_path[len-1] != '\\') 
		provider_path += '/';
}

CL_InputSourceProvider *CL_InputSourceProvider::create_file_provider(const std::string &path)
{
	return new CL_InputSourceProvider_File(path);
}

CL_InputSource *CL_InputSourceProvider_File::open_source(const std::string &filename)
{
	return new CL_InputSource_File(get_path(filename).c_str());
}

std::string CL_InputSourceProvider_File::get_path(const std::string &filename)
{
	std::string filepath;

	// if path is absolute, do not prepend provider path:
	if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':') filepath = filename;
	else filepath = provider_path + filename;

	return filepath;
}

CL_InputSourceProvider *CL_InputSourceProvider_File::clone()
{
	return new CL_InputSourceProvider_File(provider_path.c_str());
}

