/*
	$Id: inputsource_provider_file.h,v 1.8 2001/12/11 20:44:18 mbn Exp $

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

#ifndef header_inputsource_provider_file
#define header_inputsource_provider_file

#include "inputsource_provider.h"
#include <string>

class CL_InputSource;

//: The Input Source Provider File Class
class CL_InputSourceProvider_File : public CL_InputSourceProvider
{
//! Construction:
public:
	//: The Input Source Provider File Constructor
	CL_InputSourceProvider_File(const std::string &path);

	//: Open Source
	virtual CL_InputSource *open_source(const std::string &filename);

	//: Get full path to source:
	std::string get_path(const std::string &filename);

	//: Clone
	virtual CL_InputSourceProvider *clone();

//! Implementation:
private:
	std::string provider_path;
};

#endif
