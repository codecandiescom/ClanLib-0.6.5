/*
	$Id: inverse_echofilter_generic.h,v 1.1 2001/03/06 15:09:26 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_inverse_echofilter_generic
#define header_inverse_echofilter_generic

#include "API/Sound/SoundFilters/inverse_echofilter.h"

class CL_InverseEchoFilter_Generic
{
public:
	int *buffer;
	int buffer_size;
	int pos;
};

#endif
