/*
	$Id: target_be.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_target_be
#define header_target_be

#include "API/Core/Display/target.h"

#include "clanwindowscreen.h"

class CL_Target_Be : public CL_Target
{
public:
	CL_Target_Be();
		
	virtual ~CL_Target_Be();

	virtual bool is_video() const;

	virtual void lock();
	virtual void unlock();

	virtual void *get_data() const;

	virtual unsigned int get_num_frames() const;

	virtual unsigned int get_width() const;
	virtual unsigned int get_height() const;
	virtual unsigned int get_pitch() const;
	
	virtual unsigned int get_depth() const;
	virtual bool is_indexed() const;

	virtual unsigned int get_red_mask() const;
	virtual unsigned int get_green_mask() const;
	virtual unsigned int get_blue_mask() const;
	virtual unsigned int get_alpha_mask() const;

	virtual CL_Palette *get_palette() const;
        
protected:
	ClanWindowScreen *clws;
};

#endif
