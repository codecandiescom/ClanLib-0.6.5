/*
	$Id: directory_scanner_win32.h,v 1.5 2002/01/10 21:34:52 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_directory_scanner_win32
#define header_directory_scanner_win32

#include "../Generic/directory_scanner_generic.h"

#include <io.h>
#include <direct.h>


class CL_DirectoryScanner_Win32: public CL_DirectoryScanner_Generic
{
//!Construction:
public:
	CL_DirectoryScanner_Win32();
	virtual ~CL_DirectoryScanner_Win32();

	//: Copy constructor.
	CL_DirectoryScanner_Win32(const CL_DirectoryScanner_Win32 &copy);

//!Attributes:
public:
	virtual bool scan (const std::string &pathname);

	virtual bool scan (const std::string &pathname, const std::string &pattern);

	// Returns the path of the directory being scanned.
	virtual std::string get_directory_path();

	// Returns the name of the current found file.
	virtual std::string get_name();
	
	// Returns the name of the current found file, including the directory path.
	virtual std::string get_pathname();
	
	// Returns true if filename is a directory.
	virtual bool is_directory();

	// Returns true if filename is hidden.
	virtual bool is_hidden();

	// todo: add other attributes of a file.

//!Operations:
public:
	// Find next file in directory scan. Returns false if no more files was found.
	virtual bool next();

//!Implementation
private:
	std::string path_with_ending_slash(const std::string &path);

	bool find_file();
	
	bool valid_file()
	{
		return ((m_firsthandle>0) && (m_handle==0));
	}

	struct _finddata_t m_fileinfo;
	long m_firsthandle;
	long m_handle;
	std::string m_scan_exp;
};

#endif
