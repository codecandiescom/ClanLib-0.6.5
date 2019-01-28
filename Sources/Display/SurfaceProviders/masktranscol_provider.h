/*
	$Id: masktranscol_provider.h,v 1.2 2001/09/08 19:12:49 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_masktranscol_provider
#define header_masktranscol_provider

#include "API/Display/SurfaceProviders/generic_surfaceprovider.h"

class CL_MaskTranscolProvider : public CL_SurfaceProvider_Generic
{
	CL_SurfaceProvider *parent;
	bool delete_provider;
	unsigned int *mask_indices;
	int num_mask_indices;
public:
	CL_MaskTranscolProvider(
			CL_SurfaceProvider *parent, 
			bool delete_provider, 
			const unsigned int *mask_indices,
			int num_mask_indices);
	virtual ~CL_MaskTranscolProvider();

	virtual unsigned int get_pitch() const;
	virtual int get_translate_x() const;
	virtual int get_translate_y() const;
	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_num_frames() const;
	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;
	virtual unsigned int get_depth() const;
	virtual bool is_indexed() const;

	virtual CL_Palette *get_palette() const;
	virtual unsigned int get_src_colorkey() const;
	virtual bool uses_src_colorkey() const;
	
	virtual void *get_data() const;

	virtual void perform_lock();
	virtual void perform_unlock();
};

#endif
