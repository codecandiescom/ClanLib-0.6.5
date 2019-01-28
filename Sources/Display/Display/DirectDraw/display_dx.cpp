/*
	$Id: display_dx.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Display/Display/display.h"

CL_DisplayCard *CL_Display::get_current_card()
{
	if (CL_Display::cards.empty()) return NULL;

	return CL_Display::cards[0];
}
