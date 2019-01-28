/*
	$Id: png.cpp,v 1.2 2001/09/08 19:24:17 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
/*
	Test wether PNG library is correctly installed.
*/

#include <png.h>
#undef class

int main(int, char**)
{
	int png_version = PNG_LIBPNG_VER;
	if(png_version < 10000)
		return 1;
	return 0;
}
