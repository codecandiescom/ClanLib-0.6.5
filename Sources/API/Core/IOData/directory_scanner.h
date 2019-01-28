/*
	$Id: directory_scanner.h,v 1.7 2002/06/13 14:43:14 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_directory_scanner
#define header_directory_scanner

#include <string>

class CL_DirectoryScanner_Generic;

class CL_DirectoryScanner
{
//!Construction:
public:
	//: Construct directory scanner for iterating over a directory
	CL_DirectoryScanner();

	//: Copy constructor.
	CL_DirectoryScanner(const CL_DirectoryScanner &copy);

	//: Destructor.
	~CL_DirectoryScanner();

//!Attributes:
public:
	// Return 'true' if directory can't be accessed
	bool scan(const std::string& pathname);

	// Return 'true' if directory can't be accessed
	bool scan(const std::string& pathname, const std::string& pattern);

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
	CL_DirectoryScanner &operator =(const CL_DirectoryScanner &copy);

	// Find next file in directory scan. Returns false if no more files was found.
	bool next();

//!Implementation:
private:
	// Yada yada, usual data hiding.
	CL_DirectoryScanner_Generic *impl;
};

#endif
