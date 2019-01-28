/*
	$Id: workspace_generator_msvc.h,v 1.5 2002/02/13 19:13:41 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_workspace_generator_msvc
#define header_workspace_generator_msvc

#include "../Generic/workspace.h"
#include <fstream>

class WorkspaceGenerator_MSVC
// This class writes developer studio workspace and project files.
{
// Construction:
public:
	WorkspaceGenerator_MSVC();
	// Constructs a msvc++ workspace generator.

// Attributes:
public:

// Operations:
public:
	void write(const Workspace &workspace);
	// Write MSVC++ workspace and project files based on the passed generic template.

// Implementation:
private:
	void write_dsw(const Workspace &workspace);
	// Writes the Developer Studio workspace file.

	void begin_project(std::ofstream &dsw, const std::string &name, const std::string &filename);
	// Writes the 'begin_project' text in a workspace file.

	void end_project(std::ofstream &dsw);
	// Writes the 'end_project' text in a workspace file.

	void add_project_dependency(std::ofstream &dsw, const std::string &dep_name);
	// Adds a project dependency to a project in a workspace file.

	void write_dsp(const Workspace &workspace, const Project &project);
	// Writes a Developer Studio project file.

	void begin_group(std::ofstream &dsp, const std::string &group_name);
	// Writes the 'begin_group' text of a project file.

	void end_group(std::ofstream &dsp);
	// Writes the 'end_group' text of a project file.

	void add_file(std::ofstream &dsp, const std::string &filename);
	// Adds a file to a group.

	void install_mkdir(std::ofstream &dsp, const std::string &src_dir, const std::string &dest_dir, const Project *project);
	// Adds a directory to the install batch file.

	void install_copydir(std::ofstream &dsp, const std::string &src_dir, const std::string &dest_dir, const Project *project);
	// Adds copy commands for all files in a directory to the batch file.

	void generate_source_files(std::ofstream &dsp, const Project &project);
	// Generate the list of source file groups.

	std::list<std::string> extract_path(const std::string &fullname);
	// Returns the path of a file as a linked list.

	Workspace workspace;
	// Generic description of the workspace.
};

#endif
