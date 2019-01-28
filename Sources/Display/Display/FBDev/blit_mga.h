/*
        $Id: blit_mga.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

        ------------------------------------------------------------------------
        ClanLib, the platform independent game SDK.

        This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
        version 2. See COPYING for details.

        For a total list of contributers see CREDITS.

        ------------------------------------------------------------------------

*/

#ifndef header_blit_mga
#define header_blit_mga

#ifdef USE_FBDEV

#include "../Generic/blitters.h"
#include "display_fbdev.h"

class CL_Blit_AlphaMask_RLE;

class CL_Blit_MGA :
	public CL_Blit_NoClip,
	public CL_Blit_Clip,
	public CL_Blit_Scale_NoClip,
	public CL_Blit_Scale_Clip
{
public:
	CL_Blit_MGA(CL_FBDev_DisplayCard *card, CL_SurfaceProvider *provider, volatile unsigned char *mmioaddr);
	virtual ~CL_Blit_MGA();

	bool init_surface(CL_Blitters *blitters);

	virtual void blt_noclip(
		CL_Target *target,
		int x,
		int y,
		int spr_no);

	virtual void blt_clip(
		CL_Target *target,
		int x,
		int y,
		int spr_no,
		const CL_ClipRect &clip);
	
        virtual void blt_scale_noclip(
                CL_Target *target,
                int x,
                int y,
                int dest_width,
                int dest_height,
                int spr_no);

        virtual void blt_scale_clip(
                CL_Target *target,
                int x,
                int y,
                int dest_width,
                int dest_height,
                int spr_no,
		const CL_ClipRect &clip);

	static void reload_all_surfaces();

private:
	CL_FBDev_DisplayCard *m_card;
	CL_SurfaceProvider *m_provider;
	bool m_transparent;
	unsigned int m_transcol;
	unsigned int m_width, m_height;
	unsigned int m_store, m_size;
	
	volatile unsigned char *m_mmioaddr;

	CL_Blit_AlphaMask_RLE *m_alphablitter;

	bool reload(bool lock_provider, bool create_surface);

	static std::list<CL_Blit_MGA*> m_loaded_surfaces;
};

#endif

#endif
