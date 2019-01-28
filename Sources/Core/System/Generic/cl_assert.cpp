/*
	$Id: cl_assert.cpp,v 1.5 2001/08/28 23:49:05 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <cstdlib>
#include <iostream>
#include <cassert>

#ifdef BORLAND
#pragma hdrstop
#endif

#include "API/Core/System/cl_assert.h"

std::list<CL_AssertListener*> CL_Assert::listeners;

void CL_Assert::enable_channels(int /*channel_mask*/)
{
	// not implemented yet!
}

void CL_Assert::die(bool a, const char *file, int line, const char *function, const char *assert_str)
{
	if (a) return;

	if (function == NULL)
	{
		std::cout << std::endl;
		std::cout << "ClanLib Assert: " << file << ":" << line << std::endl;
		std::cout << "                '" << assert_str << "' failed." << std::endl << std::endl;
	}
	else
	{
		std::cout << std::endl;
		std::cout << "ClanLib Assert: " << file << ":" << line << "," << std::endl;
		std::cout << "                in function " << function << std::endl;
		std::cout << "                '" << assert_str << "' failed." << std::endl << std::endl;
	}
	
	for (
		std::list<CL_AssertListener*>::iterator it = listeners.begin();
		it != listeners.end();
		it++)
	{
		(*it)->assert_occurred(file, line);
	}
	
	abort();
}

void CL_Assert::info(int channel, const char *text, const char *file, int line)
{
	std::cout << "ClanLib Info(" << channel << "): " << text << " from " << file 
		<< ":" << line << std::endl;
}

void CL_Assert::add_listener(CL_AssertListener *listener)
{
	listeners.push_back(listener);
}

void CL_Assert::remove_listener(CL_AssertListener *listener)
{
	listeners.remove(listener);
}
