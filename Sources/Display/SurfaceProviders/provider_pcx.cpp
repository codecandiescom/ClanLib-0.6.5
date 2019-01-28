/*
	$Id: provider_pcx.cpp,v 1.5 2002/02/03 20:56:06 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		PCX SurfaceProvider
*/

#include "Core/precomp.h"
#include "API/Display/Display/pixelformat.h"
#include "API/Display/Display/palette.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Display/SurfaceProviders/provider_pcx.h"
#include "API/Core/System/error.h" 
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/res_surface.h"
#include "API/Core/Resources/resourceoptions.h"
#include <iostream>

CL_Surface *CL_PCXProvider::create(
	std::string handle, 
	CL_InputSourceProvider *provider, 
	bool transparent, 
	unsigned char trans_col)
{
	return CL_Surface::create(new CL_PCXProvider(
		handle, 
		provider, 
		transparent, 
		trans_col), true);
}

CL_PCXProvider::CL_PCXProvider(
	std::string _name, 
	CL_InputSourceProvider *_provider, 
	bool _transparent, 
	unsigned char _trans_col)
{
	provider = _provider != NULL ? _provider->clone() : 
								CL_InputSourceProvider::create_file_provider(".");
	pitch = height = 0;
	bounding_left = bounding_top = bounding_right = bounding_bottom = 0;
	
	transparent = _transparent;
	if (!transparent) trans_col = -1;
	else trans_col = _trans_col;
	
	name = _name;
	palette = NULL;
	image = NULL;
	pixelformat = PAL8;
}

CL_PCXProvider::~CL_PCXProvider()
{
	perform_unlock();
	delete provider;
}
/*
EPixelFormat CL_PCXProvider::get_pixel_format() const
{
	return pixelformat;
}
*/

unsigned int CL_PCXProvider::get_depth() const
{
	switch (pixelformat)
	{
	case RGBA8888:
		return 32;
	default:
		return 8;
	}
}

unsigned int CL_PCXProvider::get_red_mask() const
{
	return 0xff000000;
}

unsigned int CL_PCXProvider::get_green_mask() const
{
	return 0x00ff0000;
}

unsigned int CL_PCXProvider::get_blue_mask() const
{
	return 0x0000ff00;
}

unsigned int CL_PCXProvider::get_alpha_mask() const
{
	return 0x000000ff;
}
void CL_PCXProvider::read_header(CL_InputSource *_datafile)
{
	//: This method was created to breakup the PCX decoding code
	//: so the initializing code was separated from the actual
	//: decoding algorithm
	
	unsigned char header[128];  
	
	int read = _datafile->read(header, 128);
	if (read != 128)
		throw CL_Error("Invalid pcx file!?");
	
	pcx_version = header[1];

	// int compressed = header[2];

	bits_per_pixel_per_plane = header[3];

	short xmin = header[5] * 256 + header[4];	
	short ymin = header[7] * 256 + header[6];
	short xmax = header[9] * 256 + header[8];
	short ymax = header[11] * 256 + header[10];

	height = ymax - ymin + 1;

	num_planes = header[65];
	dest_num_planes = num_planes;
	decode_pitch = header[67] * 256 + header[66];

	//decide which pixelformat to use (8 bit palette or 32 bit)
	switch (num_planes)		
	{
	case 1:
		{
			//palettized image
			//Deluxe Paint IIe compatible 4 color CGA or
			//Monochrome or
			//256 color palette based image
			//force the final image to 8 bit/palette.
			dest_num_planes = 1;
			pixelformat = PAL8;
		}
		break;
	case 2:
		{
			//force the final image to 8 bit/palette.
			dest_num_planes = 1;
			pixelformat = PAL8;
		}
		break;
	case 3:
		{
			if (bits_per_pixel_per_plane == 1)
			{
				//This means 8 color/palette
				pixelformat = PAL8;
				dest_num_planes = 1;
			}
			else
			{
				//This means it is 24 bit RGB.
				//24 bit images are being forced into 32 bit.
				//Pixelformat RGB888 did not appear to work at the time
				//of this PCX decoder rewrite.
				pixelformat = RGBA8888;
				dest_num_planes = 4;
			}
		}
		break;
	case 4:
		{
			//no palette
			//force the final image to 8 bit/palette.
			//This case represents 16 color EGA or 4 color CGA where only
			//1 bit is stored in each plane.  The 4th plane is the
			//intensity bit.
			dest_num_planes = 1;
			pixelformat = PAL8;
		}
		break;
	default:
		{
			throw CL_Error("Unrecognized or invalid pcx file!?");
		}
		break;
	}

	//set bytes per scanline of final image.  Requires dest_num_planes to be defined.
	pitch = (xmax - xmin  + 1) * dest_num_planes;
	
	if (transparent)
	{
		bounding_left = pitch / dest_num_planes;
		bounding_top = height;
		bounding_right = 0;
		bounding_bottom = 0;
	}
	else
	{
		bounding_left = 0;
		bounding_top = 0;
		bounding_right = pitch / dest_num_planes;
		bounding_bottom = height;
	}

	//get number of bites in encoded data (including possible palette after data)
	size_data = _datafile->size() - 128;

	//Now we deal with the palette which can be in the header,
	//or at the end of the file, or defined here.  It is possible that these
	//palette definitions should have been placed in palette.h
	if (pixelformat == PAL8)
	{
		//If the pcx version is 5, the palette may be at the end of the file.
		//Otherwise it is in the header.
		if (pcx_version >= 5)	
		{
			if (num_planes == 1)
			{
				//The palette information is stored at the end of the file.
				//read the last 768 bytes from the file.
				unsigned char *temp_pal = new unsigned char[768];
				_datafile->seek(128+size_data-768,CL_InputSource::seek_set);
				read = _datafile->read(temp_pal, 768);
				palette = new CL_Palette(temp_pal);
				delete[] temp_pal;
			}
			else
			{
				//The palette information is stored in the header.
				palette = new CL_Palette(&header[16],16);
			}
		}
		else
		{
			//The palette information is stored in the header.
			palette = new CL_Palette(&header[16],16);
		}
		
		//Setup palette for PCX types that rely on a pre-defined palette
		//(MONO, CGA, EGA)
		switch (num_planes)
		{
		case 1:
			{	
				//1 color plane, 1 bit per pixel per plane
				if (bits_per_pixel_per_plane == 1)
				{
					//Simulate a monochrome display.
					//Assign "Black" and "White" colors to palette
					//(assume "White" assigned to "On" bits)
					//It works!
					unsigned char mono_pal[6] = 
						{ 0,0,0,
						255,255,255 };
					delete palette;
					palette = new CL_Palette(mono_pal, 2);
				}

				//1 color plane, 2 bits per pixel per plane
				if (bits_per_pixel_per_plane == 2)
				{
					//CGA 4 color
					//It works!
					unsigned char cga_pal[12];

					if (header[19] & 64)
					{
						//Use background/cyan/magenta/white palette
						unsigned char cga_pal1[12] = 
							{ 0,0,0,
							0,255,255,
							255,0,255,
							255,255,255 };
						memcpy (cga_pal, cga_pal1, 12);
					}
					//eveything from here forward was not tested for this mode.
					else
					{
						//Use background/green/red/yellow palette
						unsigned char cga_pal2[12] = 
							{ 0,0,0,
							0,255,0,
							255,0,0,
							255,255,0 };
						memcpy (cga_pal, cga_pal2, 12);
					}
					
					//set the background color palette
					unsigned char backcolors[3][16] =
						{ {0,0,0,
						0,0,128,
						0,128,0,
						0,128,128,
						128,0,0,
						128},{0,128,
						192,96,0,
						192,192,192,
						128,128,128,
						0,0,255,
						0,255},{0,
						0,255,255,
						255,0,0,
						255,0,255,
						255,255,0,
						255,255,255}};
					 memcpy (cga_pal, &backcolors[0][header[16] & 240 >> 4], 3);

					//set the intensity
					for (int x=3;x < 12; x++)
					{
						if (!(header[19] & 32) && cga_pal[x])
						{
							cga_pal[x] = 128;
						}
					}

					delete palette;
					palette = new CL_Palette(cga_pal, 4);
				}

				//1 color plane, 4 bits per pixel per plane				
				if (bits_per_pixel_per_plane == 4)
				{
					//This is an EGA image.
					//the palette needs to normalized
					
					//normalize the EGA palette before loading it
					//Clanlib needs palette manipulation functions...  
					if (bits_per_pixel_per_plane == 4)
					{
						for (int x = 0; x < 48; x++)
						{

							header[16+x]=header[16+x]/64*64;
						}
					}
					palette = new CL_Palette(&header[16],16);
				}
			}
			break;
		case 2:
			//nothing special to do here.  A 4 color palette is used (already loaded).
			break;
		case 3:
			{
				if (bits_per_pixel_per_plane == 1)
				{
					//Assign the 8 possible combinations of RGB to the palette 
					//to simulate 8 color RGB display.  Otherwise, it is a 256
					//color picture.  
					//8 color untested.  256 color working.
					unsigned char rgb3_pal[24] = 
						{ 0,0,0,
						0,0,255,
						0,255,0,
						0,255,255,
						255,0,0,
						255,0,255,
						255,255,0,
						255,255,255 };
					delete palette;
					palette = new CL_Palette(rgb3_pal, 8);
				}
			}
			break;
		case 4:
			//This is a 16 color arbitrary palette image.
			//working.
			break; 
		}
	}
	else
	{
		//This is an RGBA8888 image.
		palette = NULL;
	}

	//This is number of bytes to allocate for the final image
	bytes_to_allocate = (8 / bits_per_pixel_per_plane) * pitch * height * dest_num_planes;

	//allocate the image
	image = new unsigned char[bytes_to_allocate];
	cl_assert(image != NULL);

	if (pixelformat == RGBA8888)
	{
		//Set all pixels to be opaque for RGBA8888
		memset(image, 255, bytes_to_allocate);
	}
	else
	{
		//initialize bits to 0 so that bitwise operations work correctly
		memset(image, 0, bytes_to_allocate);
	}
}

void CL_PCXProvider::read_data()
{
	// Do not run if we already have image data
	if (image != NULL) return;

	// open the file and make sure its valid
	cl_assert(provider != NULL);
	CL_InputSource *datafile = provider->open_source(name.c_str());
	cl_assert(datafile != NULL);
	
	// Read the file header and initialize the variables
	read_header(datafile);

	// This section reads the file data into memory
	datafile->seek(128, CL_InputSource::seek_set);
	int size_data = datafile->size() - 128;
	unsigned char *temp = new unsigned char[size_data];
	int read = datafile->read(temp, size_data);
	if (read != size_data)
		throw CL_Error("Invalid pcx file!?");
	delete datafile;
	
	//init decoding variables
	unsigned char *p = temp;
	unsigned char *cur_line = image;
	// unsigned char *base = image;
	unsigned char packed_byte;
	unsigned char bitmask = 0;
	int skip_planes = 0;

	//used later to select bits in bit depths less than 8.
	switch (bits_per_pixel_per_plane)
	{
	case 1:
		bitmask = 0x01;
		break;
	case 2:
		bitmask = 0x03;
		break;
	case 4:
		bitmask = 0x0F;
		break;
	case 8:
		bitmask = 0xFF;
		break;
	}

	if (pixelformat == RGBA8888)
			skip_planes = 1;
	
	//This section decodes the PCX data.
	for (int scanline=0;scanline<height;scanline++)
	{
		//For every scanline
		for (int plane = num_planes - 1; plane >= 0; plane--)
		{
			//for every plane (count backward to convert from RGB to ABGR)

			//tracks bytes decoded from data
			int decoded_bytes = 0;
			//tracks position within final image
			int line_position = 0;

			while (decoded_bytes < decode_pitch)
			{
				//while we have not reached the end of the decoded line
				
				//increment the data pointer
				packed_byte = *(p++);

				if (packed_byte < 192)
				{
					// top two bits are off... this byte is data
					//The processing is now broken down to bit depths of 1, 2, 4, or 8.
					for (int bitcounter = 0; bitcounter < 8; bitcounter += bits_per_pixel_per_plane)
					{
						//For every pixel to be decoded (8 / bits_per_pixel_per_plane

						if (bits_per_pixel_per_plane < 8)
						{
							//The bit depth is below 8.  Because we are converting all
							//bit depths below 8 to 8, the current line_position is
							//always where we are writing data.    
							
							//This line pulls the packed bits out of the data.
							unsigned char packed_bits = ((packed_byte >> (8 - bits_per_pixel_per_plane - bitcounter)) & bitmask);
							
							//This part pulls bits from multiple planes.  Because the bits
							//of a palette reference may be spread across multiple planes,
							//we modify the data that is already at the line position
							//(rather than simply overwriting it).
							packed_bits = (packed_bits << (num_planes - plane - 1));
							
							//Add bits to current position.  In the case of a 
							//palette that is referenced by bits in only one plane, 
							//this is only executed once per line_position.
							cur_line[line_position] = cur_line[line_position] + packed_bits;
						}
						else
						{
							//The image is 8 bit/pallete or 24 bit.  In either case,
							//we simply add the number of color planes to the line position.
							//Then we add 1 plane for alpha support in a 24(32) bit image
							//to skip over the alpha channel (which we assume has been set
							//to 0xFF).
							int image_dest = line_position *dest_num_planes + plane + skip_planes;						
							cur_line[image_dest] = packed_byte; 
						}
						line_position++;
					}
					if (transparent && packed_byte != trans_col)
					{
						if (decoded_bytes < bounding_left) bounding_left = decoded_bytes;
						if (scanline < bounding_top) bounding_top = scanline;
						if (decoded_bytes > bounding_right) bounding_right = decoded_bytes;
						if (scanline > bounding_bottom) bounding_bottom = scanline;
					}
					decoded_bytes++;
				}
				else 
				// top two bits are on... This is a run.  The other bits contain
				// a run count for the RLE compression.  The next byte will
				// contain data to repeat.
				{ 
					unsigned char rle_count = packed_byte&0x3f;
					packed_byte = *(p++);	
					//get data byte to repeat.

					for(unsigned char rle_iterator = 0; rle_iterator < rle_count; rle_iterator++)
					//repeat data value based on rle_count
					{
						//check for end of plane
						if (decoded_bytes >= decode_pitch)
						{
							plane--;
							decoded_bytes = 0;
							line_position = 0;
						}
						
						//The processing is now broken down to bit depths of 1, 2, 4, or 8
						for (int bitcounter = 0; bitcounter < 8; bitcounter += bits_per_pixel_per_plane)
						{
							// The decoding process here is the same as above (in the non-RLE code)
							if (bits_per_pixel_per_plane < 8)
							{
								unsigned char packed_bits = ((packed_byte >> (8 - bits_per_pixel_per_plane - bitcounter)) & bitmask);
								packed_bits = (packed_bits << (num_planes - plane - 1));
								cur_line[line_position] = cur_line[line_position] + packed_bits;
							}
							else
							{
								int image_dest = line_position *dest_num_planes + plane + skip_planes;						
								cur_line[image_dest] = packed_byte; 
							}
							line_position++;
						}						
						if (transparent && packed_byte != trans_col)
						{
							if ((decoded_bytes+rle_iterator) < bounding_left) bounding_left = decoded_bytes+rle_iterator;
							if (scanline < bounding_top) bounding_top = scanline;
							if ((decoded_bytes+rle_iterator) > bounding_right) bounding_right = decoded_bytes+rle_iterator;
							if (scanline > bounding_bottom) bounding_bottom = scanline;
						}
						decoded_bytes++;
					}
				}
			}
		}
		cur_line += pitch;
	}	
	delete[] temp;
}
void CL_PCXProvider::perform_lock()
{
	//The PCX decoding code was moved to read_data() and read_header()
	//to make it easier to follow the code.
	read_data();
}

void CL_PCXProvider::perform_unlock()
{
	delete[] image;
	delete palette;

	image = NULL;
	palette = NULL;
}
