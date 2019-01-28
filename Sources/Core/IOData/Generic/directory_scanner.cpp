/*
	$Id: directory_scanner.cpp,v 1.7 2002/01/09 23:45:16 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/IOData/directory_scanner.h"
#include "directory_scanner_generic.h"

#ifdef WIN32
#include "../win32/directory_scanner_win32.h"
#else
#include "../Unix/directory_scanner_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner construction:

CL_DirectoryScanner::CL_DirectoryScanner() : impl(0)
{
#ifdef WIN32
	impl = new CL_DirectoryScanner_Win32();
#else
	impl = new CL_DirectoryScanner_Unix();
#endif
	if (impl) impl->add_ref();  
}

CL_DirectoryScanner::CL_DirectoryScanner(const CL_DirectoryScanner &copy) : impl(0)
{
	impl = copy.impl;
	if (impl) impl->add_ref();
}

CL_DirectoryScanner::~CL_DirectoryScanner()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner attributes:

bool CL_DirectoryScanner::scan(const std::string& pathname) 
{
	return impl->scan (pathname);
}

bool CL_DirectoryScanner::scan(const std::string& pathname, const std::string& pattern)
{
	return impl->scan (pathname, pattern);
}

std::string CL_DirectoryScanner::get_directory_path()
{
	return impl->get_directory_path();
}

std::string CL_DirectoryScanner::get_name()
{
	return impl->get_name();
}
	
std::string CL_DirectoryScanner::get_pathname()
{
	return impl->get_pathname();
}
	
bool CL_DirectoryScanner::is_directory()
{
	return impl->is_directory();
}

bool CL_DirectoryScanner::is_hidden()
{
	return impl->is_hidden();
}


/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner operations:

bool CL_DirectoryScanner::next() 
{ 
	return impl->next(); 
}
