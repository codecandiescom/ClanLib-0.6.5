/*
	$Id: resourceoptions.cpp,v 1.3 2001/04/10 14:24:30 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourceoptions.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions construction:

CL_ResourceOptions::CL_ResourceOptions()
{
}

CL_ResourceOptions::~CL_ResourceOptions()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions attributes:

std::map<std::string, CL_ResourceOption> &CL_ResourceOptions::get_options()
{
	return options;
}

CL_ResourceOption &CL_ResourceOptions::get_option(const std::string &name)
{
	return options[name];
}

const CL_ResourceOption &CL_ResourceOptions::get_option(const std::string &name) const
{
	return options.find(name)->second;
}

bool CL_ResourceOptions::exists(const std::string &name)
{
	return options.find(name) != options.end();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions operations:

void CL_ResourceOptions::add(const CL_ResourceOption &option)
{
	options[option.get_name()] = option;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceOptions implementation:
