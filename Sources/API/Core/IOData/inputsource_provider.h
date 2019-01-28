/*
	$Id: inputsource_provider.h,v 1.10 2002/02/28 22:12:56 japj Exp $

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

#ifndef header_inputsource_provider
#define header_inputsource_provider

#include <string>
class CL_InputSource;

//: Class that provides input sources.
//- <p>CL_InputSourceProvider is used to open inputsources. This can be from a
//- datafile where the inputsource is an index - or it a directory where the
//- files are the inputsources.</p>
class CL_InputSourceProvider
{
public:
//! Construction:
	//: Input Source Provider destructor
	virtual ~CL_InputSourceProvider() { return; }

	//: Creates a file provider.
	//: Input sources opened are considered as files.
	//- path - Path where the files are located.
	//- Returns - The file input provider created.
	static CL_InputSourceProvider *create_file_provider(const std::string &path);

	//: Creates a datafile provider.
	//: Input sources are opened as indexes in the specified datafile.
	//- filename - Name of the datafile to be used.
	//- Returns - The datafile input provider created.
	static CL_InputSourceProvider *create_datafile_provider(const std::string &filename);

//! Overrideables:
	//: Opens an inputsource using the passed handle.
	//- handle - handle/index/filename/identier to the input source requested.
	//- Returns - The opened input source.
	virtual CL_InputSource *open_source(const std::string &handle) = 0;

	//: Returns a copy of the current provider.
	//- Returns - The copy of the inputsource provider.
	virtual CL_InputSourceProvider *clone() = 0;

	//: Get full path to source:
	virtual std::string get_path(const std::string &filename) = 0;

};

#endif
