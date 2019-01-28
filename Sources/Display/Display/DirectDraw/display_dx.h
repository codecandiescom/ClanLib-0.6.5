/*
	$Id: display_dx.h,v 1.3 2001/11/29 13:07:59 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_display_dx
#define header_display_dx

#include "API/Display/Display/display.h"
#include "displaycard_directdraw.h"

class CL_Display_DX : public CL_Display
{
public:
	CL_Display_DX(int options);
	virtual ~CL_Display_DX() { ; }

	static CL_DisplayCard_DirectDraw *cur_card;
};

#endif
