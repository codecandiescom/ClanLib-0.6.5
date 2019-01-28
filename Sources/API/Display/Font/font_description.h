/*
	$Id: font_description.h,v 1.7 2002/03/02 16:43:47 plasmoid Exp $

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

#ifndef header_font_description
#define header_font_description

#include <vector>
#include <string>

class CL_SurfaceProvider;
class CL_InputSource;

//: Font provider interface.
//- <p>The font description class is used by the CL_Font" font
//- class to load the font.</p>
//-
//- Currently ClanLib support two types of fonts:
//-
//- The old method.
//-
//- <p>You may find some 256 color indexed fonts with some of ClanLib's
//- example applications. They use the three last colors in the palette to
//- seperate the letters, break the line, and to determine the size of the
//- font. The method is deprecated.</p>
//-
//- The new method.
//-
//- <p>The font description class constructs the font letters from an image
//- with alpha values. Each letter is seperated with transparent lines and
//- the font description class uses this information to determine where it so
//- seperate the letters.</p>
//-
//- <p>CL_Font_Description allows you to adjust how much transparent a line
//- must be before it consideres it as a letter seperator line. The
//- trans_limit parameter handles this.</p>
//-
//- <p>If your font is italic you can use the subtract_width parameter to
//- move the printed letters "closer to each other". This is, with a fine
//- word, called the kerning of the font.</p>
class CL_Font_Description
{
public:
//! Variables:
	//: Letter Providers
	std::vector<CL_SurfaceProvider*> letter_providers;

	//: Space length
	int space_len;

	//: Subtract Width
	int subtract_width;

	//: Letters
	std::string letters;
	
//! Construction:
	//: <p>This constructor is used to load the font from an input source.
	//: Usually this from a resource datafile.</p>
	//:
	//: <p>Normally you do not use this constructor directly. It is much
	//: easier to use CL_Font::load() when retrieving a font from a
	//: datafile.</p>
	CL_Font_Description(CL_InputSource *input);
	
	//: <p>This constructs a font description using the old font method. It
	//: is obsolete and only provided for backward compatibility. Please
	//: don't use it.</p>
	//- p - Image which contain the font letters.
	//- x - Start x coordinate on top-left most font letter.
	//- y - Start y coordinate on top-left most font letter.
	//- tcols - Array of colors to be transparent.
	//- num_tcols - Number of elements in the tcols array.
	//- spacelen - Pixel width of the space character.
	//- subtract_width - Width to subtract from each letter when drawing the font (aka. font kerning).
	//- letters - String containing all the letters listed in the image file. Must be ordered in the same way as in the image file.
	CL_Font_Description(
		CL_SurfaceProvider *p,
		int x, int y,
		int *tcols, int num_tcols,
		int spacelen,
		int subtract_width,
		const std::string &letters);

	//: <p>This constructs a font description using the new alpha cutter
	//: method.</p>
	//: <p>Please have a look on the class overview reference page for more
	//: information.</p>
	//- source - Image which contain the font letters.
	//- trans_limit - The maximum amount of alpha allowed before a pixel is no longer considered transparent.
	//- space_len - Pixel width of the space character.
	//- subtract_width - Width to subtract from each letter when drawing the font (aka. font kerning).
	//- letters - String containing all the letters listed in the image file. Must be ordered in the same way as in the image file.
	CL_Font_Description(
		CL_SurfaceProvider *source,
		float trans_limit,
		int space_len,
		int subtract_width,
		const std::string &letters);

	//: This is primarily used by the TTF resource loader to handle fonts
	//- letters - A vector array of pointers to the letters themselves
	//- space_len - Pixel width of the space character
	//- subtract_width - Width to subtract from each letter when drawing the font (aka. font kerning).
	//- letters - String containing all the letters listed in the image file. Must be ordered in the same way as in the image file.
	CL_Font_Description(
		std::vector<CL_SurfaceProvider*> letter_provider,
		int space_len,
		int subtract_width,
		std::string letters);

	//: Standard destructor for cleanup
	~CL_Font_Description();

//! Operations:
	//: Lock
	void lock();

	//: Unlock
	void unlock();

private:
	CL_SurfaceProvider *source;
//	std::vector<CL_SurfaceProvider*> letter_providers;
};

#endif
