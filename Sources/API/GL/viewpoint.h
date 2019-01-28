/*
	$Id: viewpoint.h,v 1.12 2001/12/15 18:15:45 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	Copyright 1999-2000 by the Dodge This Development Team.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGL="OpenGL"
//! header=gl.h

#ifndef header_viewpoint
#define header_viewpoint

#include "../Core/Math/cl_vector.h"

//: Viewpoint Class
class CL_Viewpoint
{
public:
//! Construction:
	//: Default Viewpoint Constructor
	CL_Viewpoint()
	{
		pos = CL_Vector(0,0,0);
		dir = CL_Vector(0,0,0);
		up = CL_Vector(0,0,0);
	}

	//: Viewpoint Constructor
	CL_Viewpoint(
		const CL_Vector &pos,
		const CL_Vector &dir,
		const CL_Vector &up)
	{
		this->pos = pos;
		this->dir = dir;
		this->up = up;
	}

//! Variables:
	//: Postion
	CL_Vector pos;

	//: Direction
	CL_Vector dir;

	//: Up
	CL_Vector up;
};

#endif
