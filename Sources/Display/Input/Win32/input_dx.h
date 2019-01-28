/*
	$Id: input_dx.h,v 1.2 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_input_dx
#define header_input_dx

class CL_Input_DX
{
public:
	static LPDIRECTINPUT dinput;

	CL_Input_DX();
	virtual ~CL_Input_DX();
};

#endif