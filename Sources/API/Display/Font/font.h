/*
	$Id: font.h,v 1.12 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	File purpose:
		Simple font support.
*/

//! clanDisplay="Fonts"
//! header=display.h

#ifndef header_font
#define header_font

#include "../../Core/Resources/resource.h"

class CL_Target;
class CL_ResourceManager;
class CL_Font_Description;
class CL_Font_Generic;
class CL_Font_Generic;

//: ClanLib's font class.
//- <p>This class allows you to easilly draw text onto the backbuffer or to a
//- renderer target.</p>
//-
//- <p>The font can be created in two ways. Either it is loaded from a
//- resource, or it is created using the font description interface. The font
//- and its description have the same relationship/functionality as the
//- CL_Surface surface and its
//- CL_SurfaceProvider provider.</p>
//-
//- <p>Please read the CL_Font_Description page if you want an description on
//- how to create your own font.</p>
class CL_Font
{
public:
	//: Contains the alignment of the text.
	//: Doesn't need to be set and is used to hold and set the current alignment
	//: You can safely ignore this
	enum alignments
	{
		//: ALIGN_LEFT - Aligns the text left.
		ALIGN_LEFT,

		//: ALIGN_CENTER - Aligns the text centre.
		ALIGN_CENTER,

		//: ALIGN_RIGHT - Aligns the text right.
		ALIGN_RIGHT
	};

//! Static_init:
public:
	//: Creates the font from a font description.
	//- font_desc - The font description used to construct the font.
	static CL_Font *create(CL_Font_Description *font_desc);
	
	//: Loads the font from resources.
	//- resource_id - The resource ID of the font.
	//- resource_manager - The resource manager used to load the
	//-                    resources from.
	static CL_Font *load(
		const std::string &resource_id,
		CL_ResourceManager *resource_manager);

//! Construction:
public:
	//: Creates a font from a font provider/description
	CL_Font(CL_Font_Description *provider);

	//: Loads a font from a resource file.
	CL_Font(const std::string &resource_id, CL_ResourceManager *manager);

	//: Make a copy of an other font, but share the image data.
	CL_Font(const CL_Font &font);

	//: Font Destructor
	virtual ~CL_Font();

//! Operations:
public:
	//: Copy assignment operator.
	CL_Font &operator =(const CL_Font &copy);

	//: Returns the height of the font.
	//- Returns - Width of the font.
	int get_height();

	//: Return the width of 'text' if drawn with this font.
	//- text - Text string the width should be calculated from.
	//- Returns - Width of the text (in pixels).
	int get_text_width(const std::string &text);

	//: Return the width of character if drawn with this font.
	//- text - Character the width should be calculated from.
	//- Returns - Width of the character (in pixels).
	int get_char_width(const char character);
	
	//: Prints 'text' left aligned at coordinates ('x','y').
	//- x - X coordinate (in pixels) the text is drawn at.
	//- y - Y coordiante (in pixels) the text is drawn at.
	//- text - The text to be drawn with this font.
	//- n_height - Adds font height + n_height pixels to Y coordinate on new line ('\n').
	void print_left(int x, int y, const std::string &text, int n_height=1);

	//: Prints 'text' left aligned at coordinates ('x','y').
	//- x - X coordinate (in pixels) the text is drawn at.
	//- y - Y coordiante (in pixels) the text is drawn at.
	//- scale_x - Scale font width scale_x times.
	//- scale_y - Scale font height scale_y times.
	//- text - The text to be drawn with this font.
	void print_left(int x, int y, float scale_x, float scale_y, const std::string &text);
	
	//: Prints 'text' center aligned at coordinates ('x','y').
	//- x - X coordinate (in pixels) the text is centered at.
	//- y - Y coordiante (in pixels) the text is centered at.
	//- text - The text to be drawn with this font.
	//- n_height - Adds font height + n_height pixels to Y coordinate on new line ('\n').
	void print_center(int x, int y, const std::string &text, int n_height=1);

	//: Prints 'text' right aligned at coordinates ('x','y').
	//- x - X coordinate (in pixels) the text is ends at.
	//- y - Y coordiante (in pixels) the text is drawn at.
	//- text - The text to be drawn with this font.
	//- n_height - Adds font height + n_height pixels to Y coordinate on new line ('\n').
	void print_right(int x, int y, const std::string &text, int n_height=1);
	
	//: Prints 'text' aligned with alignment at coordinates ('x','y') to the target surface.
	//: If CL_Font::ALIGN_LEFT is specified the text is draw starting from x, y.
	//: If CL_Font::ALIGN_CENTER is specified the text is centered at x, y.
	//: If CL_Font::ALIGN_RIGHT is specified the text ends at x, y;
	//- x - X coordinate (in pixels)
	//- y - Y coordiante (in pixels)
	//- text - The text to be drawn with this font.
	//- target - the target surface to blit to.
	//- alignment - the alignment of the text, CL_Font::ALIGN_LEFT, CL_Font::ALIGN_CENTER, CL_Font::ALIGN_RIGHT
	void put_target(int x, int y, const std::string &text, CL_Target *target, int alignment = ALIGN_LEFT);

	//: Changes the size of the font used.
	//: This function only does something meaninful if you use TrueType fonts
	//- size - The new size to use.
	//- Returns -  Returns the size set or -1 if the font size cannot be changed to size
	int change_size(int size);

	//: Changes the colour of the font used.
	//- r - The red component
	//- g - The green component
	//- b - The blue component
	//- a - The alpha component
	//- Returns - Returns the font set in packed ARGB format.
	unsigned int change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

//! Implementation:
public:
	//: Font constructor
	CL_Font(class CL_Font_Generic *impl);

	//: The font generic implementation
	CL_Font_Generic *impl;

	//: Font resource, if any.
	CL_Resource resource;
};

#endif
