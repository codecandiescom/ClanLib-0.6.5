/*
	$Id: font_ttf.cpp,v 1.29 2002/01/15 16:18:48 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"
#include <API/Core/IOData/inputsource_provider.h>
#include <API/Core/IOData/inputsource.h>
#include <API/Core/System/clanstring.h>
#include <API/Display/SurfaceProviders/canvas.h>
#include <API/Display/Display/surface.h>
#include <API/Display/Font/font.h>
#include <malloc.h>
#include "Display/Font/font_generic.h"

#include <freetype/freetype.h>

#include "font_ttf.h"

CL_Font_TTF::CL_Font_TTF(CL_String name, CL_InputSourceProvider* provider)
{
	// Init the class
	filename = name;
	font_mem = NULL;
	font_sizes = NULL;
	chars_init = false;
	a = 255;
	r = b = g = 0;

	// Ensure the proper target to load a TTF file
	if(provider != NULL)
		input_provider = provider->clone();
	else 
		input_provider = CL_InputSourceProvider::create_file_provider(".");

	// Open the font
	input_source = input_provider->open_source(filename);

	// Allocate memory for the font
	font_mem = (unsigned char*)malloc(input_source->size());
	size = input_source->size();
	if(font_mem == NULL)
		throw CL_Error("Out of Memory");
	memset((void*)font_mem, 0, size);

	// Buffered to the size of the font+1 to prevent overflow
	input_source->read(font_mem, size);

	height = 12; // Freetype defaults to font size 10 if it uses scaleable fonts
	width = 12;
	space_len = 10;
	subtract_width = 0; // I dunno about this one

  delete input_provider;
	generate_font();
}

void CL_Font_TTF::generate_font() 
{
	if(chars_init)
	{
  		for(int i=0;i<num_glyphs;i++)
			if (chars[i]) delete chars[i];
			//if the fonts exist we must delete them individually
	}
	FT_Library library;
	FT_Face face;
	int error=0;

	// Initialize 'library' as a freetype library.
	error = FT_Init_FreeType( &library );
	if (error != 0)
		throw(CL_Error("Cannot initialize the FreeType Library"));

	// Turn 'face' into a handle to the font library stored in memory and use 'library' as the library handle
	error = FT_New_Memory_Face(library, font_mem, size, 0, &face);
	// This loads 'library' with the font at filename with the font face index 0(always valid).

	if( error == FT_Err_Unknown_File_Format)
		throw CL_Error("I can't understand what the file format is.");
	else if (error != 0)
		throw CL_Error("I can't access the font file or the font may be broken");

	num_faces = face->num_faces; // Total number of available faces
	num_fixed_sizes = face->num_fixed_sizes; // Fixed sizes for non-scallable fonts
	num_glyphs = face->num_glyphs; // Number of characters

	if(num_fixed_sizes>0)
	{
		font_sizes = new int[num_fixed_sizes];
		for(int i=0;i<num_fixed_sizes;i++)
			font_sizes[i] = face->available_sizes[i].height;

		height = font_sizes[0];
		width = font_sizes[0];
	}

	FT_Set_Char_Size(face,height<<6,width<<6,72,72);

	CL_Canvas *canvas=NULL;

	int index;

	for(int i=0;i<num_glyphs;i++)
	{
		index = FT_Get_Char_Index(face,i);

		error = FT_Load_Glyph(face,index,FT_LOAD_DEFAULT);

		if(error!=0)
			continue;

		error = FT_Render_Glyph(face->glyph,ft_render_mode_normal );

		if(error!=0)
			continue;

		adjust[i].top = face->glyph->bitmap_top;
		adjust[i].left = face->glyph->bitmap_left;

		canvas = new CL_Canvas(face->glyph->bitmap.width , face->glyph->bitmap.rows);
		canvas->lock();

		unsigned char *buffer =(unsigned char*) canvas->get_data();
		unsigned char *bmp = (unsigned char*)face->glyph->bitmap.buffer;

		for(int j=0;j<(face->glyph->bitmap.rows*face->glyph->bitmap.width);j++)
		{
			*buffer = (*bmp) & a;
			*(buffer+1) = (*bmp) & b;
			*(buffer+2) = (*bmp) & g;
			*(buffer+3) = (*bmp) & r;
			bmp++;
			buffer+=4;
		}

		canvas->unlock();
		chars[i] = CL_Surface::create(canvas,true);//CL_Surface::create(canvas);
	}

	chars_init = true;
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

CL_Font_TTF::~CL_Font_TTF()
{
	free((void*)font_mem);
	if(chars_init)
	{
  		for(int i=0;i<num_glyphs;i++)
				if (chars[i]) delete chars[i];
	}

	delete[] font_sizes;
  if(input_source)
  {
  	delete input_source;
    input_source = NULL;
  }
}

int CL_Font_TTF::get_text_width(const std::string &_text)
{
	const char *text = _text.c_str();
	int width=0;
	while(*text != 0) 
	{
		width += chars[*text]->get_width()-subtract_width;
		text++;
	}
	return width;
}

int CL_Font_TTF::get_char_width(const char character)
{
	return (chars[character]->get_width());
}

void CL_Font_TTF::print_left(int x, int y, const std::string &_text, int n_height)
{
	int pos_x = x;
	const unsigned char *text = (unsigned char*) _text.c_str();
	while(*text!=0)
	{
		if(*text == '\n')
		{
			x = pos_x; 
			y += (get_height() + n_height);
			text++;
			continue;
		}
		if (chars[*text] != NULL)
		{
			int width = chars[*text]->get_width()-subtract_width;

			chars[*text]->put_screen(x + adjust[*text].left, y-adjust[*text].top);

			x += width;
			if(*text == 0x20) // Space
				x += space_len;
		}
		else
			x += space_len;
		text++;
  }
}


void CL_Font_TTF::print_left(int x, int y, float scale_x, float scale_y, const std::string &_text)
{
	const char *text = _text.c_str();
	int pos_x = x;

	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			int width = (int) ((chars[*text]->get_width()-subtract_width)*scale_x);

			chars[*text]->put_screen((int)(pos_x+(adjust[*text].left*scale_y)), (int)(y-(adjust[*text].top*scale_y)));

			pos_x += width;
		}
		else
		{
			pos_x += (int) (space_len*scale_x);
		}

		text++;
	}
}

void CL_Font_TTF::print_center(int x, int y, const std::string &text, int n_height)
{
	print_left(x - get_text_width(text)/2, y, text); 
}

void CL_Font_TTF::print_right(int x, int y, const std::string &text, int n_height)
{
	print_left(x - get_text_width(text), y, text);
}

void CL_Font_TTF::put_target(int x, int y, const std::string &_text, CL_Target *target, int alignment)
{
	const char *text = _text.c_str();
	int pos_x;
		
	switch(alignment)
	{
		case CL_Font::ALIGN_CENTER:
			pos_x = x - get_text_width(text)/2;
			break;
		case CL_Font::ALIGN_RIGHT:
			pos_x = x - get_text_width(text);
			break;
		default:
			pos_x = x;
	}	
	
	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			int width = chars[*text]->get_width()-subtract_width;

			chars[*text]->put_target(pos_x+adjust[*text].left, y-adjust[*text].top, 0, target);

			pos_x += width;
		}
		else
		{
			pos_x += space_len;
		}

		text++;
	}
}

int CL_Font_TTF::change_size(int size)
{
	if(num_fixed_sizes>0)
	{
		for(int i=0;i<num_fixed_sizes;i++)
		{
			if(font_sizes[i] == size)
			{
				width = size;
				height = size;
				generate_font();
				return size;
			}
		}
		return -1;
	}
	else
	{
		width = size;
		height = size;

		try
		{
			generate_font();
		}
		catch(...) {return -1;}
    
		return size;
	}
}

unsigned int CL_Font_TTF::change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	generate_font();
	return (r | (g<<8) | (b<<16) | (a<<24));
}
