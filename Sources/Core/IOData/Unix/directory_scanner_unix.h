/*
	$Id: directory_scanner_unix.h,v 1.5 2002/01/09 23:45:16 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_directory_scanner_unix
#define header_directory_scanner_unix

#include "../Generic/directory_scanner_generic.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

class CL_DirectoryScanner_Unix : public CL_DirectoryScanner_Generic
{
//!Construction:
public:
	CL_DirectoryScanner_Unix();
	~CL_DirectoryScanner_Unix();
	
	//: Construct directory scanner for the specified scan expression.
	//: The scan expression only supports * and ? wildcards. Eg: "gfx/*.jpg".
//	CL_DirectoryScanner_Generic(const std::string &scan_exp);

	//: Copy constructor.
//	CL_DirectoryScanner_Generic(const CL_DirectoryScanner &copy);

//!Attributes:
public:
	bool scan (const std::string& pathname);
	bool scan (const std::string& pathname, const std::string& pattern);

	// Returns the path of the directory being scanned.
	std::string get_directory_path();

	// Returns the name of the current found file.
	std::string get_name();
	
	// Returns the name of the current found file, including the directory path.
	std::string get_pathname();
	
	// Returns true if filename is a directory.
	bool is_directory();

	// Returns true if filename is hidden.
	bool is_hidden();

	// todo: add other attributes of a file.

//!Operations:
public:
	// Copy assignment operator.
	//CL_DirectoryScanner_Generic &operator =(const CL_DirectoryScanner_Generic &copy);

	// Find next file in directory scan. Returns false if no more files was found.
	bool next();
private:
	DIR *dir_temp;
	dirent *entry;
	struct stat statbuf;

	bool use_pattern;
	//Path stored without the trailing slash
	std::string file_name;
	std::string path_name;
	std::string file_pattern;

};

#endif
