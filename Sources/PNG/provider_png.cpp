/*
	$Id: provider_png.cpp,v 1.29 2002/03/23 07:09:26 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------



*/

/* Known Bugs:
  ~~~~~~~~~~~~ 
   - this provider might not work with 16bit PNG's 
   - this provider might not work with some grayscale PNG's (don't remember which one)
*/

#include "Core/precomp.h"
#include "API/Display/Display/palette.h"
#include "API/Display/Display/res_surface.h"
#include "API/Core/Resources/resourceoptions.h"
#include "API/Core/System/error.h" 
#include "API/PNG/provider_png.h"

// Workaround for a VC bug
#ifdef WIN32
#  define for if(0);else for
#endif /* WIN32 */

CL_Surface *CL_PNGProvider::create(CL_String file, CL_InputSourceProvider *provider, 
				   bool transparent,
				   bool ignore_alphachannel)
{
	return CL_Surface::create(new CL_PNGProvider(file, provider, 
						     transparent, ignore_alphachannel), true);
}

CL_PNGProvider::CL_PNGProvider(CL_String name, CL_InputSourceProvider *_provider,
			       bool _transparent,
			       bool _ignore_alphachannel)
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

	indexed = true;
	palette = 0;

	trans_redcol = 0;
	trans_greencol = 0;
	trans_bluecol = 0;

	trans_col = -1;
	m_uses_src_colorkey = false;

	locked = 0;
	filename = name;
	image = NULL;

	lock();
	unlock();
}

CL_PNGProvider::~CL_PNGProvider()
{
	perform_unlock();
	delete provider;
}

unsigned int CL_PNGProvider::get_red_mask() const
{
	if (pixel_format == PAL8)
		return CL_Color::get_red_mask(RGBA8888); // Color mask don't make sense with indexed images
	else
		return CL_Color::get_red_mask(pixel_format);
}

unsigned int CL_PNGProvider::get_green_mask() const
{
	if (pixel_format == PAL8)
		return CL_Color::get_green_mask(RGBA8888); // Color mask don't make sense with indexed images
	else
		return CL_Color::get_green_mask(pixel_format);
}

unsigned int CL_PNGProvider::get_blue_mask() const
{
	if (pixel_format == PAL8)
		return CL_Color::get_blue_mask(RGBA8888); // Color mask don't make sense with indexed images
	else
		return CL_Color::get_blue_mask(pixel_format);
}

unsigned int CL_PNGProvider::get_alpha_mask() const
{
	if (pixel_format == PAL8)
		return CL_Color::get_alpha_mask(RGBA8888); // Color mask don't make sense with indexed images
	else
		return CL_Color::get_alpha_mask(pixel_format);
}

bool CL_PNGProvider::is_indexed() const
{
  return pixel_format == PAL8;
}

unsigned int CL_PNGProvider::get_depth() const
{
	switch (pixel_format)
	{
	case RGBA8888:
		return 32;
	case RGB888:
		return 24;
	case PAL8:
		return 8;
	default:
		std::cerr << "CL_PNGProvider::get_depth: Unhandled pixel format: " << pixel_format << std::endl;
		return 32;
	}
}
  
void CL_PNGProvider::read_data()
{
	// initial fileinfo
	png_read_info(png_ptr, info_ptr); 
	
	// reduce 16bit/channel to 8Bit/channel
	png_set_strip_16(png_ptr);  
	
	// reread infostruct to reflect the made settings
	png_read_update_info(png_ptr, info_ptr); 

	width  = png_get_image_width(png_ptr,info_ptr);
	height = png_get_image_height(png_ptr,info_ptr);
	color_type = png_get_color_type(png_ptr,info_ptr);

	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		read_data_grayscale ();
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		read_data_grayscale_alpha ();
		break;
	case PNG_COLOR_TYPE_PALETTE:
		read_data_palette ();
		break;
	case PNG_COLOR_TYPE_RGB:
		read_data_rgb ();
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		read_data_rgba ();
		break;
	default:
		throw CL_Error ("CL_PNGProvider: Unsupported PNG format!");
		break;
	}
}

void CL_PNGProvider::read_data_rgb()
{
	indexed = false;
	pixel_format = RGB888;

	pitch = png_get_rowbytes(png_ptr, info_ptr);

	// This is deleted in the unlock () call
	image = new unsigned char[pitch * height];

	// setup pointers to each row in our target image
	png_bytep* row_pointers = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = image + (pitch * y);
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	// swap the colors in the right order
	unsigned char tmp;
	for (int i = 0; i < width * height; ++i)
	{
		tmp = image[3*i + 2];
		image[3*i + 2] = image[3*i + 0];
		image[3*i + 0] = tmp;
	}
}

void CL_PNGProvider::read_data_rgba()
{
	indexed = false;
	pixel_format = RGBA8888;

	pitch = png_get_rowbytes(png_ptr, info_ptr);

	// This is deleted in the unlock () call
	image = new unsigned char[pitch * height];

	unsigned char* tmp_image = new unsigned char[pitch * height];
	// setup pointers to each row in our target image
	png_bytep* row_pointers = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = tmp_image + (pitch * y);
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	if (!ignore_alphachannel)
	{
		for (int i = 0; i < pitch * height; i += 4)
		{
			image[i + 0] = tmp_image[i + 3];
			image[i + 1] = tmp_image[i + 2];
			image[i + 2] = tmp_image[i + 1];
			image[i + 3] = tmp_image[i + 0];
		}
	}
	else
	{
		for (int i = 0; i < pitch * height; i += 4)
		{
			image[i + 0] = 255;
			image[i + 1] = tmp_image[i + 2];
			image[i + 2] = tmp_image[i + 1];
			image[i + 3] = tmp_image[i + 0];
		}
	}

	delete[] tmp_image;
}

void CL_PNGProvider::read_data_grayscale()
{
	pixel_format = RGB888;
	indexed = false;

	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes  = png_get_rowbytes(png_ptr, info_ptr);

	pitch = rowbytes * 3;

	// We expand the grayscale values if necessare, so we always
	// get 8bits per pixel
	if (bit_depth < 8) png_set_expand (png_ptr);

	// Allocating the temporary buffer and fill it
	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	png_bytep* row_pointers  = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = tmp_image + (rowbytes * y);
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	image = new unsigned char[height * pitch];
	// Coverting the data in the tmp buffer to our final data
	for (int i = 0; i < rowbytes * height; i++)
	{
		image[3*i + 0] = tmp_image[i];
		image[3*i + 1] = tmp_image[i];
		image[3*i + 2] = tmp_image[i];
	}
	delete[] tmp_image;
}

void CL_PNGProvider::read_data_grayscale_alpha()
{
	pixel_format = RGBA8888;
	pitch = width * 4;
	indexed = false;

	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes  = png_get_rowbytes(png_ptr, info_ptr);

	// We expand the grayscale values if necessare, so we always
	// get 8bits per pixel
	if (bit_depth < 8) png_set_expand (png_ptr);

	// Allocating the temporary buffer
	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	png_bytep* row_pointers = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = tmp_image + (rowbytes * y);
  	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	// Creating the final image out of tmp_image
	image = new unsigned char[width * pitch];
	if (!ignore_alphachannel)
	{
		for (int i = 0; i < rowbytes * height; i += 2)
		{
			image[2*i + 0] = tmp_image[i + 1];
			image[2*i + 1] = tmp_image[i + 0];
			image[2*i + 2] = tmp_image[i + 0];
			image[2*i + 3] = tmp_image[i + 0];
		}
	} 
	else
	{
		for (int i = 0; i < rowbytes * height; i += 2)
		{
			image[2*i + 0] = 255;
			image[2*i + 1] = tmp_image[i + 0];
			image[2*i + 2] = tmp_image[i + 0];
			image[2*i + 3] = tmp_image[i + 0];
		}
	}

	delete[] tmp_image;
}

void CL_PNGProvider::read_data_palette()
{
	pixel_format = PAL8;
	indexed = true;
	palette = new CL_Palette ();

	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	// Allocating the temporary buffer (will be deleted some
	// screens below
	png_bytep* row_pointers = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = tmp_image + (rowbytes * y);
	png_read_image(png_ptr, row_pointers);
	
	if (bit_depth == 8)
	{
		// We don't need to convert the data, so we can use
		// what we got
		image = tmp_image;
		pitch = rowbytes;
	}
	else
	{
		// We need to convert the data
		pitch = width;

    int y,x;

		switch (bit_depth)
		{
		case 1:
			image = new unsigned char[height * rowbytes * 8];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
					image[y*pitch + 8*x + 0] = row_pointers[y][x] >> 7;
					image[y*pitch + 8*x + 1] = row_pointers[y][x] >> 6 & 0x1;
					image[y*pitch + 8*x + 2] = row_pointers[y][x] >> 5 & 0x1;
					image[y*pitch + 8*x + 3] = row_pointers[y][x] >> 4 & 0x1;
					image[y*pitch + 8*x + 4] = row_pointers[y][x] >> 3 & 0x1;
					image[y*pitch + 8*x + 5] = row_pointers[y][x] >> 2 & 0x1;
					image[y*pitch + 8*x + 6] = row_pointers[y][x] >> 1 & 0x1;
					image[y*pitch + 8*x + 7] = row_pointers[y][x] & 0x1;
				}
			}
			break;
		case 2:
			image = new unsigned char[height * rowbytes * 4];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
						image[y*pitch + 4*x + 0] = row_pointers[y][x] >> 6;
						image[y*pitch + 4*x + 1] = row_pointers[y][x] >> 4 & 0x3;
						image[y*pitch + 4*x + 2] = row_pointers[y][x] >> 2 & 0x3;
						image[y*pitch + 4*x + 3] = row_pointers[y][x] & 0x3;
				}
			}
			break;
		case 4:
			image = new unsigned char[height * rowbytes * 2];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
						image[y*pitch + 2*x + 0] = row_pointers[y][x] >> 4;
						image[y*pitch + 2*x + 1] = row_pointers[y][x] & 0x0f;
				}
			}
			break;
		default:
			throw CL_Error ("CL_PNGProvider: Unhandled bit depth");
		}
		delete[] tmp_image;
	}
	delete[] row_pointers;

	// Read the png palette and create the CL_Palette 
	png_colorp png_palette;
	png_get_PLTE(png_ptr, info_ptr, &png_palette, &palette->num_colors);
	palette->palette = new unsigned char [palette->num_colors * 3];
	for (int k = 0; k < palette->num_colors; k++)
	{
		palette->palette[k*3 + 0] = png_palette[k].red;
		palette->palette[k*3 + 1] = png_palette[k].green;
		palette->palette[k*3 + 2] = png_palette[k].blue;
	}

	// Reading and setting up the transparent colors from the image
	int num_trans = 0;
	png_color_16p trans_values;
	png_bytep trans;
	png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);
	
	// Setting up the transparent color
	if (num_trans == 1)
	{
		trans_col = trans[0];
		m_uses_src_colorkey = true;
	}
	else if (num_trans > 1)
	{
		trans_col = trans[0];
		// We collaps all transparent colors to a single one
		for (int j = 0; j < pitch * height; j++)
			for (int i = 0; i < num_trans; i++)
			{
				if (trans[i] == image[j])
					image[j] = trans_col;
			}
		m_uses_src_colorkey = true;			
	}
}

void* CL_PNGProvider::get_data() const
{
	return image;
}

/*
	Lock the surfaceprovider - which basically means open the file
	and read the image into a temporary memory buffer - until
	unlock() is called.
*/
void CL_PNGProvider::perform_lock()
{
	if (locked) return;

	//setting up PNGLIB stuff
	png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr) 
	  throw CL_Error ("CL_PNGProvider: png_create_read_struct() failed");
	
	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,
					(png_infopp)NULL, (png_infopp)NULL);
		throw CL_Error ("CL_PNGProvider: png_create_info_struct() failed");
	}

	end_info = png_create_info_struct(png_ptr);

	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
					(png_infopp)NULL);
		cl_assert(false);
	}   
	if (setjmp(png_ptr->jmpbuf))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		cl_assert(false);
	}  
	
	cl_assert(provider != NULL);
	input_source = provider->open_source(filename);
	cl_assert(input_source!=NULL);

	// tell libpng form whom it get the fileData
	png_set_read_fn(png_ptr, this, &CL_PNGProvider::pngread_file);

	// reading the header infos and actually read data ...
	read_data();

	// remove our data_provider from libpng
	png_set_read_fn(png_ptr,NULL,NULL);

	// free memory ...
	png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);

	delete input_source;

	locked++;
}

void CL_PNGProvider::perform_unlock()
{
	locked--;

	delete[] image;

	if (palette) delete palette;

	palette = NULL;
	image   =  NULL;
}

// Resource support:
class CL_PNG_ResourceSource : public CL_ResourceSource_Surface
{
public:
	virtual const char *get_name() { return "png"; }

	virtual bool can_create(std::string ext,
		CL_ResourceOptions &options)
	{
		if (ext == ".png") return true;
		if (options.exists("png")) return true;

		return false;
	}

	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)
	{
		return new CL_PNGProvider(resource.get_full_location().c_str(), NULL);
	}

	virtual CL_SurfaceProvider *create(const std::string &filename)
	{
		return new CL_PNGProvider(filename.c_str(), NULL);
	}
};

static CL_PNG_ResourceSource *res_source_png = NULL;

void CL_SetupPNG::init(bool register_resources_only)
{
	res_source_png = new CL_PNG_ResourceSource;
}

void CL_SetupPNG::deinit()
{
	delete res_source_png;
	res_source_png = NULL;
}
