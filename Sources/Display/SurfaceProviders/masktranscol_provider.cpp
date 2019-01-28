/*
	$Id: masktranscol_provider.cpp,v 1.5 2001/12/15 21:26:29 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "Core/precomp.h"

#include "masktranscol_provider.h"
#include "API/Core/System/cl_assert.h"

CL_MaskTranscolProvider::CL_MaskTranscolProvider(
	CL_SurfaceProvider *_parent,
	bool _delete_provider,
	const unsigned int * _mask_indices,
	int _num_mask_indices)
{
	parent = _parent;
	cl_assert(parent != NULL);
	//cl_assert(parent->get_depth() == 8);

	delete_provider = _delete_provider;
	num_mask_indices = _num_mask_indices;
	mask_indices = new unsigned int[num_mask_indices];
	memcpy(mask_indices, _mask_indices, num_mask_indices*sizeof(unsigned int));
}

CL_MaskTranscolProvider::~CL_MaskTranscolProvider()
{
	if (delete_provider) delete parent;
	delete[] mask_indices;
}

unsigned int CL_MaskTranscolProvider::get_pitch() const
{
	return parent->get_pitch();
}

int CL_MaskTranscolProvider::get_translate_x() const
{
	return parent->get_translate_x();
}

int CL_MaskTranscolProvider::get_translate_y() const
{
	return parent->get_translate_y();
}

unsigned int CL_MaskTranscolProvider::get_depth() const
{
	return parent->get_depth();
}

bool CL_MaskTranscolProvider::is_indexed() const
{
	return parent->is_indexed();
}

unsigned int CL_MaskTranscolProvider::get_width() const
{
	return parent->get_width();
}

unsigned int CL_MaskTranscolProvider::get_height() const
{
	return parent->get_height();
}

unsigned int CL_MaskTranscolProvider::get_num_frames() const
{
	return parent->get_num_frames();
}

unsigned int CL_MaskTranscolProvider::get_red_mask() const
{
	return parent->get_red_mask();
}

unsigned int CL_MaskTranscolProvider::get_green_mask() const
{
	return parent->get_green_mask();
}

unsigned int CL_MaskTranscolProvider::get_blue_mask() const
{
	return parent->get_blue_mask();
}

unsigned int CL_MaskTranscolProvider::get_alpha_mask() const
{
	return parent->get_alpha_mask();
}

CL_Palette *CL_MaskTranscolProvider::get_palette() const
{
	return parent->get_palette();
}

unsigned int CL_MaskTranscolProvider::get_src_colorkey() const
{
	return mask_indices[0];
}

bool CL_MaskTranscolProvider::uses_src_colorkey() const
{
//	return parent->uses_src_colorkey();
	return true;
}

void *CL_MaskTranscolProvider::get_data() const
{
	return parent->get_data();
}

void CL_MaskTranscolProvider::perform_lock()
{
	parent->lock();

	switch (get_depth())
	{
	case 8:
		{
			int i;
			bool trans_table[256];
			for (i=0;i<256;i++) trans_table[i] = false;
			if (parent->uses_src_colorkey()) trans_table[parent->get_src_colorkey()] = true;
			for (i=0;i<num_mask_indices;i++) trans_table[mask_indices[i]] = true;

			unsigned char *pdata = (unsigned char *) parent->get_data();
			for (unsigned int s=0;s<parent->get_num_frames();s++)
			{
				for (unsigned int y=0;y<parent->get_height();y++)
				{
					for (unsigned int x=0;x<parent->get_width();x++)
					{
						if (trans_table[*pdata]) *pdata = mask_indices[0];
						pdata++;
					}
					pdata += parent->get_pitch()-parent->get_width();
				}
			}
		}
		break;

	case 15:
	case 16:
		{
			unsigned int frames = parent->get_num_frames();
			unsigned int height = parent->get_height();
			unsigned int width = parent->get_width();
			unsigned int pitch = parent->get_pitch();

			for (unsigned int s=0;s<frames;s++)
			{
				for (unsigned int y=0;y<height;y++)
				{
					unsigned char *data =
						((unsigned char *) parent->get_data()) + (y+s*height)*pitch;

					unsigned short *pdata = (unsigned short *) data;

					for (unsigned int x=0;x<width;x++)
					{
						for (int i=0; i<num_mask_indices; i++)
						{
							if (pdata[x] == mask_indices[i])
							{
								// Set the whole pixel to 0 (Even if we should only set the alpha value
								pdata[x] = 0;
							}
						}
					}
				}
			}
		}
		break;

	case 24:
		cl_assert(false); // not implemented in 24 bpp. That depth SUCKS! :-(
		break;

	case 32:
		{
			unsigned int frames = parent->get_num_frames();
			unsigned int height = parent->get_height();
			unsigned int width = parent->get_width();
			unsigned int pitch = parent->get_pitch();

			for (unsigned int s=0;s<frames;s++)
			{
				for (unsigned int y=0;y<height;y++)
				{
					unsigned char *data =
						((unsigned char *) parent->get_data()) + (y+s*height)*pitch;

					unsigned int *pdata = (unsigned int *) data;

					for (unsigned int x=0;x<width;x++)
					{
						for (int i=0; i<num_mask_indices; i++)
						{
							if (pdata[x] == mask_indices[i])
							{
								// Set the whole pixel to 0 (Even if we should only set the alpha value
								pdata[x] = 0;
							}
						}
					}
				}
			}
		}
		break;
	}
}

void CL_MaskTranscolProvider::perform_unlock()
{
	parent->unlock();
}
