/*
	$Id: project.h,v 1.6 2002/02/13 18:56:19 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_project
#define header_project

#include <string>
#include <list>

class Project
// Generic description of a clanlib project.
{
// Construction:
public:
	Project();
	// Construct an empty project.

	Project(
		const std::string &name,
		const std::string &libname,
		const std::string &headername,
		const std::list<std::string> &libs_list_shared,
		const std::list<std::string> &libs_list_release,
		const std::list<std::string> &libs_list_debug,
		const std::list<std::string> &defines_list,
		const std::list<std::string> &ignore_list = std::list<std::string>());
	// Construct a project by searching automatically for the files in the Sources subdir.

// Attributes:
public:
	std::string name;
	// Project subdir name. Eg. "Core".

	std::string libname;
	// Project library name. Eg. "libCore".

	std::string headername;
	// Project library name. Eg. "core.h".


	std::list<std::string> defines_list;
	// Defines set for this project.

	std::list<std::string> libs_shared;
	// Libraries to link with in all targets.

	std::list<std::string> libs_release;
	// Libraries to link with in release target.

	std::list<std::string> libs_debug;
	// Libraries to link with in debug target.

	std::list<std::string> files;
	// Files used by project.

	std::list<std::string> file_paths;
	// Files and path

	std::list<std::string> paths;
	// Unique Paths used

	std::list<std::string> dependencies;
	// Other projects that this project depends on.

// Operations:
public:

// Implementation:
private:
	void generate_dir(
		const std::string &dir,
		const std::list<std::string> &ignore_list);
	// Scan directory for files and add them to file list.
};

#endif
