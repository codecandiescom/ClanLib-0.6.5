/*
	$Id: generator.h,v 1.5 2001/09/28 21:33:23 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "../Generic/project.h"
#include "../Generic/workspace.h"

void create_release(Workspace &work);
void create_makefile(Workspace &work);
void bpr_create_file(Project &proj, bool debug);
void mak_create_file(Project &proj, bool debug);
