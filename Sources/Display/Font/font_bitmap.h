/*
	$Id: font_bitmap.h,v 1.6 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Bitmap font support.
*/

#ifndef header_font_bitmap
#define header_font_bitmap

#include "font_generic.h"

class CL_Surface;
class CL_Font_Description;
class CL_Target;

class CL_Font_Bitmap : public CL_Font_Generic
{
protected:
	CL_Surface *chars[256];
	int space_len;
	int subtract_width;
	int height;

public:
	CL_Font_Bitmap(CL_Font_Description *font_desc);

	virtual ~CL_Font_Bitmap();

	virtual int get_height();
	virtual int get_text_width(const std::string &text);
	virtual int get_char_width(const char character);
	
	virtual void print_left(int x, int y, const std::string &text, int n_height=1);
	virtual void print_left(int x, int y, float scale_x, float scale_y, const std::string &text);
	virtual void print_center(int x, int y, const std::string &text, int n_height=1);
	virtual void print_right(int x, int y, const std::string &text, int n_height=1);
	
	virtual void put_target(int x, int y, const std::string &text, CL_Target *target, int alignment);
	virtual int change_size(int size) {return -1;}
	unsigned int change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {return (unsigned int)-1;}

private:
	void print_generic(int x, int y, const std::string &text, int n_height, int alignment);
	// returns width of first line in text
	int get_line_width(const unsigned char *text); 
	int x_offset(const unsigned char *text, int alignment);
};

#endif

