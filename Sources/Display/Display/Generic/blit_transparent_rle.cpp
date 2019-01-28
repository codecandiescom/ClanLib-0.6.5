/*
	$Id: blit_transparent_rle.cpp,v 1.2 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <Display/Display/Generic/blit_transparent_rle.h>
#include "API/Display/Display/surfaceprovider.h"

CL_Blit_Transparent_RLE::CL_Blit_Transparent_RLE(
	CL_SurfaceProvider *provider,
	int bytes_per_pixel)
{
	width = provider->get_width();
	height = provider->get_height();
	no_sprs = provider->get_num_frames();

	this->bytes_per_pixel = bytes_per_pixel;
	
	rle_lines = new unsigned char*[height*no_sprs];	
}


CL_Blit_Transparent_RLE::~CL_Blit_Transparent_RLE()
{
	for (int y=0; y<height; y++) delete[] rle_lines[y];
	delete[] rle_lines;
}


int CL_Blit_Transparent_RLE::calc_rle_size(unsigned char *alpha_line)
{
	int size = 0;
	
	int last_state = cmd_end;
	for (int x=0; x<width; x++)
	{
		switch(alpha_line[x])
		{
		case 0 :   // transparent pixel		
		
			if (last_state != cmd_skip) size += 3; // cmd_skip+word
			last_state = cmd_skip;
			break;
		
		case 255:  // opaque pixel
		
			if (last_state != cmd_copy) size += 3; // cmd_skip+word
			last_state = cmd_copy;
			size += bytes_per_pixel;
			break;
		
		default:   // alpha value
			
			if (last_state != cmd_alpha) size += 3; // cmd_alpha+word
			last_state = cmd_alpha;
			size += bytes_per_pixel + 1; // pixel data + alpha value
			break;
		}       
	}
	
	size++; // cmd_end
	
	return size;
}


void CL_Blit_Transparent_RLE::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	target->lock();
	
	unsigned int dest_bytes_per_pixel = (target->get_depth()+7)/8;
	unsigned int dest_pitch = target->get_pitch();
	unsigned char *dest = (unsigned char *) target->get_data();

	dest += x*dest_bytes_per_pixel + y*dest_pitch;

	int begin = height*spr_no;
	for (int yy=0; yy<height; yy++)
	{
		blt_line(rle_lines[yy+begin], dest);
		dest += dest_pitch;
	}

	target->unlock();
}
