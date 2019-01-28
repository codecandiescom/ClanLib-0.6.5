/*
	$Id: bezier.cpp,v 1.2 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "API/Core/Math/bezier.h"

/*
    control points = p1,p2,p3,p4 whereof p1,p4 are endpoints and
    p1p2,p3p4 are the respective tangent vectors. If more than one 
    curve is concatenated make sure that p4 - p3 = p5 - p4 if you
    want C0 and C1 steadyness (sp?)
    
    for the math freaks: the path is given by
    w(t)  = (1-t)^3 * P1 + 3*t*(1-t)^2 * P2 + 3*t^2*(1-t)*P3 + t^3*P4; t in [0,1]
    
    and the length is 
    Lw = /int_{0}^{1} |w'(t)| dt
*/


CL_BezierCurve::CL_BezierCurve(const CL_Vector *cp, int cs, int steps, bool stepping)
{
	curve = new CL_Vector[cs*steps];
	this->cp = new CL_Vector[cs*4];
	this->steps = steps;
	this->stepping = stepping;
	this->cs = cs;
	
	for (int i=0;i<cs*4;i++)
		this->cp[i] = cp[i];

	make_curve();
}


CL_BezierCurve::~CL_BezierCurve()
{
	delete [] curve;
	delete [] cp;
}


// hey it is not the fastest code, but I'll change it to a faster algorithm
// somewhen in the future, though this is not a promise ;-) 
// [ actually it is a lie :-) ]

void CL_BezierCurve::make_curve()
{
	// TODO: interpret stepping
	for (int n=0; n<cs; n++)
	{
		curve[n*steps] = cp[n*4];

		float t, delta;
		delta = 1.0/steps;
	
		for (int i=1; i<steps; i++)
		{
			t = i*delta;
			curve[n*steps+i].x = 
				cp[n*4+0].x * (1.0 - t) * (1.0 - t) * (1.0 - t) +
	        		cp[n*4+1].x * 3.0 * t * (1.0 - t) * (1.0 - t) +
		        	cp[n*4+2].x * 3.0 * t *  t  * (1.0 - t) + 
				cp[n*4+3].x * t * t * t;

			curve[n*steps+i].y = 
				cp[n*4+0].y * (1.0 - t) * (1.0 - t) * (1.0 - t) +
	        		cp[n*4+1].y * 3.0 * t * (1.0 - t) * (1.0 - t) +
	            		cp[n*4+2].y * 3.0 * t *  t  * (1.0 - t) + 
				cp[n*4+3].y * t * t * t;

			curve[n*steps+i].z =
				cp[n*4+0].z * (1.0 - t) * (1.0 - t) * (1.0 - t) +
				cp[n*4+1].z * 3.0 * t * (1.0 - t) * (1.0 - t) +
		    		cp[n*4+2].z * 3.0 * t *  t  * (1.0 - t) + 
				cp[n*4+3].z * t * t * t;
		}
	}
}


float CL_BezierCurve::get_length(int segment) const
{
	if (segment == -1)
	{
		float length = 0;
		for (int i=0;i<cs;i++)
			length += get_length(cs);
		return length;
	}
	// TODO: calculate Lw for segment
	return 0;	
}


void CL_BezierCurve::set_steps(int steps)
{
	delete [] curve;
	curve = new CL_Vector[cs*steps];
	this->steps = steps;
	make_curve();
}


void CL_BezierCurve::set_stepping(bool stepping)
{
	this->stepping = stepping;
	make_curve();
}


// CL_BezierSurface

CL_BezierSurface::CL_BezierSurface(const CL_Vector *cp, int xs, int ys, int xsteps, int ysteps, bool stepping)
{
	surface = new CL_Vector[xs*xsteps*ys*ysteps];
	this->cp = new CL_Vector[xs*ys*16];
	this->xsteps = xsteps;
	this->ysteps = ysteps;
	this->stepping = stepping;
	this->xs = xs;
	this->ys = ys;
	
	for (int i=0;i<xs*ys*16;i++)
		this->cp[i] = cp[i];

	make_surface();
}


CL_BezierSurface::~CL_BezierSurface()
{
	delete [] surface;
	delete [] cp;
}


CL_Vector CL_BezierSurface::evaluate(float s, float t, int offset)
{
	int stride = 4 * xs;
	CL_Vector v;

	for (int i=0; i<3; i++)
	{
		v[i] = (1-s)*(1-s)*(1-s)* (
			cp[offset+0+0*stride][i] * (1-t)*(1-t)*(1-t) + 
			cp[offset+0+1*stride][i] * 3*(1-t)*(1-t)*t + 
			cp[offset+0+2*stride][i] * 3*(1-t)*t*t + 
			cp[offset+0+3*stride][i] *t*t*t) +
 
			3*(1-s)*(1-s)*s* (
			cp[offset+1+0*stride][i] * (1-t)*(1-t)*(1-t) + 
			cp[offset+1+1*stride][i] * 3*(1-t)*(1-t)*t + 
			cp[offset+1+2*stride][i] * 3*(1-t)*t*t + 
			cp[offset+1+3*stride][i] *t*t*t) +

			3*(1-s)*s*s* (
			cp[offset+2+0*stride][i] * (1-t)*(1-t)*(1-t) + 
			cp[offset+2+1*stride][i] * 3*(1-t)*(1-t)*t + 
			cp[offset+2+2*stride][i] * 3*(1-t)*t*t + 
			cp[offset+2+3*stride][i] *t*t*t) +

			s*s*s* (
			cp[offset+3+0*stride][i] * (1-t)*(1-t)*(1-t) + 
			cp[offset+3+1*stride][i] * 3*(1-t)*(1-t)*t + 
			cp[offset+3+2*stride][i] * 3*(1-t)*t*t + 
			cp[offset+3+3*stride][i] *t*t*t);
	}
	return v;
} 

void CL_BezierSurface::make_surface()
{
	// TODO: interpret stepping

	for (int y=0; y<ys; y++)
	{
		for (int x=0; x<xs; x++)
		{
			float s,t, deltax, deltay;

			deltax = 1.0/xsteps;
			deltay = 1.0/ysteps;

			for (int ny=0; ny<ysteps; ny++)
			{
				for (int nx=0; nx<xsteps; nx++)
				{
					s = nx * deltax;
					t = ny * deltay;
					
					surface[y*ysteps*xsteps*xs + x] = evaluate(s,t, y*16*xs + x);
				}
			}
		}
	}
}


void CL_BezierSurface::set_xsteps(int xsteps)
{
	delete [] surface;
	surface = new CL_Vector[xs*xsteps*ys*ysteps];
	this->xsteps = xsteps;
	make_surface();
}


void CL_BezierSurface::set_ysteps(int ysteps)
{
	delete [] surface;
	surface = new CL_Vector[xs*xsteps*ys*ysteps];
	this->ysteps = ysteps;
	make_surface();
}


void CL_BezierSurface::set_stepping(bool stepping)
{
	this->stepping = stepping;
	make_surface();
}
