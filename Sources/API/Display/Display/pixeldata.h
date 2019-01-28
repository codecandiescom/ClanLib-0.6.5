/*
	$Id: pixeldata.h,v 1.8 2001/11/01 13:56:50 sphair Exp $

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

#ifndef header_pixeldata
#define header_pixeldata

#include <iostream>
#include <Hermes/H_Conv.h>
#include <Hermes/H_Pal.h>

#include "surfaceprovider_32bpp.h"

//: Pixel Data Class
class CL_PixelData
{
protected:
	HermesHandle m_handle;

	HermesFormat *m_src_format;
	HermesHandle m_src_palette;

	HermesFormat *m_dest_format;
	
	CL_SurfaceProvider *m_provider;
	bool m_delete_provider;

	unsigned char *scanline;

public:
//! Construction:
	//: PixelData Constructor
	CL_PixelData(
		unsigned int red_mask,
		unsigned int green_mask,
		unsigned int blue_mask,
		unsigned int alpha_mask,
		CL_SurfaceProvider *src,
		unsigned int bytes_per_pixel)
	{
		m_delete_provider = false;

		m_provider = src;
	
		// Access surface provider until we're destructed.
		m_provider->lock();
		
		// if PAL8, convert to RGBA8888 because Hermes doesn't support
		// colorkeys.
		if (src->get_depth() == 8)
		{
			m_provider = new CL_SurfaceProvider_32bpp(src);
			m_provider->lock();
			if (m_delete_provider) delete src;
			m_delete_provider = true;
		}

		m_handle = Hermes_ConverterInstance(HERMES_CONVERT_NORMAL);
		cl_assert(m_handle!=0);

		m_src_palette = Hermes_PaletteInstance();
		cl_assert(m_src_palette != 0);

		// Set dest pixel format:
		
		m_dest_format = Hermes_FormatNew(
			bytes_per_pixel*8,
			red_mask,
			green_mask,
			blue_mask,
			alpha_mask,
			0 /* indexed */);
		cl_assert(m_dest_format != 0);

		// only PAL8 colorkey support (for now):
		// dv: 05.12 23:53 removed below
		//cl_assert(m_provider->uses_src_colorkey() == false);

		// Set source pixel format:

		m_src_format = Hermes_FormatNew(
			m_provider->get_depth(),
			m_provider->get_red_mask(),
			m_provider->get_green_mask(),
			m_provider->get_blue_mask(),
			m_provider->get_alpha_mask(),
			0 /* indexed */);
		cl_assert(m_src_format != 0);

		scanline = new unsigned char[get_bytes_per_pixel()*(get_width())];
	}

	//: PixelData Destructor
	virtual ~CL_PixelData()
	{
		Hermes_ConverterReturn(m_handle);
		Hermes_PaletteReturn(m_src_palette);

		Hermes_FormatFree(m_src_format);
		Hermes_FormatFree(m_dest_format);

		delete[] scanline;

		// Release surface provider.
		m_provider->unlock();
		if (m_delete_provider) delete m_provider;
	}

//! Attributes:
	//: Get width
	unsigned int get_width()
	{
		return m_provider->get_width();
	}

	//: Get height
	unsigned int get_height()
	{
		return m_provider->get_height();
	}
	
	//: Get bytes per pixel
	unsigned int get_bytes_per_pixel()
	{
		return (m_dest_format->bits+7)/8;
	}

	//: Get bytes per line
	int get_bytes_per_line() { return get_bytes_per_pixel()*get_width(); }
	
	//: Get line pixel to dest
	void get_line_pixel_to_dest(int y, unsigned char *dest)
	{
		// Hack to support alpha when Hermes doesn't:

		if ((unsigned int) y >= m_provider->get_height()*m_provider->get_num_frames())
		{
//			std::cout<< "Tried to obtain line " << y << std::endl;
			cl_assert(false);
		}

		int width = get_width();

		if (m_dest_format->a == 255 && get_bytes_per_pixel()==1)
		{
			if (m_src_format->a == 0)
			{
				memset(dest, 255, width);
				return;
			}
			switch (m_src_format->bits)
			{
			case 15:
			case 16:
				{
					unsigned short *ptr =
						(unsigned short *) m_provider->get_data();

					for (int x=0; x<width; x++)
					{
						dest[x] = (unsigned char) (ptr[x+y*width]&255);
					}
				}
				return;

			case 32:
				{
					unsigned int *ptr =
						(unsigned int *) m_provider->get_data();

					for (int x=0; x<width; x++)
					{
						dest[x] = (unsigned char) (ptr[x+y*width]&255);
					}
				}
				return;
			}
		}
	
	
		int res = Hermes_ConverterRequest(
			m_handle,
			m_src_format,
			m_dest_format);

		if (res == 0)
		{
			std::cout <<"Hermes ConverterRequest FAILED:" << std::endl;
			dump_assert_info(y);
			cl_assert(res != 0);
		}

		res = Hermes_ConverterPalette(m_handle, m_src_palette, 0);
		if (res == 0)
		{
			std::cout <<"Hermes ConverterPalette FAILED:" << std::endl;
			dump_assert_info(y);
			cl_assert(res != 0);
		}

		res = Hermes_ConverterCopy(
			m_handle,
			((unsigned char *) m_provider->get_data())+m_provider->get_pitch()*y,
			0,
			0,
			width,
			1, // m_provider->get_height(),
			m_provider->get_pitch(),
			dest,
			0,
			0,
			width,
			1, // m_provider->get_height(),
			get_bytes_per_pixel()*width
			);

		if (res == 0)
		{
			std::cout <<"Hermes ConverterCopy FAILED:" << std::endl;
			dump_assert_info(y);
			cl_assert(res != 0);
		}
	}

	//: Dump assert info
	void dump_assert_info(int line)
	{
		std::cout <<"  m_src_format->r = " << (unsigned int) m_src_format->r <<std::endl;
		std::cout <<"  m_src_format->g = " << (unsigned int) m_src_format->g <<std::endl;
		std::cout <<"  m_src_format->b = " << (unsigned int) m_src_format->b <<std::endl;
		std::cout <<"  m_src_format->a = " << (unsigned int) m_src_format->a <<std::endl;
		std::cout <<"  m_src_format->bits = " << (unsigned int) m_src_format->bits <<std::endl;
		std::cout <<"  m_src_format->indexed = " << (unsigned int) m_src_format->indexed <<std::endl <<std::endl;

		std::cout <<"  m_dest_format->r = " << (unsigned int) m_dest_format->r <<std::endl;
		std::cout <<"  m_dest_format->g = " << (unsigned int) m_dest_format->g <<std::endl;
		std::cout <<"  m_dest_format->b = " << (unsigned int) m_dest_format->b <<std::endl;
		std::cout <<"  m_dest_format->a = " << (unsigned int) m_dest_format->a <<std::endl;
		std::cout <<"  m_dest_format->bits = " << (unsigned int) m_dest_format->bits <<std::endl;
		std::cout <<"  m_dest_format->indexed = " << (unsigned int) m_dest_format->indexed <<std::endl <<std::endl;
		
		std::cout <<"  line: " << line <<std::endl <<std::endl;

		std::cout <<"  source pitch: " << m_provider->get_pitch() <<std::endl;
		std::cout <<"  source width: " << m_provider->get_width() <<std::endl;
		std::cout <<"  source height: " << 1 <<std::endl <<std::endl;

		std::cout <<"  dest pitch: " << m_provider->get_width() * get_bytes_per_pixel() <<std::endl;
		std::cout <<"  dest width: " << get_width() <<std::endl;
		std::cout <<"  dest height: " << 1 <<std::endl <<std::endl;

		std::cout <<"  provider height: " << m_provider->get_height() <<std::endl;
		std::cout <<"  provider num frames: " << m_provider->get_num_frames() <<std::endl <<std::endl;
	}

	//: Get line pixel
	unsigned char *get_line_pixel(int y)
	{
		get_line_pixel_to_dest(y, scanline);
		return scanline;
	}

	//: Get line pixel16
	unsigned short *get_line_pixel16(int y)
	{
		return (unsigned short *) get_line_pixel(y);
	}

	//: Get line pixel32
	unsigned int *get_line_pixel32(int y)
	{
		return (unsigned int *) get_line_pixel(y);
	}
};

//: PixelData Palette Class
//- Class not implemented yet
class CL_PixelData_Palette
{
public:
//! Construction:
	//: PixelData Palette Constructor (not implemented yet)
	CL_PixelData_Palette(
		CL_Palette * /*dest_pal*/,
		CL_SurfaceProvider * /*src*/)
	{
		cl_assert(false); // not implemented yet.
	}

	//: PixelData Palette Destructor
	virtual ~CL_PixelData_Palette()
	{
	}
	
//! Attributes:
	//: Get Width
	unsigned int get_width()
	{
		return 0;
	}

	//: Get Height
	unsigned int get_height()
	{
		return 0;
	}

	//: Has colour key
	bool has_colorkey()
	{
		return false;
	}

	//: Get colour key
	unsigned int get_colorkey()
	{
		return 0;
	}

	//: Get line pixel
	unsigned char *get_line_pixel(int /*y*/)
	{
		return NULL;
	}
};

#endif
