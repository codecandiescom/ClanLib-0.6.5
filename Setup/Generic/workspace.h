/*
	$Id: workspace.h,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_workspace
#define header_workspace

#include "project.h"

class Workspace
{
// Construction:
public:

// Attributes:
public:
	std::list<Project> projects;
	// Projects in workspace.

	std::string output_include_dir;
	// Output include directory. Project headers should be copied to this destination.

	std::string output_lib_dir;
	// Output library directory. Project library files should be copied to this destination.

	std::string input_include_dir;
	// Input include directory path.

	std::string input_lib_dir;
	// Input library directory path.

// Operations:
public:

// Implementation:
private:
};

#endif
