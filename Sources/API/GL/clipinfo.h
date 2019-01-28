/*
	$Id: clipinfo.h,v 1.8 2001/09/22 15:52:13 plasmoid Exp $

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

#ifndef header_clipinfo
#define header_clipinfo

#include "camera.h"
#include "viewpoint.h"

//: Clipping information class.
//- <p>This class is used by all drawable objects to do intercommunication
//- between what's viewable and what isn't.</p>
//- <p>At the moment it only contains the camera (eye type) and camera's
//- viewpoint, but if more advanced clipping techniques should be needed,
//- then this is where the toplevel support should be added.</p>
class CL_ClipInfo
{
public:
//! Construction:
	//: The camera/eye type.
	CL_Camera camera;

	//: The camera/eye's viewpoint center.
	CL_Viewpoint viewpoint;

//! Variables:
	//: Distance from center to real pos
	float distance;
};

#endif
