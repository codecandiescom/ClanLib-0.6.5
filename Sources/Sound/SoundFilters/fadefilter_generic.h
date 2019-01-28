/*
	$Id: fadefilter_generic.h,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_fadefilter_generic
#define header_fadefilter_generic

#include "API/Sound/SoundFilters/fadefilter.h"

class CL_FadeFilter_Generic
{
public:
	float cur_volume, new_volume;
	float speed;
};

#endif
