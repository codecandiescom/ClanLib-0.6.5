/*
	$Id: target_be.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Core/Display/palette.h>
#include <Core/System/Be/app_beos.h>
#include "target_be.h"

CL_Target_Be::CL_Target_Be()
{
	clws = ((LibApplication*)be_app)->clanscreen;
}

CL_Target_Be::~CL_Target_Be()
{
}

bool CL_Target_Be::is_video() const
{
	return (clws->is_connected());
}

void CL_Target_Be::lock()
{
//	return (clws->get_data()!=NULL);
}
	
void CL_Target_Be::unlock()
{
}

void *CL_Target_Be::get_data() const
{
	return clws->get_data();
}

unsigned int CL_Target_Be::get_num_frames() const
{
	return 1;
}

unsigned int CL_Target_Be::get_width() const
{
	return clws->get_width();
}

unsigned int CL_Target_Be::get_height() const
{
	return clws->get_width();
}

unsigned int CL_Target_Be::get_pitch() const
{
	return clws->get_pitch();
}
	
unsigned int CL_Target_Be::get_depth() const
{
	return clws->get_depth();
}

bool CL_Target_Be::is_indexed() const
{
	return false;
}

unsigned int CL_Target_Be::get_red_mask() const
{
	return ((int)((pow(2,5))-1) << (6 + 5));
}

unsigned int CL_Target_Be::get_green_mask() const
{
	return ((int)(pow(2,6))-1) << 5;
}

unsigned int CL_Target_Be::get_blue_mask() const
{
	return (int)(pow(2,5))-1;
}

unsigned int CL_Target_Be::get_alpha_mask() const
{
	return 0;
}

CL_Palette *CL_Target_Be::get_palette() const
{
	return NULL;
}

