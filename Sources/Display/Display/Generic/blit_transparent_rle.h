/*
	$Id: blit_transparent_rle.h,v 1.2 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/


#ifndef header_blit_transparent_rle
#define header_blit_transparent_rle

#include "blitters.h"

class CL_SurfaceProvider;

class CL_Blit_Transparent_RLE :
	public CL_Blit_NoClip
{
protected:
	int width, height, no_sprs;
	int bytes_per_pixel;
	unsigned char **rle_lines;
	
	enum
	{
		cmd_skip,     // next word = bytes to skip
		cmd_copy,     // next word = bytes to copy
		cmd_alpha,    // next word = _blocks_ to apply alpha
		cmd_end
	};
	
	int calc_rle_size(unsigned char *alpha_line);

	virtual unsigned char *make_rle_line(
	        unsigned char *src_line, 
		unsigned char *alpha_line) = 0;

	virtual void blt_line(
	        unsigned char *s, 
		unsigned char *d) = 0;

public:

	CL_Blit_Transparent_RLE(
		CL_SurfaceProvider *provider,
		int bytes_per_pixel);

	virtual ~CL_Blit_Transparent_RLE();

	virtual void blt_noclip(
		CL_Target *dest_surf,
		int x,
		int y,
		int spr_no);
};

#endif
