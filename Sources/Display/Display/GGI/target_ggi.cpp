/*
	$Id: target_ggi.cpp,v 1.2 2001/03/17 23:53:24 vaidhy Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI

#include <Display/Display/GGI/target_ggi.h>

CL_Target_GGI::CL_Target_GGI(ggi_visual_t vis)
{
	m_vis = vis;
        ggiGetMode( m_vis, &m_mode );
}

CL_Target_GGI::~CL_Target_GGI()
{
}

#endif
