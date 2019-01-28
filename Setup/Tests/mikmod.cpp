/*
	$Id: mikmod.cpp,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
/*
	Test wether MikMod library is correctly installed.
*/

#include <mikmod.h>
#undef class

int main(int, char**)
{
	int libmikmod_major_version = (MikMod_GetVersion() >> 16) & 255;
	int libmikmod_minor_version = (MikMod_GetVersion() >>  8) & 255;
	int libmikmod_micro_version = (MikMod_GetVersion()      ) & 255;

	return 0;
}
