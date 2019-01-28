/*
	$Id: provider_jpeg_generic.cpp,v 1.6 2001/11/01 13:56:51 sphair Exp $
	
	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#define WIN32_LEAN_AND_MEAN

#include "provider_jpeg_generic.h"
#include <setjmp.h>
#include <jerror.h>

#define INPUT_BUF_SIZE  4096

CL_JPEGProvider_Generic::CL_JPEGProvider_Generic(
	CL_String name,
	CL_InputSourceProvider *_provider,
	bool _transparent,
	bool _ignore_alphachannel)
{
	if (_provider == NULL)
	{
		input_provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		input_provider = _provider->clone();
	}

	ignore_alphachannel = _ignore_alphachannel;
	transparent = _transparent;
	use_alphapixels = transparent && !ignore_alphachannel;

	trans_redcol = 0;
	trans_greencol = 0;
	trans_bluecol = 0;
		
	locked = 0;
	filename = name;
	image = NULL;
}

CL_JPEGProvider_Generic::~CL_JPEGProvider_Generic()
{
	perform_unlock();
	delete input_provider;
}

unsigned int CL_JPEGProvider_Generic::get_red_mask() const
{
	return CL_Color::get_red_mask(RGB888);
}

unsigned int CL_JPEGProvider_Generic::get_green_mask() const
{
	return CL_Color::get_green_mask(RGB888);
}

unsigned int CL_JPEGProvider_Generic::get_blue_mask() const
{
	return CL_Color::get_blue_mask(RGB888);
}

unsigned int CL_JPEGProvider_Generic::get_alpha_mask() const
{
	return CL_Color::get_alpha_mask(RGB888);
}
  
void *CL_JPEGProvider_Generic::get_data() const
{
	return image;
}

/*
	Lock the surfaceprovider - which basically means open the file
	and read the image into a temporary memory buffer - until
	unlock() is called.
*/
void CL_JPEGProvider_Generic::perform_lock()
{
	if (locked++) return;

	struct jpeg_error_mgr jerr;	
	struct jpeg_decompress_struct cinfo;
	//	FILE * infile;		
	JSAMPARRAY buffer;	
	int row_stride;		
	
	input_source = input_provider->open_source(filename);

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_InputSource_src(&cinfo, this);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;

	width = cinfo.output_width;
	height = cinfo.output_height;
	pitch = width * 3;

	// FIXME: Where is the allocated memory freed?
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 32);

	image = new unsigned char[get_pitch() * cinfo.output_height];

	// RGB Image
	if (cinfo.output_components == 3)
	{
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
		
			for(unsigned int i=0; i < get_pitch(); i += 3)
			{
				image[pitch * (cinfo.output_scanline - 1) + i + 0] = buffer[0][i + 2];
				image[pitch * (cinfo.output_scanline - 1) + i + 1] = buffer[0][i + 1];
				image[pitch * (cinfo.output_scanline - 1) + i + 2] = buffer[0][i + 0];
			}
		}
	}
	// Greyscale Image
	else if (cinfo.output_components == 1)
	{
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
			
			for(int i=0; i < width; i += 1)
			{
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 0] = buffer[0][i];
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 1] = buffer[0][i];
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 2] = buffer[0][i];
			}			
		}
	}
	else
	{
//		std::cout << "CL_JPEGProvider: Unsupported color completion: " << cinfo.output_components << std::endl;
		cl_assert(false);
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete input_source;
	input_source = 0;
}

void CL_JPEGProvider_Generic::perform_unlock()
{
	if (!(--locked))
	{
		delete[] image;
		image = NULL;
	}
}

void CL_JPEGProvider_Generic::jpeg_InputSource_src (j_decompress_ptr cinfo, CL_JPEGProvider_Generic* provider)
{
	InputSource_src_ptr src;
	
	if (cinfo->src == NULL) /* first time for this JPEG object? */
	{
		cinfo->src = (struct jpeg_source_mgr*)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
						    sizeof(InputSource_source_mgr));
		src = (InputSource_src_ptr) cinfo->src;
		src->buffer = (JOCTET *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
						    INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	src = (InputSource_src_ptr) cinfo->src;
	src->pub.init_source = CL_JPEGProvider_Generic::init_source;
	src->pub.fill_input_buffer = CL_JPEGProvider_Generic::fill_input_buffer;
	src->pub.skip_input_data = CL_JPEGProvider_Generic::skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = CL_JPEGProvider_Generic::term_source;
	src->jpeg_provider = provider;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
}

void CL_JPEGProvider_Generic::init_source (j_decompress_ptr cinfo)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	src->start_of_file = TRUE;	
}

boolean CL_JPEGProvider_Generic::fill_input_buffer (j_decompress_ptr cinfo)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	size_t nbytes;

	nbytes = src->jpeg_provider->input_source->read (src->buffer, INPUT_BUF_SIZE);
	
	if (nbytes <= 0) 
	{
		if (src->start_of_file)	/* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;
	
	return TRUE;
}

void CL_JPEGProvider_Generic::skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	
	/* Just a dumb implementation for now.  Could use fseek() except
	 * it doesn't work on pipes.  Not clear that being smart is worth
	 * any trouble anyway --- large skips are infrequent.
	 */
	if (num_bytes > 0)
	{
		while (num_bytes > (long) src->pub.bytes_in_buffer)
		{
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never return FALSE,
			 * so suspension need not be handled.
			 */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

void CL_JPEGProvider_Generic::term_source (j_decompress_ptr cinfo)
{
	// do nothing
}
