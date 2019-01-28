/*
	$Id: directory_scanner_win32.cpp,v 1.7 2002/01/10 21:36:38 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/cl_assert.h"
#include "directory_scanner_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner_Win32 construction:

CL_DirectoryScanner_Win32::CL_DirectoryScanner_Win32() 
: m_firsthandle(0), m_handle(0)
{
}

CL_DirectoryScanner_Win32::CL_DirectoryScanner_Win32(const CL_DirectoryScanner_Win32 &copy) 
: m_firsthandle(0)
{

}

CL_DirectoryScanner_Win32::~CL_DirectoryScanner_Win32()
{
	if (m_firsthandle)
		_findclose(m_firsthandle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner_Win32 attributes:

bool CL_DirectoryScanner_Win32::scan (const std::string &pathname)
{
	return scan (pathname, "*.*");
}

bool CL_DirectoryScanner_Win32::scan (const std::string &_pathname, const std::string& pattern)
{
	if (m_firsthandle)
		_findclose(m_firsthandle);

	std::string pathname = path_with_ending_slash(_pathname);

	// find first file
	m_firsthandle = _findfirst((char*)(pathname + pattern).c_str (), &m_fileinfo);
	
	// update m_handle in order for valid_file to work correctly
	m_handle = (m_firsthandle != -1) ? 0 : -1;

	return (m_firsthandle != -1);
}

std::string CL_DirectoryScanner_Win32::get_directory_path()
{
	//fallback
	//if (!valid_file())
		return std::string("");

	//return; 
}

std::string CL_DirectoryScanner_Win32::get_name()
{
	//fallback
	if (!valid_file())
		return std::string("");
	
	return std::string(m_fileinfo.name);
}
	
std::string CL_DirectoryScanner_Win32::get_pathname()
{
	//fallback
//	if (!valid_file())
		return std::string("");

//	return m_fileinfo;
}
	
bool CL_DirectoryScanner_Win32::is_directory()
{	//fallback
	if (!valid_file())
		return false;

	return ((m_fileinfo.attrib & _A_SUBDIR) == _A_SUBDIR);
}

bool CL_DirectoryScanner_Win32::is_hidden()
{	//fallback
	if (!valid_file())
		return false;

	return ((m_fileinfo.attrib & _A_HIDDEN) == _A_HIDDEN);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner_Win32 operations:

bool CL_DirectoryScanner_Win32::next() 
{ 
	return find_file();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner_Win32 implementation:

std::string CL_DirectoryScanner_Win32::path_with_ending_slash(const std::string &path)
{
	int len = path.length();
	if (len == 0) return "./";
	if (path[len-1] == '/' || path[len-1] == '\\') return path;
	return path + "/";
}

bool CL_DirectoryScanner_Win32::find_file()
{
	// find next files
	m_handle=_findnext(m_firsthandle,&m_fileinfo);
	
	return (m_handle == 0);
}
