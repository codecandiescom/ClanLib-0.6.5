/*
	$Id: smalljpeg.h,v 1.4 2002/03/01 22:00:16 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_smalljpeg
#define header_smalljpeg

#include "SmallJPEG/jpgd_provider.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanSmallJPEG.lib")
#else
#pragma comment(lib, "clanSmallJPEGd.lib")
#endif
#endif

#endif
