/*
	$Id: font_description.cpp,v 1.9 2002/03/02 19:43:46 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Simple font support.
*/

#include "Core/precomp.h"
#include "API/Display/Display/pixeldata.h"
#include "API/Display/SurfaceProviders/sprite_subarray_provider.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/SurfaceProviders/sprite2.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Display/Font/font_description.h"

CL_Font_Description::CL_Font_Description(CL_InputSource *input)
: source(NULL)
{
	space_len = input->read_int32();
	subtract_width = input->read_int32();
	letters = input->read_string();
	
	int len = letters.length();
	for (int i=0; i<len; i++)
	{
		letter_providers.push_back(new CL_Sprite2Provider(input));
	}
}

CL_Font_Description::CL_Font_Description(
	CL_SurfaceProvider *p,
	int x, int y,
	int *tcols, int num_tcols,
	int spacelen,
	int subtract_width,
	const std::string &letters)
{
	// this constructor handles the _old_ pcx font system.

	this->source = p;
	this->space_len = spacelen;
	this->subtract_width = subtract_width;
	this->letters = letters;
		
	p->lock();

	if (p->get_depth() != 8)
	{
		p->unlock();
		throw CL_Error("Old font method only works on PAL8 images.");
	}
		
	unsigned char *surface_data = (unsigned char *) p->get_data();

	// Find height
	unsigned int h;
	for (h=y; h<p->get_height(); h++)
	{
		if (surface_data[h*p->get_pitch()+x]==255)
		{
			h-=y;
			break;
		}
	}
	
	// Load letters
	int org_x=x;
	int next_line=0;
	int pos=0;
	unsigned char bogst_nr=0;

	bogst_nr=letters[pos];
		
	while (bogst_nr != 0)
	{
		// Find width
		int b;
		int p_width = p->get_width();
		for (b=x; b<p_width; b++)
		{
			if (surface_data[y*p->get_pitch()+b]==254)
			{
				b-=x;
				break;
			}
			else if (surface_data[y*p->get_pitch()+b]==253)
			{
				b-=x;
				next_line=-1;
				break;
			}
		}

		letter_providers.push_back(
		new CL_Sprite2Provider(
			p,
			x, y,
			b, h,
			tcols, num_tcols));
		x+=b+1;

		if (next_line)
		{
			x=org_x;
			y+=h+1;
			next_line=0;
		}

		pos++;
		bogst_nr=letters[pos];
	}

	p->unlock();
}

CL_Font_Description::CL_Font_Description(
	CL_SurfaceProvider *source,
	float trans_limit,
	int space_len,
	int subtract_width,
	const std::string &letters)
{
	this->source = source;
	this->space_len = space_len;
	this->subtract_width = subtract_width;
	this->letters = letters;

	int begin=0;
	bool prev_trans=true;
		
	CL_PixelData alpha(0, 0, 0, 255, source, 1);

	int *opaque_row = new int[alpha.get_width()];
	memset(opaque_row, 0, alpha.get_width()*sizeof(int));

	int alpha_width = alpha.get_width();
	int alpha_height = alpha.get_height();
	bool found_opaque = false;
	bool found_trans = false;
		
	int cut_top = 0;
	int cut_bottom = alpha_height;
		
	for (int y=0; y < alpha_height; y++)
	{
		bool opaque_line = false;
		unsigned char *line = alpha.get_line_pixel(y);
		for (int x=0; x < alpha_width; x++)
		{
			if (line[x] > trans_limit*255)
			{
				opaque_row[x] = 1;
				opaque_line = true;
				found_opaque = true;
			}
		}
			
		if (opaque_line == false) // cut something of top or bottom
		{
			if (found_opaque)
			{
				cut_bottom--;
				found_trans = true;
			}
			else
				cut_top ++;
		}
		else if (found_trans)
		{
			found_trans = false;
			cut_bottom = alpha_height;
		}
	}
		
	if (cut_top >= cut_bottom)
		throw CL_Error("Font image contained only alpha! (or the alpha clipper is broken)");

	for(int x=0; x < alpha_width; x++)
	{
		if(opaque_row[x] && prev_trans)
		{
			begin = x;
			prev_trans = false;
		}
		else if (!opaque_row[x] && !prev_trans)
		{
			letter_providers.push_back(
				new CL_SpriteSubarrayProvider(
					source,
					begin,
					cut_top,
					x-begin+1,
					cut_bottom-cut_top,
					1, 1));

			prev_trans = true;
		}
	}
		
	if (!prev_trans)
	{
		letter_providers.push_back(
			new CL_SpriteSubarrayProvider(
				source,
				begin,
				cut_top,
				alpha.get_width()-begin,
				cut_bottom-cut_top,
				1, 1));
	}

	delete[] opaque_row;

	// if you get this assertion, then it is because you have more or
	// less letters in your font than in your font description.
	if(letter_providers.size() != this->letters.size())
	{
		CL_String error;
		error << "Imagefile characters: "
		      << (int)letter_providers.size()
		      << " Resourcefile characters: "
		      << (int)this->letters.size();

		throw CL_Error(error.get_string());
	}
//	cl_assert(letter_providers.size() == this->letters.size());
}

CL_Font_Description::~CL_Font_Description()
{
	for(std::vector<CL_SurfaceProvider*>::iterator it = letter_providers.begin(); it != letter_providers.end(); it++)
	{
		delete *it;
	}
	letter_providers.clear();
}

void CL_Font_Description::lock()
{
	if (source != NULL) source->lock();
}

void CL_Font_Description::unlock()
{
	if (source != NULL) source->unlock();
}
