/*
	$Id: palette.cpp,v 1.2 2001/11/02 02:24:09 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <API/Display/Display/palette.h>
#include <API/Core/IOData/inputsource_provider.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/IOData/inputsource.h>

using namespace std;

CL_Palette::CL_Palette()
{
	num_colors = 256;
	palette = new unsigned char[num_colors*3];
}

CL_Palette::CL_Palette(char *palette_id, CL_InputSourceProvider *provider)
{
	CL_InputSource *source = provider->open_source(palette_id);
	cl_assert(source != NULL);

	num_colors = source->read_int32();
	cl_assert(num_colors > 0 && num_colors < (256*256));

	palette = new unsigned char[num_colors*3];
	source->read(palette, num_colors*3);

	delete source;
}

CL_Palette::CL_Palette(CL_InputSource *source)
{
	cl_assert(source != NULL);

	num_colors = source->read_int32();
	cl_assert(num_colors > 0 && num_colors < (256*256));

	palette = new unsigned char[num_colors*3];
	source->read(palette, num_colors*3);
}

CL_Palette::CL_Palette(unsigned char *_palette, int _num_colors)
{
	num_colors = _num_colors;
	palette = new unsigned char[num_colors*3];
	memcpy(palette, _palette, num_colors*3);
}

CL_Palette::~CL_Palette()
{
	delete[] palette;
}
