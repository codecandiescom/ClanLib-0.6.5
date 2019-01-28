/*
        $Id: blit_mga.cpp,v 1.5 2001/10/14 19:59:11 japj Exp $

        ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        ------------------------------------------------------------------------

	TODO:
		- m_alphablitter
		- colorkeying with other than 16 bits per pixel
		- 8 byte alignment for every sprite in the surface
		- (config option for limiting offscreen images)
*/

#include "Core/precomp.h"

#include <Display/Display/FBDev/blit_mga.h>
#include <Display/Display/Generic/blitters.h>
#include <API/Display/Display/cliprect.h>
#include <Display/Display/Generic/colormap.h>
#include <Display/Display/FBDev/display_fbdev.h>
#include <API/Display/Display/pixeldata.h>
#include <API/Display/Display/surfaceprovider.h>
#include <API/Display/Display/target.h>
#include <Display/Display/Generic/blit_alphamask_rle.h>

#include <linux/fb.h>

#include "regs.h"
#include "mmio.h"

static int store_index = 0;

std::list<CL_Blit_MGA*> CL_Blit_MGA::m_loaded_surfaces;

CL_Blit_MGA::CL_Blit_MGA(
	CL_FBDev_DisplayCard *card,
	CL_SurfaceProvider *provider,
	volatile unsigned char *mmioaddr)
{
	m_card = card;
	m_provider = provider;
	m_alphablitter = NULL;
	m_transparent = false;
	m_mmioaddr = mmioaddr;
}

CL_Blit_MGA::~CL_Blit_MGA()
{
	if (m_alphablitter != NULL) delete m_alphablitter;
}

bool CL_Blit_MGA::init_surface(CL_Blitters *blitters)
{
	m_width = m_provider->get_width();
	m_height = m_provider->get_height();
	
	/* sort big images out for cards with only 4Mb */
	if (m_card->get_total_memory() <= 4096*1024)
	{
		if ((m_width>400) || (m_height>300)) return false;
	}
	
	if (!reload(false, true)) return false;

	blitters->set_clip(this);
	blitters->set_noclip(this);

	if (m_card->accel_scale_blit())
	{
		blitters->set_scale_noclip(this);
		blitters->set_scale_clip(this);
	}

	m_loaded_surfaces.push_back(this);

	return true;
}

void CL_Blit_MGA::blt_noclip(
	CL_Target *target,
	int x,
	int y,
	int spr_no)
{
	int new_store = m_store;
	if (spr_no)
	{
		new_store += spr_no*m_size;
	}

	y += m_card->yoffset;

	if (m_transparent)
	{
		mga_waitfifo(m_mmioaddr, 7);

		mga_out32(m_mmioaddr, BLTMOD_BU32RGB | BOP_COPY | SHFTZERO | OP_BITBLT
					| SGNZERO | LINEAR | NOCLIP | ATYPE_RSTR | TRANS, DWGCTL);
		mga_out32(m_mmioaddr, m_transcol, FCOL);
		mga_out32(m_mmioaddr, 0xFFFFFFFF, BCOL);
	} else
	{
		mga_waitfifo(m_mmioaddr, 5);

		mga_out32(m_mmioaddr, BLTMOD_BU32RGB | BOP_COPY | SHFTZERO | OP_BITBLT
					| SGNZERO | LINEAR | NOCLIP | ATYPE_RPL, DWGCTL);
	}

	mga_out32(m_mmioaddr, new_store, AR3);
	mga_out32(m_mmioaddr, new_store+m_size, AR0);
	mga_out32(m_mmioaddr, ((x + m_width-1) << 16) | x, FXBNDRY);
	mga_out32(m_mmioaddr, (y << 16) | m_height, YDSTLEN | EXECUTE);

// 	if (m_alphablitter != NULL) m_alphablitter->blt_noclip(target, x, y, spr_no);*/
}

void CL_Blit_MGA::blt_clip(
	CL_Target *target,
	int x,
	int y,
	int spr_no,
	const CL_ClipRect &clip)
{
	int new_store = m_store;
	if (spr_no)
	{
		new_store += spr_no*m_size;
	}

	y += m_card->yoffset;

	if (m_transparent)
	{
		mga_waitfifo(m_mmioaddr, 7);

		mga_out32(m_mmioaddr, BLTMOD_BU32RGB | BOP_COPY | SHFTZERO | OP_BITBLT
					| SGNZERO | LINEAR | ATYPE_RSTR | TRANS, DWGCTL);
		mga_out32(m_mmioaddr, m_transcol, FCOL);
		mga_out32(m_mmioaddr, 0xFFFFFFFF, BCOL);
	} else
	{
		mga_waitfifo(m_mmioaddr, 5);

		mga_out32(m_mmioaddr, BLTMOD_BU32RGB | BOP_COPY | SHFTZERO | OP_BITBLT
					| SGNZERO | LINEAR | ATYPE_RPL, DWGCTL);
	}

	mga_out32(m_mmioaddr, new_store, AR3);
	mga_out32(m_mmioaddr, new_store+m_size, AR0);
	mga_out32(m_mmioaddr, ((x + m_width-1) << 16) | x, FXBNDRY);
	mga_out32(m_mmioaddr, (y << 16) | m_height, YDSTLEN | EXECUTE);
	
// 	if (m_alphablitter != NULL) m_alphablitter->blt_noclip(target, x, y, spr_no);*/
}

void CL_Blit_MGA::blt_scale_noclip(
                CL_Target *target,
                int x,
                int y,
                int dest_width,
                int dest_height,
                int spr_no)
{
	int new_store = m_store;
	if (spr_no)
	{
		new_store += spr_no*m_size;
	}

	y += m_card->yoffset;


	if (m_transparent)
	{
		mga_waitfifo( m_mmioaddr, 9 );
		mga_out32( m_mmioaddr, DECALDIS, TEXCTL2 );
		mga_out32( m_mmioaddr, 0xFFFF0000 | (m_transcol & 0xFFFF) | (1<<30), TEXTRANS );
	} else
	{
		mga_waitfifo( m_mmioaddr, 8 );
		mga_out32( m_mmioaddr, DECALDIS | CKSTRANSDIS, TEXCTL2 );
	}

	mga_out32( m_mmioaddr, BOP_COPY | SHFTZERO | SGNZERO | ARZERO | OP_TEXTURE_TRAP | ATYPE_I, DWGCTL );
	mga_out32( m_mmioaddr, (3<<27) | ((m_width)<<9) | LINEXT | TW16, TEXCTL );
	mga_out32( m_mmioaddr, (1<<20) | MAG_BILINEAR | MIN_BILINEAR, TEXFILTER );
	mga_out32( m_mmioaddr, ((m_width-1)<<18) | 2<<9 | 6, TEXWIDTH );
	mga_out32( m_mmioaddr, ((m_height-1)<<18) | 2<<9 | 6, TEXHEIGHT );
	mga_out32( m_mmioaddr, F1220(1/(float)(dest_width)), TMR0 );
	mga_out32( m_mmioaddr, 0, TMR1 );

	mga_waitfifo( m_mmioaddr, 10 );
	mga_out32( m_mmioaddr, 0, TMR2 );
	mga_out32( m_mmioaddr, F1220((1/(float)(dest_height)) / (m_width/(float)(m_height))), TMR3 );
	mga_out32( m_mmioaddr, 0, TMR4 );
	mga_out32( m_mmioaddr, 0, TMR5 );
	mga_out32( m_mmioaddr, 0, TMR6 );
	mga_out32( m_mmioaddr, 0, TMR7 );
	mga_out32( m_mmioaddr, F1616(64/(float)(m_width)), TMR8 );
	mga_out32( m_mmioaddr, new_store*2, TEXORG );
	mga_out32( m_mmioaddr, ((x+dest_width-1) << 16) | x, FXBNDRY );
	mga_out32( m_mmioaddr, (y << 16) | dest_height, YDSTLEN | EXECUTE );
}

void CL_Blit_MGA::blt_scale_clip(
                CL_Target *target,
                int x,
                int y,
                int dest_width,
                int dest_height,
                int spr_no,
		const CL_ClipRect &clip)
{
	int new_store = m_store;
	if (spr_no)
	{
		new_store += spr_no*m_size;
	}

	y += m_card->yoffset;


	if (m_transparent)
	{
		mga_waitfifo( m_mmioaddr, 9 );
		mga_out32( m_mmioaddr, DECALDIS, TEXCTL2 );
		mga_out32( m_mmioaddr, 0xFFFF0000 | (m_transcol & 0xFFFF) | (1<<30), TEXTRANS );
	} else
	{
		mga_waitfifo( m_mmioaddr, 8 );
		mga_out32( m_mmioaddr, DECALDIS | CKSTRANSDIS, TEXCTL2 );
	}

	mga_out32( m_mmioaddr, BOP_COPY | SHFTZERO | SGNZERO | ARZERO | OP_TEXTURE_TRAP | ATYPE_I, DWGCTL );
	mga_out32( m_mmioaddr, (3<<27) | ((m_width)<<9) | LINEXT | TW16, TEXCTL );
	mga_out32( m_mmioaddr, (1<<20) | MAG_BILINEAR | MIN_BILINEAR, TEXFILTER );
	mga_out32( m_mmioaddr, ((m_width-1)<<18) | 2<<9 | 6, TEXWIDTH );
	mga_out32( m_mmioaddr, ((m_height-1)<<18) | 2<<9 | 6, TEXHEIGHT );
	mga_out32( m_mmioaddr, F1220(1/(float)(dest_width)), TMR0 );
	mga_out32( m_mmioaddr, 0, TMR1 );

	mga_waitfifo( m_mmioaddr, 10 );
	mga_out32( m_mmioaddr, 0, TMR2 );
	mga_out32( m_mmioaddr, F1220((1/(float)(dest_height)) / (m_width/(float)(m_height))), TMR3 );
	mga_out32( m_mmioaddr, 0, TMR4 );
	mga_out32( m_mmioaddr, 0, TMR5 );
	mga_out32( m_mmioaddr, 0, TMR6 );
	mga_out32( m_mmioaddr, 0, TMR7 );
	mga_out32( m_mmioaddr, F1616(64/(float)(m_width)), TMR8 );
	mga_out32( m_mmioaddr, new_store*2, TEXORG );
	mga_out32( m_mmioaddr, ((x+dest_width-1) << 16) | x, FXBNDRY );
	mga_out32( m_mmioaddr, (y << 16) | dest_height, YDSTLEN | EXECUTE );
}

bool CL_Blit_MGA::reload(bool lock_provider, bool create_surface)
{
	if (lock_provider) m_provider->lock();

	CL_PixelData input(
		m_card->get_target()->get_red_mask(),
		m_card->get_target()->get_green_mask(),
		m_card->get_target()->get_blue_mask(),
		m_card->get_target()->get_alpha_mask(),
		m_provider,
		(m_card->get_target()->get_depth()+7)/8);

	CL_PixelData alpha( 0, 0, 0, 255, m_provider, 1 );
	
	// always use 8 byte alignment
	while ((long)(m_card->get_offscreen_mem()+store_index*input.get_bytes_per_pixel()) %8)
	{
		store_index++;
	}

	m_store = store_index + m_card->get_virt_width()*m_card->get_height()*2;
	m_size = m_width*m_height;

	if ((int)(m_card->get_target()->get_pitch()*m_card->get_height()*2+(store_index+m_size)*input.get_bytes_per_pixel()) > m_card->get_total_memory())
	{
		// sorry, out of video ram
		return false;
	}
	
	CL_ColorMap color_map(m_card->get_target());
	unsigned char* dest = m_card->get_offscreen_mem() + store_index * input.get_bytes_per_pixel();
	unsigned short trans_color = color_map.calc_color(0.05f, 0.05f, 0.05f, 0);

	m_transcol = (trans_color<<16) | trans_color; //FIXME: Support other bpp

	for( unsigned int i=0; i<m_height*m_provider->get_num_frames(); i++)
	{
		memcpy( dest, input.get_line_pixel(i), input.get_bytes_per_line() );
		
		unsigned short* dd = (unsigned short*)dest;
		unsigned char* aa = alpha.get_line_pixel(i);
		for ( int a=0; a<alpha.get_bytes_per_line(); a++ )
		{
			if (*dd == trans_color)
			{
				(*dd)++;
			}
			if (*aa != 255)
			{
				*dd = trans_color;
				m_transparent = true;
			}
			aa++;
			dd++;
		}

		dest += input.get_bytes_per_line();
		store_index += m_width;
	}

	if (lock_provider) m_provider->unlock();

//	std::cout << "Offscreen memory in use: " << store_index * (m_card->get_target()->get_depth()+7)/8 << std::endl;

	return true;
}

void CL_Blit_MGA::reload_all_surfaces()
{
	store_index = 0;
	for (
		std::list<CL_Blit_MGA*>::iterator counter = m_loaded_surfaces.begin();
		counter != m_loaded_surfaces.end();
		counter++)
	{
		(*counter)->reload(true, false);
	}
}
