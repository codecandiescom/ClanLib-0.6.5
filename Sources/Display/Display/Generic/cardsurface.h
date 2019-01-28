/*
	$Id: cardsurface.h,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_cardsurface
#define header_cardsurface

class CL_SurfaceProvider;
class CL_Target;

class CL_CardSurface
{
public:
	CL_CardSurface(CL_SurfaceProvider *provider)
	{
		m_provider = provider;
	}

	virtual ~CL_CardSurface() {;}

	virtual CL_SurfaceProvider *get_provider()
	{
		return m_provider;
	}

	virtual bool reload()=0;

	virtual bool can_convert_video()=0;
	virtual bool can_convert_system()=0;

	virtual bool convert_video()=0;
	virtual bool convert_system()=0;

	virtual void put_screen(
		int x,
		int y,
		int spr_no,
		CL_Target *target)=0;
		
	virtual void put_screen(
		int x,
		int y,
		float scale_x,
		float scale_y,
		int spr_no,
		CL_Target *target)=0;

	virtual void put_screen(
		int x,
		int y,
		int size_x,
		int size_y,
		int spr_no,
		CL_Target *target)=0;
		
	virtual bool is_video()=0;

protected:
	CL_SurfaceProvider *m_provider;
};

#endif
