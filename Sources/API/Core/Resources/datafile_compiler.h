/*
	$Id: datafile_compiler.h,v 1.11 2002/06/13 14:43:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="Resources"
//! header=core.h

#ifndef header_datafile_compiler
#define header_datafile_compiler

#include <string>

//: Datafile writer class.
//- This class is used to write compressed datafiles.
class CL_DatafileCompiler
{
public:
//! Operations:
	//: Reads the script file(.scr) and creates a compressed datafile(.dat).
  //- scr_file - path to the .scr file
  //- dat_file - target for the .dat file
	static void write(
		const std::string &scr_file,
		const std::string &dat_file);
};

#endif
