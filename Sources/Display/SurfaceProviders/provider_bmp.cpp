/*
	$Id: provider_bmp.cpp,v 1.2 2001/03/13 21:43:01 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		BMP SurfaceProvider
*/
#include "Core/precomp.h"
#include <iostream>
#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#include "API/Display/Display/pixelformat.h"
#include "API/Display/Display/palette.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Display/SurfaceProviders/provider_bmp.h"
#include "API/Core/System/error.h" 
#include "API/Core/System/cl_assert.h"
#include "API/Display/Display/res_surface.h"
#include "API/Core/Resources/resourceoptions.h"

CL_Surface *CL_BMPProvider::create(
	std::string handle, 
	CL_InputSourceProvider *provider, 
	bool transparent, 
	short trans_col)
{
	return CL_Surface::create(new CL_BMPProvider(
		handle, 
		provider, 
		transparent, 
		trans_col), true);
}

CL_BMPProvider::CL_BMPProvider(
	std::string _name, 
	CL_InputSourceProvider *_provider, 
	bool _transparent, 
	short _trans_col)
{
	provider = _provider != NULL ? _provider->clone() : 
								CL_InputSourceProvider::create_file_provider(".");
//	width = height = 0;
	//bounding_left = bounding_top = bounding_right = bounding_bottom = 0;
	
	transparent = _transparent;
	if (!transparent) trans_col = -1;
	else trans_col = _trans_col;
	
	name = _name;
	palette = NULL;
	image = NULL;
//	pixelformat = PAL8;
}

CL_BMPProvider::~CL_BMPProvider()
{
	perform_unlock();
	delete provider;
}
/*
EPixelFormat CL_BMPProvider::get_pixel_format() const
{
	return pixelformat;
}
*/

unsigned int CL_BMPProvider::get_depth() const
{
	if (biBitCount==24)
		return 32;
	else if (biBitCount==8)
		return 8;
	else
		return ~0;
}

unsigned int CL_BMPProvider::get_pitch() const
{	
	if (biBitCount==24)
		return 4*get_width();
	
	else if (biBitCount==8)
		return get_width();

	else
		return ~0;

}

unsigned int CL_BMPProvider::get_red_mask() const
{

	return 0xff000000;
	//return Rmask;
}

unsigned int CL_BMPProvider::get_green_mask() const
{
	return 0x00ff0000;
	//return Gmask;
}

unsigned int CL_BMPProvider::get_blue_mask() const
{
	return 0x0000ff00;
	//return Bmask;
}

unsigned int CL_BMPProvider::get_alpha_mask() const
{
	return 0x000000ff;
}

void CL_BMPProvider::perform_lock()
{
	if (image != NULL) return;

	cl_assert(provider != NULL);
	CL_InputSource *datafile = provider->open_source(name.c_str());
	cl_assert(datafile != NULL);


// ---------------- read the bmp headers ---------
	magic[0] = datafile->read_char8();
	magic[1] = datafile->read_char8();

	if ( (magic[0] != 'B') && (magic[1] != 'M') )
		std::cout << std::endl << "File is not a Windows BMP file(DIB)" << std::endl;

	bfSize		= datafile->read_uint32();//SDL_ReadLE32(src);
	bfReserved1	= datafile->read_ushort16();//SDL_ReadLE16(src);
	bfReserved2	= datafile->read_ushort16();//SDL_ReadLE16(src);
	bfOffBits	= datafile->read_uint32();//SDL_ReadLE32(src);

	/* Read the Win32 BITMAPINFOHEADER */
	biSize		= datafile->read_uint32();//SDL_ReadLE32(src);
	biWidth			= datafile->read_int32();//SDL_ReadLE32(src);
	biHeight		= datafile->read_int32();//SDL_ReadLE32(src);
	biPlanes		= datafile->read_ushort16();//SDL_ReadLE16(src);
	biBitCount		= datafile->read_ushort16();//SDL_ReadLE16(src);
	biCompression	= datafile->read_uint32();//SDL_ReadLE32(src);
	biSizeImage		= datafile->read_uint32();//SDL_ReadLE32(src);
	biXPelsPerMeter	= datafile->read_int32();//SDL_ReadLE32(src);
	biYPelsPerMeter	= datafile->read_int32();//SDL_ReadLE32(src);
	biClrUsed		= datafile->read_uint32();//SDL_ReadLE32(src);
	biClrImportant	= datafile->read_uint32();//SDL_ReadLE32(src);
	
	if(biSize!=40)
		std::cout << std::endl << "This BMP file are in a old format. Please update this BMP file to BMP(DIB) version 3.0,";

	std::cout << std::endl
		<< "bfSize = " << bfSize << std::endl
		<< "bfReserved1 = " << bfReserved1 << std::endl
		<< "bfReserved2 = " << bfReserved2 << std::endl
		<< "bfOffBits = " << bfOffBits << std::endl
		<< "biWidth = " << biWidth << std::endl
		<< "biHeight = " << biHeight << std::endl
		<< "biPlanes = " << biPlanes << std::endl
		<< "biBitCount = " << biBitCount << std::endl
		<< "biCompression = " << biCompression << std::endl
		<< "biSizeImage = " << biSizeImage << std::endl
		<< "biXPelsPerMeter = " << biXPelsPerMeter << std::endl
		<< "biYPelsPerMeter = " << biYPelsPerMeter << std::endl
		<< "biClrUsed = " << biClrUsed << std::endl
		<< "biClrImportant = " << biClrImportant << std::endl;
// --(end)--------- read the bmp headers ---------		



	if ( (biBitCount==8) || (biBitCount==16) || (biBitCount==24) )
		//this is supported
		;

	else
	{
		throw CL_Error("We only support 8, 16 or 24 bpp.");
	}






	/* Load the palette, if any */
	if (biBitCount==8)
	{
		if(biClrUsed==0)//ofcouse there is some colors use !! ;-)
			biClrUsed=256;
		
		unsigned char *pal = new unsigned char[(biClrUsed*3)];
		
		int i = 0;

		for (;;)
		{
			//std::cout << "pal[i] = " << i;
			
			pal[i+2] = datafile->read_uchar8();
						
			//std::cout << "pal[i] = " << i;
			
			pal[i+1] = datafile->read_uchar8();
			
			//std::cout << "pal[i] = " << i;
			
			pal[i] = datafile->read_uchar8();

			i+=3;
			//std::cout << std::endl;
			
			
			// read the unused space in palette
			datafile->read_uchar8();
			
			// Break if finish reading palette.
			if ( ((int)biClrUsed*3) <= i)
				break;
		}
	
		//set new palette
		palette = new CL_Palette(pal, biClrUsed);	
	
	}

	else
	{
		palette = NULL;
	}
	


//---------- load 8bpp - BI_RGB encoding ----------------------

	
	if ( (biBitCount==8) && (biCompression==BI_RGB) )
	{
		image = new unsigned char[biSizeImage];
	
		cl_assert(image != NULL);

		for (int y=biHeight-1; y>-1; y--)
		//for (int y=0; y<biHeight; y++)
		{
			//for (int x=biWidth-1; x>-1; x--)
			for (int x=0; x<biWidth; x++)
			{
				image[(x+y*biHeight)] = datafile->read_uchar8(); 
			}
		}
	

	}
//--(end)--- load 8bpp - BI_RGB encoding ----------------------


//---------- load 8bpp - BI_RLE8 encoding ----------------------
	
	if ( (biBitCount==8) && (biCompression==BI_RLE8) )
	{
		image = new unsigned char[biWidth*biHeight];

		cl_assert(image != NULL);
		
		//unsigned char value1=0;
		//unsigned char value2=0;
/*
		int byte1=0;
		int byte2=0;

		unsigned char color=5;
		int num_pixels=0;

		//bool need_input = true;
		//byte1 = datafile->read_uchar8();
		//byte2 = datafile->read_uchar8();
		int mode=0;//1= encode, 2= absolute
		
		for (int y=biHeight-1; y>-1; y--)
		{
			for (int x=0; x<biWidth; x++)
			{
				std::cout << std::endl << "num_pixels = " << num_pixels; 
				std::cout << std::endl << "color = " << (int)color; 

				//are we finish writing pixels we know ?, if so lets get some more.
				if(num_pixels<=0)
				{
					if(mode ==2)//read out the unuse byte
						datafile->read_uchar8();

					byte1 = (int)datafile->read_uchar8();
					byte2 = (int)datafile->read_uchar8();
				
					// Absolute mode.
					if( (byte1 == 0) && ( !(byte2==0) || !(byte2==1) || !(byte2==2) ) )
					{
						num_pixels = byte2;
						mode = 2;
					
					}// end -- Absolute mode.
					// Encoded mode
					else
					{
						mode = 1;

						if(byte1 == 0)
						{
							switch(byte2)
							{
							case 0://End of line.
								num_pixels = (biWidth-x);
								break;
									
							case 1://End of bitmap.
								num_pixels = (biWidth-x)*(y-biHeight);
								break;

							case 2://Delta. The two bytes following the escape contain unsigned values indicating the horizontal and vertical offsets of the next pixel from the current position.
								int dummy_x = (int)datafile->read_uchar8();
								int dummy_y = (int)datafile->read_uchar8();
								
								num_pixels = (int)((biWidth+dummy_x)*(biHeight-dummy_y));
								break;
							}
						}
						else
						{
							num_pixels = byte1;
							color      = (unsigned char)byte2;
						}


					}// end -- Encoded mode

				
					//write pixel
					if(mode==1)//encoded
					{
						//color = datafile->read_uchar8();
						image[(x+y*biHeight)] = color;

						num_pixels--;
					}
					else if(mode==2)//absolute
					{
						color = datafile->read_uchar8();
						image[(x+y*biHeight)] = color;

						num_pixels--;

					}
				
				}
				
				

				
				
			}
		}
	}
*/	

		unsigned char value1=0;
		unsigned char value2=0;

		unsigned char color=5;
		unsigned char num_pixels=0;

		//bool need_input = true;
		//byte1 = datafile->read_uchar8();
		//byte2 = datafile->read_uchar8();
//		int mode=0;//1= encode, 2= absolute
	
		int x=0;
		int y=0/*biHeight*/;


		for (;;)
		{
		

			// Absolute mode.
			if( (value1 == 0) && !(value2==0 || value2==1 || value2==2) )
			{
				for	(int i=0; i < value2; i++)
				{
					if(x >= biWidth)
					{
						x=0;
						//y--;
						y++;
					}
					color = datafile->read_uchar8();
										
					image[(x+y*biWidth)] = color;
					x++;		
					
				}
				//read out the unuse byte
				datafile->read_uchar8();
				//datafile->read_ushort16();

			}// end -- Absolute mode.
			// Encoded mode
			else
			{
				if(value1 == 0)
				{
					switch(value2)
					{
					case 0://End of line.
						for (; x<biWidth; x++)
							image[(x+y*biWidth)] = color;
						break;
							
					case 1://End of bitmap.
						//for (;  y>-1; y--)
						for (; y<biHeight; y++)
							for (; x<biWidth; x++)
								image[(x+y*biWidth)] = color;
						break;

					case 2://Delta. The two bytes following the escape contain unsigned values indicating the horizontal and vertical offsets of the next pixel from the current position.
						value1 = datafile->read_uchar8();
						value2 = datafile->read_uchar8();

						x += value1;
						y += value2;
					break;
					}
				}
				
				else
				{
					num_pixels = value1;
					color      = value2;

					for(int i = 0; i < num_pixels;i++)
					{
						if(x >= biWidth)
						{
							x=0;
							y++;
						}
						image[(x+y*biWidth)] = color;
						x++;	
												
					}
				}


			}// end -- Encoded mode

	

			
			// If we are finish reading the bitmap, then exit.
			//if( (x+y*biWidth) >= (biWidth*biHeight) )
			if ( (y==biHeight)/*(y==-1)*/ && (x==biWidth) )
				break;

			if(x >= biWidth)
			{
				x=0;
				y++;
			}
	
			
		}

	}

//--(end)--- load 8bpp - BI_RLE8 encoding ----------------------


//---------- load 24bpp - BI_RGB encoding ----------------------

	//stored in BGR
	if ( (biBitCount==24) && (biCompression==BI_RGB) )
	{
		image = new unsigned char[biWidth*biHeight*4];
	
		cl_assert(image != NULL);

		for (int y=biHeight-1; y>-1; y--)
		//for (int y=0; y<biHeight; y++)
		{
			//for (int x=biWidth-1; x>-1; x--)
			for (int x=0; x<biWidth; x++)
			{
				image[(x+y*biWidth)*4 + 0] = 255;/*datafile->read_uchar8();*/ // alpha component of pixel
				image[(x+y*biWidth)*4 + 1] = datafile->read_uchar8(); // blue component
				image[(x+y*biWidth)*4 + 2] = datafile->read_uchar8(); // green component
				image[(x+y*biWidth)*4 + 3] = datafile->read_uchar8(); // red component
			}
		}
	
	}
//--(end)--- load 24bpp - BI_RGB encoding ----------------------




		/* If there are no masks, use the defaults */
	//	if ( bfOffBits == (2+biSize) )
	//	{
		/* Default values for the BMP format */
/*		switch (biBitCount)
		{
			case 15:
			case 16:
				Rmask = 0x7C00;
				Gmask = 0x03E0;
				Bmask = 0x001F;
				break;
			case 24:
			case 32:
				Bmask = 0x00FF0000;
				Gmask = 0x0000FF00;
				Rmask = 0x000000FF;
				break;
			default:
				break;
		}
		break;
	//	}
	}
			
*/
		/* Fall through -- read the RGB masks */
/*		case BI_BITFIELDS:
			switch (biBitCount)
			{
				case 15:
				case 16:
				case 24:
				case 32:
					Rmask = datafile->read_uint32();//SDL_ReadLE32(src);
					Gmask = datafile->read_uint32();//SDL_ReadLE32(src);
					Bmask = datafile->read_uint32();//SDL_ReadLE32(src);
					break;
				default:
					break;
			}
			break;
		default:
			std::cout << "Compressed BMP files not supported";
			break;
	
	}


			std::cout << std::endl << "Rmask = " << Rmask;

			std::cout << std::endl << "Gmask = " << Gmask;

			std::cout << std::endl << "Bmask = " << Bmask;

			std::cout << std::endl << "biSize = " << biSize;

	// Create a compatible surface, note that the colors are RGB ordered
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
			biWidth, biHeight, biBitCount, Rmask, Gmask, Bmask, 0);
	if ( surface == NULL )
	{
		was_error = 1;
		goto done;
	}
*/

//---------- load 8bpp ------------------------------
	



//---------- load 8bpp ------------------------------
/*
		std::cout << std::endl
		<< "bfSize = " << bfSize << std::endl
		<< "bfReserved1 = " << bfReserved1 << std::endl
		<< "bfReserved2 = " << bfReserved2 << std::endl
		<< "bfOffBits = " << bfOffBits << std::endl
		<< "biWidth = " << biWidth << std::endl
		<< "biHeight = " << biHeight << std::endl
		<< "biPlanes = " << biPlanes << std::endl
		<< "biBitCount = " << biBitCount << std::endl
		<< "biCompression = " << biCompression << std::endl
		<< "biSizeImage = " << biSizeImage << std::endl
		<< "biXPelsPerMeter = " << biXPelsPerMeter << std::endl
		<< "biYPelsPerMeter = " << biYPelsPerMeter << std::endl
		<< "biClrUsed = " << biClrUsed << std::endl
		<< "biClrImportant = " << biClrImportant << std::endl;
		
*/
	/* Read the surface pixels.  Note that the bmp image is upside down */
/*	if ( SDL_RWseek(src, fp_offset+bfOffBits, SEEK_SET) < 0 )
	{
		SDL_Error(SDL_EFSEEK);
		was_error = 1;
		goto done;
	}
	bits = (Uint8 *)surface->pixels+(surface->h*surface->pitch);
	switch (ExpandBMP)
	{
		case 1:
		case 4:
			pad  = (((surface->pitch/(8/ExpandBMP))%4) ?
				(4-((surface->pitch/(8/ExpandBMP))%4)) : 0);
			break;
		default:
			pad  = ((surface->pitch%4) ?
					(4-(surface->pitch%4)) : 0);
			break;
	}
	while ( bits > (Uint8 *)surface->pixels )
	{
		bits -= surface->pitch;
		switch (ExpandBMP) {
			case 1:
			case 4: {
			Uint8 pixel = 0;
			int   shift = (8-ExpandBMP);
			for ( i=0; i<surface->w; ++i )
			{
				if ( i%(8/ExpandBMP) == 0 ) {
					if ( !SDL_RWread(src, &pixel, 1, 1) ) {
						SDL_SetError(
					"Error reading from BMP");
						was_error = 1;
						goto done;
					}
				}
				*(bits+i) = (pixel>>shift);
				pixel <<= ExpandBMP;
			} }
			break;

			default:
			if ( SDL_RWread(src, bits, 1, surface->pitch)
							 != surface->pitch ) {
				SDL_Error(SDL_EFREAD);
				was_error = 1;
				goto done;
			}
			break;
		}
*/		/* Skip padding bytes, ugh */
/*		if ( pad ) {
			Uint8 padbyte;
			for ( i=0; i<pad; ++i ) {
				SDL_RWread(src, &padbyte, 1, 1);
			}
		}
	}
done:
	if ( was_error ) {
		if ( surface ) {
			SDL_FreeSurface(surface);
		}
		surface = NULL;
	}
	if ( freesrc && src ) {
		SDL_RWclose(src);
	}
	return(surface);
*/
///}
	
	//////////////////////////*/*/**/*//*/*/*/*//*	
	
	
//	pitch = width = biWidth;
//	height = biHeight;

	//datafile->seek(4, CL_InputSource::seek_set);
	
//	int bit_per_pixel = datafile->read_char8();
//	int dest_num_planes = num_planes;
/*
	switch (bit_per_pixel)
	{
	case 1: //1bit  (monochrome colors)
		//pixelformat = ;
		break;
	case 4: //4bit (16 colors)
		//pixelformat = ;
		break;
	case 8: //8bit (256 colors)
		pixelformat = PAL8;
		break;
	case 24: //24bit (2^24 colors)
		//pixelformat = RGBA8888;
	//	dest_num_planes = 4;
		break;
	default:
		cl_assert(false);
		break;
	}
*/
//	image = new unsigned char[pitch*height/* *dest_num_planes */];
	


	
/*	
	//swap the data
	for(int j=0;j<biSizeImage;j++)
	{
		image[j] = swap[biSizeImage-j];
	}
*/
//	int read = datafile->read(swap, biSizeImage);
//	if (read != biSizeImage)
//		throw CL_Error("Invalid BMP file!?");
//	cl_assert(read == biSizeImage);
	



//	for (int y=0;y<height;y++)
//	{
//		for (int x=0;x<get_width();x++)
//		{
//				image[(x+y*height/*get_width()*/)/**4*/ + 0] = datafile->read_uchar8(); // alpha component of pixel
			//	image[(x+y*get_width())/**4*/ + 1] =   x; // blue component
			//	image[(x+y*get_width())/**4*/ + 2] =   y; // green component
			//	image[(x+y*get_width())/**4*/ + 3] = x+y; // red component


//		}
//	}

	/*if (dest_num_planes == 4)
	{
		memset(image, 255, pitch*height*dest_num_planes);
	}
*/
//	datafile->seek(bfOffBits, CL_InputSource::seek_set);

	//int size_data = bfSize - bfOffBits;//datafile->size() - size_of_the_structure;
	
//	unsigned char *temp = new unsigned char[biSizeImage];
	//int read = datafile->read(image, biSizeImage);
	//if (read != biSizeImage)
	//	throw CL_Error("Invalid BMP file!?");
	//cl_assert(read == biSizeImage);
	
	//delete datafile;
	
//	unsigned char *p = temp;

//	unsigned char *cur_line = image;
	/*
	for (int y=0;y<height;y++)
	{
		for (int plane=0;plane<num_planes;plane++)
		{
			int x = 0;
			while (x < pitch)
			{
				unsigned char packed_byte = *(p++);
				if (packed_byte<192)
				{
					cur_line[(x*dest_num_planes)+plane] = packed_byte;

					if (transparent && packed_byte != trans_col)
					{
						if (x < bounding_left) bounding_left = x;
						if (y < bounding_top) bounding_top = y;
						if (x > bounding_right) bounding_right = x;
						if (y > bounding_bottom) bounding_bottom = y;
					}
					x++;
				}
				else 
				{ 
					unsigned char counter = packed_byte&0x3f;
					packed_byte = *(p++);
					for(unsigned char j=0;j<counter;j++)
					{
						cur_line[(x+j)*dest_num_planes+plane] = packed_byte;
						if (transparent && packed_byte != trans_col)
						{
							if ((x+j) < bounding_left) bounding_left = x+j;
							if (y < bounding_top) bounding_top = y;
							if ((x+j) > bounding_right) bounding_right = x+j;
							if (y > bounding_bottom) bounding_bottom = y;
						}
					}
					x += counter;
				}
			}
		}
		cur_line += dest_num_planes * pitch;
	}
*/
	
//	if (bit_per_pixel == 8)
//	{
	//	palette = new CL_Palette((image+biSizeImage-768));
//	}

//	else
//	{
//		palette = NULL;
//	}

//	delete[] temp;
//	delete[] image;
	delete datafile;
}

void CL_BMPProvider::perform_unlock()
{
	delete[] image;
	delete palette;

	image = NULL;
	palette = NULL;
}
