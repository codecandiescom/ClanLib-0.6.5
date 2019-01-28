/*
	$Id: pixelformat.h,v 1.6 2001/11/02 02:24:09 plasmoid Exp $

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

#ifndef header_pixelformat
#define header_pixelformat

class CL_Target;

//: Pixelformat types available.
enum EPixelFormat
{
//: Palettelized 8 bpp mode.
	PAL8,
	
//: Red-green-blue, using depth 565, 16 bpp.
	RGB565,
	
//: Red-green-blue-alpha, using depth 4444, 16 bpp.
	RGBA4444,

//: Red-green-blue-alpha, using depth 5551, 16 bpp.
	RGBA5551,

//: Red-green-blue, using depth 888, 24 bpp.
	RGB888,

//: Red-green-blue-alpha, using depth 8888, 32 bpp.
	RGBA8888
};

//: The Color class
//- <p>This class provides methods that deal with colors, 
//- including the ability to find color-component bit
//- masks for particular pixel formats. Also you can 
//- use this class to get colors, in any of the
//- different pixel formats,  from the color components 
//- (Red, Blue, Green and Alpha).</p>
//- <p>Often you can specify color components as either
//- integers or floating points (the component value 
//- divided by it's maximum possible value.)</p>
class CL_Color
{
public:
//! Attributes:
	//: Returns the bit mask for red using the given pixel format.
	static unsigned int get_red_mask(EPixelFormat pixelformat);

	//: Returns the bit mask for green using the given pixel format.
	static unsigned int get_green_mask(EPixelFormat pixelformat);

	//: Returns the bit mask for blue using the given pixel format.
	static unsigned int get_blue_mask(EPixelFormat pixelformat);

	//: Returns the bit mask for alpha using the given pixel format.
	static unsigned int get_alpha_mask(EPixelFormat pixelformat);

	//: Converts separate red, blue, green and alpha  integer values into a color value
	//- fmt - the pixel format to use
	//- r - the red value as an integer
	//- g - the green value as an integer
	//- b - the blue value as an integer
	//- a - the alpha value as an integer(defaults to opaque)
	//- Returns - the color value in the requested pixel format
	static unsigned int get_color(EPixelFormat fmt, int r, int g, int b, int a=255);

	//: <p>Converts separate red, blue, green and alpha floating
	//: point values into a single color value</p>
	//- fmt - the pixel format to use
	//- r - the red value as a floating point number
	//- g - the green value as a floating point number
	//- b - the blue value as a floating point number
	//- a - the alpha value as a floating point number (defaults to opaque)
	//- Returns - the color value in the requested pixel format
	static unsigned int get_color(EPixelFormat fmt, float r, float g, float b, float a=1.0);

	//: <p>Returns the color value in a format you specify
	//: by giving the red, green, blue and alpha
	//: masks, along with the corresponding red, blue,
	//: green and alpha values.</p>
	//- rmask - the bit mask for red
	//- gmask - the bit mask for green
	//- bmask - the bit mask for blue
	//- amask - the bit mask for the alpha
	//- r - the red value as an integer
	//- g - the green value as an integer
	//- b - the blue value as an integer
	//- a - the alpha value as an integer (default is opaque)
	//- Returns - the color value in the format you specify
	static unsigned int get_color(unsigned int rmask, 
				      unsigned int gmask, 
			              unsigned int bmask, 
			              unsigned int amask, 
			              int r, int g, int b, int a=255);

	//: <p>Returns the color value in a format you specify
	//: by giving the red, green, blue and alpha
	//: masks, along with the corresponding red, blue,
	//: green and alpha values.</p>
	//- rmask - the bit mask for red
	//- gmask - the bit mask for green
	//- bmask - the bit mask for blue
	//- amask - the bit mask for the alpha
	//- r - the red value as a floating point 
	//- g - the green value as a floating point 
	//- b - the blue value as a flaoting point
	//- a - the alpha value as a floating point (default is opaque)
	//- Returns - the color value in the format you specify
	static unsigned int get_color(unsigned int rmask, 
				      unsigned int gmask, 
			              unsigned int bmask, 
			              unsigned int amask, 
			              float r, float g, float b, float a=1.0);
	
	//: <p>Returns the color value for the specified
	//: red, blue and green integer components,
	//: in the  same pixel format as the target.
	//- target - a pointer to a target </p>
	//- r - the red value as an integer
	//- g - the green value as an integer
	//- b - the blue value as an integer
	//- a - the alpha value (default is opaque)
	static unsigned int get_color(CL_Target* target,
			              int r, int g, int b, int a=255);

	//: <p>Returns the color value for the specified 
	//: red, blue and green floating point components
	//: in the same pixel format as the target.</p>
	//- target - pointer to the target
	//- r - the red value as a floating point
	//- g - the green value as a floating point
	//- b - the blue value as a floating point
	static unsigned int get_color(CL_Target* target,
			              float r, float g, float b, float a=1.0);

	//: <p>Takes a color, extracts the color component 
	//: (red, green, blue or alpha) specified by cmask, and
	//: returns it as a floating point. (The floating point
	//: value is the amount of the color component divided by
	//: that color component's maximum value.) </p>
	//:  For example:
	//: <p>If the red component has a maximum value of 255,
	//: and the red component of your color was 200, the floating
	//: point returned would be  200/255 or 0.7843</p>
	//- cmask - the bit mask for the color component you want
	//- color - the color to extract the component from
	//- Returns - the R, G, B or A value as a floating point
	static float get_color(unsigned int cmask, int color);

};

#endif
