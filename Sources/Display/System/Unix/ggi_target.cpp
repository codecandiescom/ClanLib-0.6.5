/*
	$Id: ggi_target.cpp,v 1.2 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "ggi_target.h"

const char *CL_GGITarget::get_long_name()
{
	return "Generic Graphics Interface (GGI)";
}

const char *CL_GGITarget::get_short_name()
{
	return "ggi";
}

void CL_GGITarget::init()
{
}

void CL_GGITarget::deinit()
{
}
