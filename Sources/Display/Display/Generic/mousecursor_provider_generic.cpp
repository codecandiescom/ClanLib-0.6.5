/*
	$Id: mousecursor_provider_generic.cpp,v 1.2 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "mousecursor_provider_generic.h"

CL_MouseCursorProvider *CL_MouseCursorProvider::create(
	CL_SurfaceProvider *provider,
	bool delete_provider,
	bool animate,
	const std::vector<int> *frame_delays)
{
	return new CL_MouseCursorProvider_Generic(provider, delete_provider, animate, frame_delays);
}

CL_MouseCursorProvider *CL_MouseCursorProvider::load(
	const std::string &resource_id,
	CL_ResourceManager *manager)
//	bool animate,
//	const std::vector<int> *frame_delays)
{
	return new CL_MouseCursorProvider_Generic(
		CL_SurfaceProvider::load(resource_id, manager),
		true/*,
		animate,
		frame_delays*/);
}

CL_MouseCursorProvider_Generic::CL_MouseCursorProvider_Generic(
	CL_SurfaceProvider *provider,
	bool delete_provider,
	bool animate,
	const std::vector<int> *frame_delays)
{
	m_surface_provider = provider;
	m_delete_provider = delete_provider;
	m_animated = animate;
	if (frame_delays != NULL) m_frame_delays = *frame_delays;
}

CL_MouseCursorProvider_Generic::~CL_MouseCursorProvider_Generic() 
{ 
	if (m_delete_provider) delete m_surface_provider;
}

CL_SurfaceProvider *CL_MouseCursorProvider_Generic::get_surface_provider()
{
	return m_surface_provider;
}

bool CL_MouseCursorProvider_Generic::is_animated()
{
	return m_animated;
}

int CL_MouseCursorProvider_Generic::get_frame_delay(unsigned int frame_number)
{
	if (frame_number < m_frame_delays.size())
	{
		return m_frame_delays[frame_number];
	}
	else
	{
		return -1;
	}
}
