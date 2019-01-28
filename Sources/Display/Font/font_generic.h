/*
	$Id: font_generic.h,v 1.6 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Generic font support.
*/

#ifndef header_font_generic
#define header_font_generic

class CL_Target;

class CL_Font_Generic
{
public:
	CL_Font_Generic();
	virtual ~CL_Font_Generic();

	int add_reference();
	int release_reference();

	virtual int get_height() = 0;
	virtual int get_text_width(const std::string &text) = 0;
	virtual int get_char_width(const char character) = 0;
	
	virtual void print_left(int x, int y, const std::string &text, int n_height=1) = 0;
	virtual void print_left(int x, int y, float scale_x, float scale_y, const std::string &text) = 0;
	virtual void print_center(int x, int y, const std::string &text, int n_height=1) = 0;
	virtual void print_right(int x, int y, const std::string &text, int n_height=1) = 0;
	
	virtual void put_target(int x, int y, const std::string &text, CL_Target *target, int alignment) = 0;
	virtual int change_size(int size) = 0;

	virtual unsigned int change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

private:
	int ref_count;
};

#endif

