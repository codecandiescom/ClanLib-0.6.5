/*
	$Id: resourceoptions.h,v 1.10 2001/09/22 15:52:11 plasmoid Exp $

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

#ifndef header_resourceoptions
#define header_resourceoptions

#include <map>
#include <string>
#include "resourceoption.h"

class CL_ResourceManager;

//: Resource Options Class
class CL_ResourceOptions
{
//! Construction:
public:
	//: Resource Options Constructor
	CL_ResourceOptions();

	//: Resource Options Destructor
	~CL_ResourceOptions();

//! Attributes:
public:
	//: Returns a list of all the resource options.
	std::map<std::string, CL_ResourceOption> &get_options();

	//: Returns a resource option.
	CL_ResourceOption &get_option(const std::string &name);

	//: Returns a resource option.
	const CL_ResourceOption &get_option(const std::string &name) const;

	//: Returns true if an option exists.
	bool exists(const std::string &name);

//! Operations:
public:
	//: Add a resource option.
	void add(const CL_ResourceOption &option);

//! Implementation:
private:
	std::map<std::string, CL_ResourceOption> options;
};

#endif
