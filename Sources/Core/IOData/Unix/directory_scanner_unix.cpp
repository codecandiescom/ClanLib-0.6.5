/*
        $Id: directory_scanner_unix.cpp,v 1.7 2002/01/10 10:23:30 grumbel Exp $

        ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        See http://www.clanlib.org
        ------------------------------------------------------------------------
*/

#include <API/Core/System/error.h>
#include <iostream>
#include <libgen.h>
#include <fnmatch.h>
#include "directory_scanner_unix.h"

CL_DirectoryScanner_Unix::CL_DirectoryScanner_Unix ()
	: dir_temp (NULL), entry (NULL)
{
}

bool CL_DirectoryScanner_Unix::scan (const std::string& arg_path_name)
{
	path_name   = arg_path_name;
	use_pattern = false;

	if (path_name.empty ())
	  path_name = ".";

	if(dir_temp)
		closedir(dir_temp);

	dir_temp = opendir(path_name.c_str());
	if (dir_temp == NULL)
		return false;
	else
		return true;
}

bool CL_DirectoryScanner_Unix::scan (const std::string& arg_path_name, 
				     const std::string& arg_file_pattern)
{
	path_name    = arg_path_name;
	file_pattern = arg_file_pattern;
	use_pattern  = true;

	if (path_name.empty ())
	  path_name = ".";

	if(dir_temp)
		closedir(dir_temp);

	dir_temp = opendir(path_name.c_str());
	if (dir_temp == NULL)
		return false;
	else
		return true;
}

CL_DirectoryScanner_Unix::~CL_DirectoryScanner_Unix()
{
	if(dir_temp)
		closedir(dir_temp);
}

std::string CL_DirectoryScanner_Unix::get_directory_path()
{
	return path_name;
}

std::string CL_DirectoryScanner_Unix::get_name()
{
	return file_name;
}

std::string CL_DirectoryScanner_Unix::get_pathname()
{
	return path_name + "/" + file_name;
}

bool CL_DirectoryScanner_Unix::is_directory()
{
	return (statbuf.st_mode & S_IFDIR);
}

bool CL_DirectoryScanner_Unix::is_hidden()
{
	return file_name[0] == '.';
}


bool CL_DirectoryScanner_Unix::next()
{	
	assert (dir_temp);

	entry = readdir(dir_temp);

	if( entry == NULL )
		return false;

	stat(entry->d_name,&statbuf);

	file_name = entry->d_name;

	if (use_pattern)
	{
		if (fnmatch(file_pattern.c_str(), file_name.c_str(), FNM_PATHNAME) == 0)
			return true;
		else
			return next();
	} else {
		return true;
	}
}


