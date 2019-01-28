/*
	$Id: stylemanager.cpp,v 1.1 2002/02/28 11:38:16 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager_Generic:

class CL_StyleManager_Generic
{
public:
	CL_ResourceManager *resources;
};

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager construction:

CL_StyleManager::CL_StyleManager(CL_ResourceManager *resources)
: impl(new CL_StyleManager_Generic)
{
	impl->resources = resources;
}

CL_StyleManager::~CL_StyleManager()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager attributes:

CL_ResourceManager *CL_StyleManager::get_resources()
{
	return impl->resources;
}
