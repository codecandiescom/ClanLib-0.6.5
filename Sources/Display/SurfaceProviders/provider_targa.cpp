/*
	$Id: provider_targa.cpp,v 1.4 2001/05/13 16:48:58 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		.TGA reader and surfaceprovider

*/

#include "Core/precomp.h"
#include <iostream>
#include <cstdio>
#include "API/Display/SurfaceProviders/provider_targa.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/res_surface.h"
#include "API/Core/Resources/resourceoptions.h"

using namespace std;

/*
  Generic info about using alphachannel/transparanty

  Apparently the code works in this way (as far as I can understand):

  ==============================================================
  The blit code "only" looks at rgb & alpha. 

  If alpha = 0 then a pixel is transparant
  If alpha = 255 then a pixel is "fully" visible. 
  All values in between are used for alphablending

  ==============================================================
  The *Loading* code works as follows:

  0) you can turn nothing on (or everything off :)
  1) you can turn on transparant pixels
  2) you can set to ignore the alphachannel (turn it off)
  
  ad 0) the alphachannel is just copied from the input file format 
		(or set to 255 if unavailable) and we don't care about 
		any pixels marked as transparant by color.

  ad 1) If you choose to turn on transparant pixels, then all pixels 
		are traversed and for the pixels matching the "transparant color", 
		the alpha value is set to 0 (transparant).

  ad 2) If you choose to ignore the alphachannel, then the alphachannel 
		for ALL pixels is set to 255 UNLESS you also specified to use 
		transparant pixels (wich means that some pixels might get aplha=0).

  note: if you have transparant pixels & alphachannel on then pixels
  with an alphavalue of 0 will be reset to the "transparant pixel color"
  ==============================================================
*/


CL_Surface *CL_TargaProvider::create(
	std::string file,
	CL_InputSourceProvider *provider, 
	bool transparent,
	bool ignore_alphachannel,
	unsigned char trans_red, 
	unsigned char trans_green, 
	unsigned char trans_blue)
{
	return CL_Surface::create(
		new CL_TargaProvider(
			file,
			provider, 
			transparent,
			ignore_alphachannel, 
			trans_red,
			trans_green,
			trans_blue),
		true);
}

CL_TargaProvider::CL_TargaProvider(
	std::string name,
	CL_InputSourceProvider *_provider,
	bool _transparent,
	bool _ignore_alphachannel,
	unsigned char trans_red, 
	unsigned char trans_green, 
	unsigned char trans_blue)
{
	if (_provider == NULL)
	{
		provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		provider = _provider->clone();
	}

	ignore_alphachannel = _ignore_alphachannel;
	transparent = _transparent;
	use_alphapixels = transparent && !ignore_alphachannel;
	trans_redcol = trans_red;
	trans_greencol = trans_green;
	trans_bluecol = trans_blue;

	if (transparent)
	{
		trans_col = (((unsigned int) trans_redcol) << 24) +
			(((unsigned int) trans_greencol)<<16) +
			(((unsigned int) trans_bluecol) << 8);
	}
	else
	{
		trans_col = -1;
	}

	locked = false;
	filename = name;
	file = image = color_map = NULL;
}

CL_TargaProvider::~CL_TargaProvider()
{
	perform_unlock();
	delete provider;
}

/*
	Read the file data - a .TGA file can exist in several
	different formats - not all formats are supported by
	this surfaceprovider.
*/
void CL_TargaProvider::read_data()
{
	datatype = file[2];

	switch (datatype)
	{
	case 1:
		read_colormapped();
		return;
		break;
	case 2:
		read_uncompressed_rgb();
		return;
		break;
	case 9:
		// this format is not tested (but I think it works)
		read_runlength_encoded_colormapped_rgb();
		return;
		break;
	case 10:
		read_runlength_encoded_rgb();
		return;
		break;
	}

	std::string err("CL_TargaProvider Fatal Error: Unsupported TGA filetype encountered");
	throw CL_Error(err);
//	cl_assert(false);
}

/*
	Reads the .TGA header - this header is relatively equal
	between subformats - except for whether the colormap
	is interesting.
*/
void CL_TargaProvider::read_header(bool read_colormap)
{
	unsigned char idlength = file[0];
	unsigned char colormaptype = file[1];

	pos = 18;

	// Skip the id-string (if any)
	if (idlength != 0)
	{
		pos += idlength;
	}

	bpp = file[16];

	// read or skip the colormap (rgb-palette)
	if (colormaptype == 1)
	{
		map_length = *((unsigned short *) &file[5]);
		SWAP_IF_BIG(map_length);
		unsigned char map_size = file[7]>>3;

		if (!read_colormap)
		{
			// Skip colormap - we dont need it
			pos += map_length*map_size;
		}
		else
		{
			// Read the colormap
			color_map = new unsigned char[map_length * 4];
			
			// read color depending on bytes-pr-pixel
			for (unsigned int i=0;i<map_length;i++)
			{
				switch (bpp)
				{
				case 16:
					read_rgb_16(&color_map[i*4+0],
							 &color_map[i*4+1],
							 &color_map[i*4+2],
							 &color_map[i*4+3]);
					break;
				case 24:
					read_rgb_24(&color_map[i*4+0],
							 &color_map[i*4+1],
							 &color_map[i*4+2],
							 &color_map[i*4+3]);
					break;
				case 32:
					read_rgb_32(&color_map[i*4+0],
							 &color_map[i*4+1],
							 &color_map[i*4+2],
							 &color_map[i*4+3]);
					break;
				default:
					{
						char aBuf[500];
						sprintf(aBuf, "Unsupported bit-depth in targafile '%s'", filename.c_str());
						cl_info(1, aBuf);
					}
				}
			}
		}
	}

	// read pitch, height and bits-pr-pixel
	pitch = *((unsigned short *) &file[12]);
	SWAP_IF_BIG(pitch);
	bounding_left = pitch;
	bounding_right = 0;

	height = *((unsigned short *) &file[14]);
	SWAP_IF_BIG(height);
	bounding_top = height;
	bounding_bottom = 0;

	map_direction_x = (file[17] & 0x10) == 0 ? 1 : -1;
	map_direction_y = (file[17] & 0x20) == 0 ? -1 : 1;
}

/*
	Read and convert a rgb-color from the current position
	in the file.
*/
inline bool CL_TargaProvider::read_rgb_16(
	unsigned char *a, 
	unsigned char *b, 
	unsigned char *g, 
	unsigned char *r)
{
	bool ret = true;

	if (pos >= filesize)
		throw CL_Error("Invalid targa file!?");

	// In 16 bits pr pixel the individual color-intensities are
	// described in two bytes:
	// ARRRRRGG	 GGGBBBBB
	// A = ATTRIBUTE
	// R = RED
	// G = GREEN
	// B = BLUE
	// These two bytes are in reality read backwards (last byte first)

	*a = (file[pos+1]&128) ? 0 : 255;
	// Transparent pixel
	if (use_alphapixels && !(*a))
	{
		*r = trans_redcol;
		*g = trans_greencol;
		*b = trans_bluecol;
		ret = false;
	}
	else
	{
		*r = ((file[pos+1]>>2) & 0x1f)<<3;
		*g = (((file[pos+1] & 0x03)<<3) + 
			 ((file[pos]>>5) & 0x7))<<3;
		*b = (file[pos] & 0x1f)<<3;
		
		if (transparent &&
			*r == trans_redcol &&
			*g == trans_greencol &&
			*b == trans_bluecol)
		{
			*a = 0;
			ret = false;
		}
//
		else
		if (ignore_alphachannel) 
		{
			// else if we're not using any alphapixels at all, then alpha should be 255
			*a = 255;
		}
//
	}

	pos += 2;

	return ret;
}

inline bool CL_TargaProvider::read_rgb_24(
	unsigned char *a, 
	unsigned char *b, 
	unsigned char *g, 
	unsigned char *r)
{
	bool ret = true;

	if (pos >= filesize)
		throw CL_Error("Invalid targa file!?");

	*b = file[pos];
	*g = file[pos+1];
	*r = file[pos+2];
	*a = 255;

	if (transparent && 
		*r == trans_redcol &&
		*g == trans_greencol &&
		*b == trans_bluecol)
	{
		*a = 0;
		ret = false;
	}


	pos += 3;

	return ret;
}

inline bool CL_TargaProvider::read_rgb_32(
	unsigned char *a, 
	unsigned char *b, 
	unsigned char *g, 
	unsigned char *r)
{
	bool ret = true;

	if (pos >= filesize)
		throw CL_Error("Invalid targa file!?");

	*a = file[pos+3];
	if (use_alphapixels && !(*a))
	{
		*r = trans_redcol;
		*g = trans_greencol;
		*b = trans_bluecol;

		ret = false;
	}
	else
	{
		*b = file[pos];
		*g = file[pos+1];
		*r = file[pos+2];
		
		if (transparent && 
			*r == trans_redcol &&
			*g == trans_greencol &&
			*b == trans_bluecol)
		{
			// if using transparant and we have matching transparant pixel, alpha should be 0
			*a = 0;
			ret = false;
		}
//
		else
		if (ignore_alphachannel) 
		{
			// else if we're not using any alphapixels at all, then alpha should be 255
			*a = 255;
		}
//
	}

	pos += 4;

	return ret;
}

inline bool CL_TargaProvider::read_rgb(
	unsigned char *a, 
	unsigned char *b, 
	unsigned char *g, 
	unsigned char *r)
{
	bool ret = true;

	if (pos >= filesize)
		throw CL_Error("Invalid targa file!?");

	switch (bpp)
	{
	case 16:
		// In 16 bits pr pixel the individual color-intensities are
		// described in two bytes:
		// ARRRRRGG	 GGGBBBBB
		// A = ATTRIBUTE
		// R = RED
		// G = GREEN
		// B = BLUE
		// These two bytes are in reality read backwards (last byte first)

		*a = (int(file[pos+1]&128)>>7)*255;
		// Transparent pixel
		if (use_alphapixels && !(*a))
		{
			*r = trans_redcol;
			*g = trans_greencol;
			*b = trans_bluecol;
			ret = false;
		}
		else
		{
			*r = ((file[pos+1]>>2) & 0x1f)<<3;
			*g = (((file[pos+1] & 0x03)<<3) + 
				 ((file[pos]>>5) & 0x7))<<3;
			*b = (file[pos] & 0x1f)<<3;
			
			if (transparent &&
				*r == trans_redcol &&
				*g == trans_greencol &&
				*b == trans_bluecol)
			{
				*a = 0;
				ret = false;
			}
//
			else
			if (ignore_alphachannel) 
			{
				// else if we're not using any alphapixels at all, then alpha should be 255
				*a = 255;
			}
//
		}

		pos += 2;
		break;

	case 24:
		*b = file[pos];
		*g = file[pos+1];
		*r = file[pos+2];
		*a = 255;

		if (transparent && 
			*r == trans_redcol &&
			*g == trans_greencol &&
			*b == trans_bluecol)
		{
			*a = 0;
			ret = false;
		}

		pos += 3;
		break;

	case 32:
		*a = file[pos+3];
		if (use_alphapixels && !(*a))
		{
			*r = trans_redcol;
			*g = trans_greencol;
			*b = trans_bluecol;

			ret = false;
		}
		else
		{
			*b = file[pos];
			*g = file[pos+1];
			*r = file[pos+2];
			
			if (transparent && 
				*r == trans_redcol &&
				*g == trans_greencol &&
				*b == trans_bluecol)
			{
				*a = 0;
				ret = false;
			}
//
			else
			if (ignore_alphachannel) 
			{
				// else if we're not using any alphapixels at all, then alpha should be 255
				*a = 255;
			}
//
		}

		pos += 4;
		break;
	}

	return ret;
}

/*
	Read and convert a colormap-index into an rgba-color.
	This function speculates in an index to exist in the 
	file directly at the current position.
*/
void CL_TargaProvider::read_from_colormap(
	unsigned char *a,
	unsigned char *b,
	unsigned char *g,
	unsigned char *r)
{
//	cl_assert(pos < filesize);
	if (pos >= filesize)
		throw CL_Error("Invalid targa file!?");

	// if the colormap is <= 256 entries large, the
	// index will be contained in a byte - 
	// otherwise in a short
	int entry;
	if (map_length <= 256)
	{
		entry = file[pos++];
	}
	else
	{
		entry = *((unsigned short *) &file[pos]);
		SWAP_IF_BIG(entry);
		pos += 2;
	}

//	cl_assert(entry >= 0 && entry < map_length);
	if (entry < 0 || entry >= map_length)
		throw CL_Error("Invalid targa file!?");

	// convert index-value to rgba-values using the colormap
	*r = color_map[entry*4+0];
	*g = color_map[entry*4+1];
	*b = color_map[entry*4+2];
	*a = color_map[entry*4+3];
}

/*
	CODE FOR DECODING TYPE 1 TARGA FILES
*/
void CL_TargaProvider::read_colormapped()
{
	read_header(true);

	image = new unsigned char[pitch * height * 4];

	int ystart = map_direction_y == 1 ? 0 : height-1;
	int xstart = map_direction_x == 1 ? 0 : pitch-1;

	for (int y = ystart;y>=0&&y<height;y+=map_direction_y)
	{
		for (int x = xstart; x>=0 && x<pitch;x+=map_direction_x)
		{
			read_from_colormap(&image[(x+y*pitch)*4],
								 &image[(x+y*pitch)*4+1],
								 &image[(x+y*pitch)*4+2],
								 &image[(x+y*pitch)*4+3]);
		}
	}
}

/*
	CODE FOR DECODING TYPE 2 TARGA FILES
*/
void CL_TargaProvider::read_uncompressed_rgb()
{
	read_header(false);

	int ystart = map_direction_y == 1 ? 0 : height-1;
	int xstart = map_direction_x == 1 ? 0 : pitch-1;

	image = new unsigned char[pitch * height * 4];
	switch (bpp)
	{
	case 16:
		{
			for (int y = ystart;y>=0&&y<height;y+=map_direction_y)
			{
				for (int x = xstart; x>=0 && x<pitch;x+=map_direction_x)
				{
					bool v = read_rgb_16(&image[(x+y*pitch)*4+0], 
										&image[(x+y*pitch)*4+1],
										&image[(x+y*pitch)*4+2],
										&image[(x+y*pitch)*4+3]);
					if (v)
					{
						if (x < bounding_left) bounding_left = x;
						if (y < bounding_top) bounding_top = y;
						if (x > bounding_right) bounding_right = x;
						if (y > bounding_bottom) bounding_bottom = y;
					}
				}
			}
		}
		break;
	
	case 24:
		{
			for (int y = ystart;y>=0&&y<height;y+=map_direction_y)
			{
				for (int x = xstart; x>=0 && x<pitch;x+=map_direction_x)
				{
					bool v = read_rgb_24(&image[(x+y*pitch)*4+0], 
												&image[(x+y*pitch)*4+1],
												&image[(x+y*pitch)*4+2],
												&image[(x+y*pitch)*4+3]);
					if (v)
					{
						if (x < bounding_left) bounding_left = x;
						if (y < bounding_top) bounding_top = y;
						if (x > bounding_right) bounding_right = x;
						if (y > bounding_bottom) bounding_bottom = y;
					}

				}
			}
		}
		break;

	case 32:
		{
			for (int y = ystart;y>=0&&y<height;y+=map_direction_y)
			{
				for (int x = xstart; x>=0 && x<pitch;x+=map_direction_x)
				{
					bool v = read_rgb_32(&image[(x+y*pitch)*4+0], 
										&image[(x+y*pitch)*4+1],
										&image[(x+y*pitch)*4+2],
										&image[(x+y*pitch)*4+3]);
					if (v)
					{
						if (x < bounding_left) bounding_left = x;
						if (y < bounding_top) bounding_top = y;
						if (x > bounding_right) bounding_right = x;
						if (y > bounding_bottom) bounding_bottom = y;
					}
				}
			}
		}
		break;

	default: 
		std::cout << "Unknown bpp!" << std::endl;
		cl_assert(false);
	}
}

/*
	CODE FOR DECODING TYPE 9 TARGA FILES
*/

void CL_TargaProvider::read_runlength_encoded_colormapped_rgb()
{
	read_header(true);

	image = new unsigned char[pitch * height * 4];
	
	int ystart = map_direction_y == 1 ? 0 : height-1;
	int xstart = map_direction_x == 1 ? 0 : pitch-1;

	int line = ystart;
	int xpos = xstart;
	while (line >= 0 && line < height)
	{
		unsigned char head = file[pos++];
		unsigned char type = (head & 128)>>7;
		unsigned char repcount = (head & 127)+1;

		switch (type)
		{
		// RAW PACKET
		case 0:
			for (;repcount>0;repcount--)
			{
				read_from_colormap(&image[(xpos+line*pitch)*4],
									 &image[(xpos+line*pitch)*4+1],
									 &image[(xpos+line*pitch)*4+2],
									 &image[(xpos+line*pitch)*4+3]);

				xpos+=map_direction_x;
				if (xpos < 0 || xpos >= pitch)
				{
					xpos = xstart;
					line += map_direction_y;
				}
			}
			break;

		// RUNLENGTH ENCODED PACKET
		case 1:
			{
				unsigned char r, g, b, a;
				read_from_colormap(&r, &g, &b, &a);
				
				for (;repcount>0;repcount--)
				{
					image[((line*pitch+xpos)*4)+0] = r;
					image[((line*pitch+xpos)*4)+1] = g;
					image[((line*pitch+xpos)*4)+2] = b;
					image[((line*pitch+xpos)*4)+3] = a;

					xpos+=map_direction_x;
					if (xpos < 0 || xpos >= pitch)
					{
						xpos = xstart;
						line += map_direction_y;
					}
				}
			}
			break;
		}
	}
}

/*
	CODE FOR DECODING TYPE 10 TARGA FILES
*/
void CL_TargaProvider::read_runlength_encoded_rgb()
{
	read_header(false);

	image = new unsigned char[pitch * height * 4];

	int ystart = map_direction_y == 1 ? 0 : height-1;
	int xstart = map_direction_x == 1 ? 0 : pitch-1;

	int line = ystart;
	int xpos = xstart;
	while (line >= 0 && line < height)
	{
		unsigned char head = file[pos++];
		unsigned char type = (head & 128)>>7;
		unsigned char repcount = (head & 127)+1;

		switch (type)
		{
		// RAW PACKET
		case 0:
			for (;repcount>0;repcount--)
			{
				bool v = read_rgb(&image[((line*pitch+xpos)*4)+0], 
									&image[((line*pitch+xpos)*4)+1],
									&image[((line*pitch+xpos)*4)+2],
									&image[((line*pitch+xpos)*4)+3]);
				if (v)
				{
					if (xpos < bounding_left) bounding_left = xpos;
					if (line < bounding_top) bounding_top = line;
					if (xpos > bounding_right) bounding_right = xpos;
					if (line > bounding_bottom) bounding_bottom = line;
				}

				xpos+=map_direction_x;
				if (xpos < 0 || xpos >= pitch)
				{
					xpos = xstart;
					line += map_direction_y;
				}
			}
			break;

		// RUNLENGTH ENCODED PACKET
		case 1:
			{
				unsigned char r, g, b, a;
				bool v = read_rgb(&r, &g, &b, &a);
				
				for (;repcount>0;repcount--)
				{
					image[((line*pitch+xpos)*4)+0] = r;
					image[((line*pitch+xpos)*4)+1] = g;
					image[((line*pitch+xpos)*4)+2] = b;
					image[((line*pitch+xpos)*4)+3] = a;

					if (v)
					{
						if (xpos < bounding_left) bounding_left = xpos;
						if (line < bounding_top) bounding_top = line;
						if (xpos > bounding_right) bounding_right = xpos;
						if (line > bounding_bottom) bounding_bottom = line;
					}

					xpos+=map_direction_x;
					if (xpos < 0 || xpos >= pitch)
					{
						xpos = xstart;
						line += map_direction_y;
					}
				}
			}
			break;
		}
	}
}

void *CL_TargaProvider::get_data() const
{
	return image+bounding_left*4+bounding_top*pitch*4;
}

/*
	Lock the surfaceprovider - which basically means open the file
	and read the image into a temporary memory buffer - until
	unlock() is called.
*/
void CL_TargaProvider::perform_lock()
{
	if (locked) return;

	cl_assert(provider != NULL);
	input_source = provider->open_source(filename.c_str());
	cl_assert(input_source!=NULL);

	no_sprs = 1;

	filesize = input_source->size();
	file = new unsigned char[filesize];
	cl_assert(file != NULL);

	int num_bytes_read = input_source->read(file, filesize);
	cl_assert(num_bytes_read == ((int) filesize));

	// read the data
	read_data();

	if (bounding_left > bounding_right) bounding_left = bounding_right;
	if (bounding_top > bounding_bottom) bounding_top = bounding_bottom;

	delete[] file;
	delete input_source;
	file = NULL;

	locked = true;
}

void CL_TargaProvider::perform_unlock()
{
	locked = false;
	delete[] color_map;
	delete[] image;

	image = color_map = NULL;
}
