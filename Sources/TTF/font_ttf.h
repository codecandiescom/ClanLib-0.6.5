/*
	$Id: font_ttf.h,v 1.17 2002/01/15 16:18:48 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		TrueType Font support through the FreeType library
*/

#ifndef header_font_tt
#define header_font_tt

#include <Display/Font/font_generic.h>

struct font_adj
{
	int top;
	int left;
};

class CL_InputSourceProvider;
class CL_InputSource;
class CL_String;
class CL_Canvas;
class CL_Surface;
class CL_Font_Generic;

class CL_Font_TTF : public CL_Font_Generic
{
 public:
  CL_Font_TTF(CL_String name, CL_InputSourceProvider* provider);
  virtual ~CL_Font_TTF();

  int get_height() {return height;}
  int get_text_width(const std::string &text);
  int get_char_width(const char character);

  void print_left(int x, int y, const std::string &text, int n_height=1);
  void print_left(int x, int y, float scale_x, float scale_y, const std::string &text);
  void print_center(int x, int y, const std::string &text, int height);
  void print_right(int x, int y, const std::string &text, int height);

  void put_target(int x, int y, const std::string &text, CL_Target *target, int alignment);
  int change_size(int size);
  unsigned int change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

 protected:
  friend class CL_Font_Resource_TTF;
  void generate_font();

  int num_faces; //How many font faces in this font pkg
  int num_glyphs; //How many character images in this face
  unsigned int flags; //Describes Font_face properties
  int num_fixed_sizes; //The number of unique sizes for the font face

  CL_InputSourceProvider *input_provider;
  CL_InputSource *input_source;
  CL_String filename;

  CL_Surface *chars[256];
  font_adj adjust[256]; //One for each letter
  bool chars_init;

  int width;
  int height;
  int space_len;
  int subtract_width;

  unsigned char* font_mem;
  int *font_sizes;
  int size;

  unsigned char r,g,b,a;


};

#endif
