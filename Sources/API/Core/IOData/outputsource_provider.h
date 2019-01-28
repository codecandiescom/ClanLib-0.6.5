/*
	$Id: outputsource_provider.h,v 1.9 2001/12/11 20:44:18 mbn Exp $

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

#ifndef header_outputsource_provider
#define header_outputsource_provider

#include <string>

class CL_OutputSource;

//: Class that provides output targets.
//- <p>CL_OutputSourceProvider is used to open output sources. This can be from a
//- datafile where the outputsource is an index - or it a directory where the
//- files are the outputsources.</p>
class CL_OutputSourceProvider
{
public:
//! Overrideables:
	//: Output Source Provider Destructor
	virtual ~CL_OutputSourceProvider() { return; }

	//: Opens an outputsource using the passed handle.
	//- handle - handle/index/filename/identier to the output source requested.
	//- Returns - The output source opened.
	virtual CL_OutputSource *open_source(const std::string &handle) = 0;

	//: Returns a copy of the current provider.
	//- Returns - The copy of the outputsource provider.
	virtual CL_OutputSourceProvider *clone() = 0;
};

#endif
