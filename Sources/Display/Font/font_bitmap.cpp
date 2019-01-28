/*
	$Id: font_bitmap.cpp,v 1.5 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Bitmap font class.

*/

#include "Core/precomp.h"
#include <API/Display/Font/font.h>
#include <API/Display/Font/font_description.h>
#include <API/Display/Display/surfaceprovider.h>
#include <API/Display/Display/surface.h>

#include "font_bitmap.h"

// tiptoe around windows.h
#ifndef max
#  define max(a,b) ((a)>(b)?(a):(b))
#endif

CL_Font_Bitmap::CL_Font_Bitmap(CL_Font_Description *font_desc)
{
	space_len = font_desc->space_len;
	subtract_width = font_desc->subtract_width;

	int len = font_desc->letters.length();
	for (int j=0; j<256; j++)
		chars[j] = NULL;

	font_desc->lock();
	for (int i=0; i<len; i++)
	{
		if (i == 0)
			height = font_desc->letter_providers[i]->get_height();
	
		chars[(unsigned char) font_desc->letters[i]] = CL_Surface::create(
			font_desc->letter_providers[i],
			false);
	}
	font_desc->unlock();
}

CL_Font_Bitmap::~CL_Font_Bitmap()
{
	for (int i=0; i<256; i++)
		if (chars[i]!=NULL)
			delete chars[i];
}

int CL_Font_Bitmap::get_height()
{
	return height;
}

int CL_Font_Bitmap::get_text_width(const std::string &_text)
{
	int len = 0, max_line_len = 0;

	unsigned char *text = (unsigned char *) _text.c_str();

	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			len += chars[*text]->get_width()-subtract_width;
		}
		else if (*text == '\n')
		{
			max_line_len = max(len, max_line_len);
			len = 0;
		}			
		else
		{
			len += space_len;
		}

		text++;
	}

	return max(len, max_line_len);
}

int CL_Font_Bitmap::get_line_width(const unsigned char *text)
{
	int len = 0;

	while (*text != 0 && *text != '\n')
	{
		if (chars[*text] != NULL)
			len += chars[*text]->get_width()-subtract_width;
		else
			len += space_len;

		text++;
	}

	return len;
}

int CL_Font_Bitmap::get_char_width(const char _character)
{
	unsigned char character = (unsigned char) _character;
	
	if (chars[character] != NULL)
	{
		return chars[character]->get_width()-subtract_width;
	}
	else
	{
		return space_len;
	}
}

inline int CL_Font_Bitmap::x_offset(const unsigned char *text, int alignment)
{
	switch(alignment)
	{
		case CL_Font::ALIGN_CENTER:
			return get_line_width(text)/2;
		case CL_Font::ALIGN_RIGHT:
			return get_line_width(text);
		default:
			return 0;
	}	
}

void CL_Font_Bitmap::print_generic(int x, int y, const std::string &_text, int n_height, int alignment)
{
	unsigned char *text = (unsigned char *) _text.c_str();
	int pos_x = x - x_offset(text, alignment);
	int pos_y = y;


	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			int width = chars[*text]->get_width()-subtract_width;

			chars[*text]->put_screen(pos_x, pos_y);

			pos_x += width;
		}
		else if (*text == '\n')
		{
			// new line on '\n'
	  		pos_x = x - x_offset(text+1, alignment);
			pos_y += (get_height() + n_height);
		}
		else
		{
			pos_x += space_len;
		}
		text++;
	}
}

void CL_Font_Bitmap::print_left(int x, int y, const std::string &_text, int n_height)
{
	print_generic(x,y,_text,n_height,CL_Font::ALIGN_LEFT);
}

void CL_Font_Bitmap::print_left(int x, int y, float sx, float sy, const std::string &_text)
{
	int pos_x = x;

	unsigned char *text = (unsigned char *) _text.c_str();

	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			int width = (int) ((chars[*text]->get_width()-subtract_width)*sx);

			chars[*text]->put_screen(pos_x, y, sx, sy);

			pos_x += width;
		}
		else
		{
			pos_x += (int) (space_len*sx);
		}

		text++;
	}
}

void CL_Font_Bitmap::print_center(int x, int y, const std::string &_text, int n_height)
{
	print_generic(x,y,_text,n_height,CL_Font::ALIGN_CENTER);
}

void CL_Font_Bitmap::print_right(int x, int y, const std::string &_text, int n_height)
{
	print_generic(x,y,_text,n_height,CL_Font::ALIGN_RIGHT);
}

void CL_Font_Bitmap::put_target(int x, int y, const std::string &_text, CL_Target *target, int alignment)
{
	int pos_x;
	unsigned char *text = (unsigned char *) _text.c_str();
		
	switch(alignment)
	{
		case CL_Font::ALIGN_CENTER:
			pos_x = x - get_text_width(_text)/2;
			break;
		case CL_Font::ALIGN_RIGHT:
			pos_x = x - get_text_width(_text);
			break;
		default:
			pos_x = x;
	}	
	
	while (*text != 0)
	{
		if (chars[*text] != NULL)
		{
			int width = chars[*text]->get_width()-subtract_width;

			chars[*text]->put_target(pos_x, y, 0, target);

			pos_x += width;
		}
		else
		{
			pos_x += space_len;
		}

		text++;
	}
}

