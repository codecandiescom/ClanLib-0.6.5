/*
	$Id: jpgd_provider.cpp,v 1.7 2001/09/08 19:24:21 japj Exp $
	
	ClanLib surface provider implementation for the "Small JPEG Decoder Library".
	Copyright 2001 Magnus Norddahl / ClanSoft.
*/

#include "API/SmallJPEG/jpgd_provider.h"
#include "jpgd/jpegdecoder.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "stream_datafile.h"

CL_JPGDProvider::CL_JPGDProvider(const std::string &filename, CL_InputSourceProvider *_provider)
: filename(filename), provider(0), width(0), height(0), components(0), data(0), ref_count(0)
{
	if (_provider == 0)
	{
		provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		provider = _provider->clone();
	}
}

CL_JPGDProvider::~CL_JPGDProvider()
{
	delete provider;
	delete data;
}
		
void CL_JPGDProvider::lock()
{
	ref_count++;
	if (ref_count > 1) return;

	bool use_mmx = false;

	jpeg_decoder_datafile_stream stream(provider->open_source(filename.c_str()), true);

	jpeg_decoder decoder(&stream, use_mmx);
	if (decoder.get_error_code() != 0) throw CL_Error("Decoder failed (1)!");
		
	width = decoder.get_width();
	height = decoder.get_height();

	components = decoder.get_num_components();
	if (components != 3 && components != 4)
		throw CL_Error("This jpeg provider only support 3 or 4 components at the moment");
		
	data = new unsigned int[width * height];
	memset(data, 0, width*height*sizeof(unsigned int));
	try
	{
		decoder.begin();
		for (unsigned int y=0; y<height; y++)
		{
			void *scanline;
			uint scanline_len;
				
			if (decoder.decode(&scanline, &scanline_len)) break;
				
			if (scanline_len < width*components)
				throw CL_Error("jpgd provider: scanline_len is less than expected length!");
				
			unsigned char *src_pixels = (unsigned char *) scanline;
			unsigned int *dst_pixels = &data[y*width];
			for (unsigned int x=0; x<width; x++)
			{
				int src_pos = x * 4; //components;

				if (components == 3)
				{
					dst_pixels[x] =
						(src_pixels[src_pos + 0] << (8+8+8)) +
						(src_pixels[src_pos + 1] << (8+8)) +
						(src_pixels[src_pos + 2] << (8)) +
						255;
				}
				else
				{
					dst_pixels[x] =
						(src_pixels[src_pos + 0] << (8+8+8)) +
						(src_pixels[src_pos + 1] << (8+8)) +
						(src_pixels[src_pos + 2] << (8)) +
						src_pixels[src_pos + 3];
				}
			}
		}
		
		if (decoder.get_error_code()) throw CL_Error("Decoder failed (2)!");
	}
	catch (...)
	{
		delete[] data;
		data = 0;
		throw;
	}

	components = 4;
}
	
void CL_JPGDProvider::unlock()
{
	ref_count--;
	if (ref_count > 0) return;
		
	delete[] data;
	data = 0;
}

void *CL_JPGDProvider::get_data() const
{
	return data;
}

unsigned int CL_JPGDProvider::get_num_frames() const
{
	return 1;
}

unsigned int CL_JPGDProvider::get_width() const
{
	return width;
}

unsigned int CL_JPGDProvider::get_height() const
{
	return height;
}

unsigned int CL_JPGDProvider::get_pitch() const
{
	return get_width() * get_bytes_per_pixel();
}

unsigned int CL_JPGDProvider::get_bytes_per_pixel() const
{
	return 4;
}

bool CL_JPGDProvider::is_indexed() const
{
	return false;
}

unsigned int CL_JPGDProvider::get_red_mask() const
{
	if (components == 3) return 0x00ff0000;
	return 0xff000000;
}

unsigned int CL_JPGDProvider::get_green_mask() const
{
	if (components == 3) return 0x0000ff00;
	return 0x00ff0000;
}

unsigned int CL_JPGDProvider::get_blue_mask() const
{
	if (components == 3) return 0x000000ff;
	return 0x0000ff00;
}

unsigned int CL_JPGDProvider::get_alpha_mask() const
{
	if (components == 3) return 0;
	return 0x000000ff;
}

CL_Palette *CL_JPGDProvider::get_palette() const
{
	return NULL;
}

bool CL_JPGDProvider::uses_src_colorkey() const
{
	return false;
}

unsigned int CL_JPGDProvider::get_src_colorkey() const
{
	return 0;
}

