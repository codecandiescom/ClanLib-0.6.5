/*
	$Id: palette.h,v 1.5 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_palette
#define header_palette

class CL_InputSourceProvider;
class CL_InputSource;

//: Palette class.
//- CL_Palette represents a palette in the form of a 256 byte array.
//-
//- <p>Palettes are in ClanLib only used by surface providers. A provider
//- specifies it's palette by returning a CL_Palette. But It can also choose
//- to return NULL. In that case it will use the system palette. You can
//- change the system palette from CL_Display or CL_DisplayCard.</p>
//- See Also - CL_Display - The system palette is set here.
//- See Also - CL_DisplayCard - The card specific system palette is set here.
//- See Also - CL_SurfaceProvider - Palettelized surfaces use this class.
class CL_Palette
{
public:
//! Variables:
	//: The actual palette. 1 byte for each color component (rgb), 256 colors.
	unsigned char *palette;

	//: Number of colours in the palette.
	int num_colors;

public:
//! Construction:
	//: Creates a palette.
	CL_Palette();
	
	//: Palette destructor
	virtual ~CL_Palette();

	//: Creates a palette using values from an unsigned char array.
	//- palette - palette source.
	//- num_colors - number of palette colors.
	CL_Palette(unsigned char *palette, int num_colors=256);

	//: Creates a palette from a input source.
	//- palette_id - ID used to get palette from provider.
	//- provider - Input source type (eg. a datafile or a normal file).
	CL_Palette(char *palette_id, CL_InputSourceProvider *provider);

	//: Creates a palette from an already open input source.
	//- input - Input source to read from.
	CL_Palette(CL_InputSource *input);
};

#endif
