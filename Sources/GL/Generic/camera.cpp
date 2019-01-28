/*
	Dodge This, ClanLib example Game,
	Copyright 1999-2000 by the Dodge This Development Team.

	This game is licensed under the GNU PUBLIC LICENSE (GPL), version 2.
*/

#include "API/gl.h"
#include <math.h>

//#include "API/gl.h"

#include "API/GL/camera.h"
#include "API/GL/viewpoint.h"
#include "API/Display/Display/display.h"

CL_Camera::CL_Camera()
{
	nearZ = 0;
	farZ = 0;
	fov = 0;
}

CL_Camera::CL_Camera(
	float nearZ,
	float farZ,
	float fov)
{
	this->nearZ = nearZ;
	this->farZ = farZ;
	this->fov = fov;
}

void CL_Camera::set_viewport(const CL_Viewpoint &vp, float dist) const
{
	float ratio = CL_Display::get_width() / (float) CL_Display::get_height();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();			
	gluPerspective(
		fov,                   // field of view
		ratio,                 // aspect ratio
		nearZ,                 // near clipping plane
		farZ);                 // far clipping plane

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();			

	gluLookAt(
		// position
		vp.pos.x - dist * vp.dir.x,
		vp.pos.y - dist * vp.dir.y,
		vp.pos.z - dist * vp.dir.z,
		// center
		vp.pos.x,
		vp.pos.y,
		vp.pos.z,
		// up
		vp.up.x,
		vp.up.y,
		vp.up.z);
}
