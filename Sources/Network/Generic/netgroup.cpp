/*
	$Id: netgroup.cpp,v 1.3 2001/02/28 15:06:03 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network/netgroup.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup_Generic:

class CL_NetGroup_Generic
{
public:
	std::list<CL_NetComputer> computers;
};

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup construction:

CL_NetGroup::CL_NetGroup()
: impl(NULL)
{
	impl = new CL_NetGroup_Generic;
}

CL_NetGroup::CL_NetGroup(const CL_NetComputer &computer)
: impl(NULL)
{
	impl = new CL_NetGroup_Generic;
	add(computer);
}

CL_NetGroup::~CL_NetGroup()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup attributes:

std::list<CL_NetComputer> &CL_NetGroup::get_computers() const
{
	return impl->computers;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup operations:

void CL_NetGroup::add(const CL_NetComputer &computer)
{
	impl->computers.push_back(computer);
}

void CL_NetGroup::remove(const CL_NetComputer &computer)
{
	impl->computers.remove(computer);
}

void CL_NetGroup::disconnect()
{
	std::list<CL_NetComputer>::iterator it;

	for (
		it = impl->computers.begin();
		it != impl->computers.end();
		it++)
	{
		CL_NetComputer &cur = *it;
		cur.disconnect();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup implementation:
