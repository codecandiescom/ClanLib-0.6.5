/*
	$Id: mousecursor_provider_generic.h,v 1.3 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_mousecursorprovider_generic
#define header_mousecursorprovider_generic

#include <vector>
#include "API/Display/Display/mousecursor_provider.h"
#include "API/Display/Display/surfaceprovider.h"


class CL_MouseCursorProvider_Generic : public CL_MouseCursorProvider
{
public:
	CL_MouseCursorProvider_Generic(
		CL_SurfaceProvider *provider,
		bool delete_provider=true,
		bool animate=true,
		const std::vector<int> *frame_delays=NULL);
	virtual ~CL_MouseCursorProvider_Generic();

	virtual CL_SurfaceProvider *get_surface_provider();
	virtual bool is_animated();
	virtual int get_frame_delay(unsigned int frame_number);

private:
	CL_SurfaceProvider *m_surface_provider;
	bool m_delete_provider;
	bool m_animated;
	std::vector<int> m_frame_delays;
};

#endif

