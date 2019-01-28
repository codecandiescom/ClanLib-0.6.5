/*
	$Id: math.h,v 1.3 2001/09/06 18:52:05 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

#ifndef header_math
#define header_math

#include <math.h>

#ifndef PI
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795	// maybe more accuracy than needed ;-)
#endif
#define PI M_PI
#endif

#endif
