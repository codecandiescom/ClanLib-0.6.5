/*
	$Id: surfaceprovider_32bpp.h,v 1.7 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_surfaceprovider_32bpp
#define header_surfaceprovider_32bpp

#include "display.h"
#include "surfaceprovider.h"
#include "palette.h"
#include "../../Core/System/cl_assert.h"

//: Purpose: Convert to RGBA8888 to ensure support for colorkeying.
//-old old old old old old old old old old old old old old old old old old 
//-Purpose: Convert a PAL8 surface provider to 16 bpp. This is needed since
//-         Hermes doesn't support colorkeys (atleast not when this code
//-         was written).
class CL_SurfaceProvider_32bpp : public CL_SurfaceProvider
{
public:
//! Construction:
	//: Surface Provider 32bpp constructor
	CL_SurfaceProvider_32bpp(CL_SurfaceProvider *src)
	{
		src->lock();
		cl_assert(src->get_depth() == 8 || src->get_depth() == 32);

		m_width = src->get_width();
		m_height = src->get_height();
		m_no_sprs = src->get_num_frames();
		unsigned int pitch = src->get_pitch();
		int transcol = src->uses_src_colorkey() ? (int) src->get_src_colorkey() : -1;

		m_data = new unsigned int[pitch*m_height*m_no_sprs];
		
		switch (src->get_depth())
		{
			case 8:
			{
				unsigned char *ptr = (unsigned char *) src->get_data();

				CL_Palette *pal = src->get_palette();
				if (pal == NULL) pal = CL_Display::get_palette();
	
				for (int y=0; y<m_height*m_no_sprs; y++)
				{
					for (int x=0; x<m_width; x++)
					{
						int color = ptr[x+y*pitch];
			
						m_data[x+y*m_width] =
							(pal->palette[color*3+0] << 24) +
							(pal->palette[color*3+1] << 16) +
							(pal->palette[color*3+2] << 8);
				
						// Set alphamask according to transparency
						if (transcol == -1 || color != transcol)
						{
							m_data[x+y*m_width] += 255;
						}
					}
				}
				break;
			}
			
			case 32:
			{
				unsigned int *ptr = (unsigned int *) src->get_data();

				for (int y=0; y<m_height*m_no_sprs; y++)
				{
					for (int x=0; x<m_width; x++)
					{
						// BUG: possible pitch problem - but only theoretically
						int color = ptr[x+y*m_width];
						m_data[x+y*m_width] = color;
				
	    					// Set alphamask according to transparency
		    				if (transcol == -1 || color != transcol)
						{
							m_data[x+y*m_width] |= 255;
						}	
					}
				}
				break;
			}
			default :
				cl_assert(false);
		}
		src->unlock();
	}

	//: Surface Provider 32bpp destructor
	virtual ~CL_SurfaceProvider_32bpp()
	{
		delete[] m_data;
	}

//! Attributes:
	//: Get pitch
	virtual unsigned int get_pitch() const { return m_width*sizeof(int); }

	//: Get width
	virtual unsigned int get_width() const { return m_width; }

	//: Get height
	virtual unsigned int get_height() const { return m_height; }

	//: Get num frames
	virtual unsigned int get_num_frames() const { return m_no_sprs; }

	//: Get depth
	virtual unsigned int get_depth() const { return 32; }

	//: Get red mask
	virtual unsigned int get_red_mask() const { return 0xff000000; }

	//: Get green mask
	virtual unsigned int get_green_mask() const { return 0x00ff0000; }

	//: Get blue mask
	virtual unsigned int get_blue_mask() const { return 0x0000ff00; }

	//: Get alpha mask
	virtual unsigned int get_alpha_mask() const { return 0x000000ff; }

	//: Is indexed
	virtual bool is_indexed() const { return false; }

	//: Get palette
	virtual CL_Palette *get_palette() const { return NULL; }

	//: Lock
	virtual void lock() { return; }

	//: Unlock
	virtual void unlock() { return; }

	//: Get Data
	virtual void *get_data() const { return m_data; }

	//: Uses src colourkey
	virtual bool uses_src_colorkey() const { return false; }

	//: Get src colourkey
	virtual unsigned int get_src_colorkey() const { return 0; }
	
protected:
	unsigned int *m_data;
	int m_width, m_height, m_no_sprs;
};

#endif
