/*
	$Id: directory_scanner_generic.h,v 1.6 2002/01/09 23:45:16 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_directory_scanner_generic
#define header_directory_scanner_generic

#include <string>

class CL_DirectoryScanner_Generic
{
//!Construction:
public:
	CL_DirectoryScanner_Generic() : ref_count(0) { return; }

	virtual ~CL_DirectoryScanner_Generic() { return; }
	
//!Attributes:
public:
	//
	virtual bool scan (const std::string& pathname) =0;

	//
	virtual bool scan (const std::string& pathname, const std::string& pattern) =0;

	// Returns the path of the directory being scanned.
	virtual std::string get_directory_path() = 0;

	// Returns the name of the current found file.
	virtual std::string get_name() = 0;
	
	// Returns the name of the current found file, including the directory path.
	virtual std::string get_pathname() = 0;
	
	// Returns true if filename is a directory.
	virtual bool is_directory() = 0;

	// Returns true if filename is hidden.
	virtual bool is_hidden() = 0;


	// todo: add other attributes of a file.

//!Operations:
public:
	// Find next file in directory scan. Returns false if no more files was found.
	virtual bool next() = 0;

	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0)
		{
			delete this;
			return 0;
		}
		return ref_count;
	}

//!Implementation:
private:
	int ref_count;
};

#endif
