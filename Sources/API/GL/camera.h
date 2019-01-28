/*
	$Id: camera.h,v 1.8 2001/09/22 15:52:13 plasmoid Exp $

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

#ifndef header_camera
#define header_camera

class CL_Viewpoint;
//: This is camera class.
//- Camera maintains the projection and view matrix
class CL_Camera
{
public:
//! Construction:
	//: Constructs an uninitialized camera.
	CL_Camera();

	//: Constructs a camera.
	//- pos - The position that the camera stands in.
	//- dir - The direction that the camera looks in.
	//- up - The up direction of the camera.
	//- nearZ - Near clipping plane.
	//- farZ - Far clipping plane.
	//- fov - Field of view.
	CL_Camera(
		float nearZ,
		float farZ,
		float fov);

//! Operations:
	//: Set view matrix to look from the camera's point of view.
	void set_viewport(const CL_Viewpoint &viewpoint, float distance) const;

//! Variables:
	//: Field of view
	float fov;

	//: Near clipping plane
	float nearZ;

	//: Far clipping plane
	float farZ;
};

#endif
