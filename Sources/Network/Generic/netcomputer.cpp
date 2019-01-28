/*
	$Id: netcomputer.cpp,v 1.9 2001/11/12 20:32:32 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "API/Network/netcomputer.h"
#include "API/Core/System/error.h"
#include "netcomputer_generic.h"

#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer construction:

CL_NetComputer::CL_NetComputer(const CL_NetComputer &copy)
: impl(NULL)
{
	impl = copy.impl;
	if (impl != NULL) impl->add_ref();
}

CL_NetComputer::CL_NetComputer()
: impl(NULL)
{
}

CL_NetComputer::~CL_NetComputer()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer attributes:

unsigned long CL_NetComputer::get_address() const
{
	if (impl == NULL) throw CL_Error("CL_NetComputer object isn't attached!");
	return impl->get_address();
}

CL_NetSession CL_NetComputer::get_session()
{
	if (impl == NULL) throw CL_Error("CL_NetComputer object isn't attached!");
	return CL_NetSession(impl->get_session());
}

bool CL_NetComputer::operator == (const CL_NetComputer &other) const
{
	return impl == other.impl;
}

bool CL_NetComputer::operator != (const CL_NetComputer &other) const
{
	return impl != other.impl;
}

bool CL_NetComputer::operator < (const CL_NetComputer &other) const
{
	if (impl == other.impl) return false;
	if (impl == NULL) return true;
	if (other.impl == NULL) return false;

	unsigned long addr1 = ntohl(impl->get_address());
	unsigned long addr2 = ntohl(other.impl->get_address());

	if (addr1 == addr2) return impl->get_port() < other.impl->get_port();
	return addr1 < addr2;
}

bool CL_NetComputer::operator > (const CL_NetComputer &other) const
{
	if (impl == other.impl) return false;
	if (impl == NULL) return false;
	if (other.impl == NULL) return true;

	unsigned long addr1 = ntohl(impl->get_address());
	unsigned long addr2 = ntohl(other.impl->get_address());

	if (addr1 == addr2) return impl->get_port() > other.impl->get_port();
	return addr1 > addr2;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer operations:

void CL_NetComputer::disconnect()
{
	if (impl == NULL) throw CL_Error("CL_NetComputer object isn't attached!");
	impl->disconnect();
}

CL_NetComputer &CL_NetComputer::operator = (const CL_NetComputer &other_instance)
{
	if (impl != NULL) impl->release_ref();
	impl = other_instance.impl;
	if (impl != NULL) impl->add_ref();

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer implementation:

CL_NetComputer::CL_NetComputer(class CL_NetComputer_Generic *impl)
: impl(impl)
{
	if (impl != NULL) impl->add_ref();
}
