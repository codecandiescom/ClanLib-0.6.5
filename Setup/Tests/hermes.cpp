/*
	$Id: hermes.cpp,v 1.5 2002/01/09 09:57:31 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
/*
	Test for a working Hermes 1.3.x
*/

// need to comment out stdlib.h because of a fatal bug in autoconf. -- mbn 3. nov 2000
// #include <stdlib.h>

#include <Hermes/Hermes.h>
#include <Hermes/H_Conv.h>
#include <Hermes/H_Pal.h>
#include <Hermes/H_Blit.h>

int main(int, char**)
{
	return 0; // success
}

void used_stuff()
{
	unsigned char *ptr;
	int32 *pal;
	HermesHandle handle;
	HermesFormat *format;

	Hermes_Init();
	Hermes_Done();

	handle = Hermes_ConverterInstance(HERMES_CONVERT_NORMAL);
	handle = Hermes_PaletteInstance();
	handle = Hermes_BlitterInstance(HERMES_CONVERT_NORMAL);
	format = Hermes_FormatNew(1,2,3,4,5,6);

	Hermes_PaletteSet(handle, pal);

	Hermes_BlitterRequest(handle, format, format);
	Hermes_BlitterPalette(handle, handle, 0);
	Hermes_BlitterBlit(handle, ptr, 3, 4, 5, 6, 7, ptr, 9, 10, 11, 12, 13);

	Hermes_ConverterRequest(handle, format, format);
	Hermes_ConverterPalette(handle, handle, 0);
	Hermes_ConverterCopy(handle, ptr, 3, 4, 5, 6, 7, ptr, 9, 10, 11, 12, 13);

	Hermes_BlitterReturn(handle);
	Hermes_ConverterReturn(handle);
	Hermes_PaletteReturn(handle);
	Hermes_FormatFree(format);
}
