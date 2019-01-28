/*
	$Id: bezier.h,v 1.8 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_bezier
#define header_bezier

#include "cl_vector.h"

//: Class for Bezier curves
//- This class generates points on a Bezier curve from given control points
//-
//- <p>USE AT YOUR OWN RISK TILL THIS CLASS IS EXTENSIVELY TESTED!!! I WILL
//- REMOVE THIS WARNING ONCE IT BASICALLY WORKS.</p>
//- 
//- <p>I hate documentation work and I won't document this code before any
//- one besides me actually uses it. So if you want to use it simply
//- write a mail on the ClanLib User list and I will do what I can :-) </p>
//- 
//- 1999/12/30	vogel       renamed CL_Bezier to CL_BezierCurve
//-                         added CL_BezierSurface
//- 
//- 1999/07/07	vogel       initial version
//- 
//- 
//- BezierCurve TODO: 
//- 
//- - length and stepping per curve
//- 
//- <p>stepping per curve means that the generated points are equidistant to 
//- each other and this requires get_length() working. So at the moment all
//- that is working is having 'n' equidistant points per segment.</p>
//- 	
//- same applies to BezierSurface.
class CL_BezierCurve
{
protected:
	CL_Vector *curve;	// the generated curve
	CL_Vector *cp;		// control points

	int steps, cs;		// number of steps, curve segments 
	bool stepping;		// steps per segment or per curve
		
	virtual void make_curve();

public:
//! Construction:
	//: Constructor
	//- cp - an array of control points with size (4 * cs)
	//- cs - number of curve segments
	//- steps - number of points to generate
	//- stepping - whether steps is the number of points per segment or per curve. 
	CL_BezierCurve(const CL_Vector *cp, int cs, int steps, bool stepping = true);

	//: Destructor
	virtual	~CL_BezierCurve();
	
//! Attributes:
	//: Returns the number of steps
	//- Returns - number of steps
	virtual int get_steps() const { return steps; }
	
	//: Returns the number of curve segments
	//- Returns - number of curve segments
	virtual int get_cs() const { return cs; }

	//: Returns whether steps is the number of points per segment or per curve.
	//- Returns - stepping
	virtual bool get_stepping() const { return stepping; }

	//: Returns the lenght of a segment (or the curve)
	//- segment - number of segment (-1 indicates whole curve)
	//- Returns - length of segment (or curve)
	virtual float get_length(int segment = -1) const;
	
	//: Returns an array of cs * steps CL_Vectors -> the curve
	//- Returns - the curve (cs * steps CL_Vectors)
	virtual CL_Vector* get_curve() const { return curve; }
	
	//: Returns the 4 * cs control points
	//- Returns - the 4 * cs control points
	virtual CL_Vector* get_control_points() const { return cp; }

//! Operations:
	//: Sets the number of steps
	//- steps - number of steps
	virtual void set_steps(int steps);
	
	//: Sets the stepping
	//- stepping - whether steps is the number of points per segment or per curve.
	virtual void set_stepping(bool stepping);

//! Operators:
	//: Returns the n-th vector of the curve (_doesn't_ return a reference)
	//- n - the vector to return (counting starts at 0)
	//- Returns - the n-th vector of the curve (_doesn't_ return a reference)
	CL_Vector operator [] (int n) { return curve[n]; }
};


//: Class for Bezier surfaces
//- This class generates points on a Bezier surface from given control points
//-
//- <p>USE AT YOUR OWN RISK TILL THIS CLASS IS EXTENSIVELY TESTED!!! I WILL
//- REMOVE THIS WARNING ONCE IT BASICALLY WORKS.</p>
//- 
//- <p>I hate documentation work and I won't document this code before any
//- one besides me actually uses it. So if you want to use it simply
//- write a mail on the ClanLib User list and I will do what I can :-)</p>
//- 
//- 1999/12/30	vogel       renamed CL_Bezier to CL_BezierCurve
//-                         added CL_BezierSurface
//- 
//- 1999/07/07	vogel       initial version
//- 
//- 
//- BezierCurve TODO: 
//- 
//- - length and stepping per curve
//- 
//- <p>stepping per curve means that the generated points are equidistant to 
//- each other and this requires get_length() working. So at the moment all
//- that is working is having 'n' equidistant points per segment.</p>
//- 	
//- same applies to BezierSurface.
class CL_BezierSurface
{
protected:
	CL_Vector *surface;	// the generated surface
	CL_Vector *cp;		// control points

	int xsteps, ysteps;	// number of steps in x and y direction, 
	int xs, ys;		// number of surface segments om x and y direction 
	bool stepping;		// steps per segment or per surface

	virtual CL_Vector evaluate(float, float, int);
	virtual void make_surface();

public:
//! Construction:
	//: Constructor
	//- cp - an array of control points with size (16 * xs * ys)
	//- xs - number of surface segments on x direction
	//- ys - number of surface segments on y direction
	//- xsteps - number of points to generate on x direction
	//- ysteps - number of points to generate on x direction
	//- stepping - whether steps is the number of points per segment or per surface. 
	CL_BezierSurface(const CL_Vector *cp, int xs, int ys, int xsteps, int ysteps, bool stepping = true);

	//: Destructor
	virtual	~CL_BezierSurface();

//! Attributes:
	//: Returns the number of steps
	//- Returns - number of steps
	virtual int get_xsteps() const { return xsteps; }
	
	virtual int get_ysteps() const { return ysteps; }
	//: Returns the number of steps
	//- Returns - number of steps
	
	//: Returns the number of surface segments
	//- Returns - number of surface segments
	virtual int get_xs() const { return xs; }

	//: Returns the number of surface segments
	//- Returns - number of surface segments
	virtual int get_ys() const { return ys; }

	//: Returns whether steps is the number of points per segment or per curve.
	//- Returns - stepping
	virtual bool get_stepping() const { return stepping; }

	//: Nak Nak - don't expect it to be implemented too soon...
	//: Returns the length of a segment (or the surface)
	//- segment - number of segment (-1 indicates whole surface)
	//- Returns - length of segment (or surface)
	// virtual float get_length(int segment = -1) const;
	
	//: Returns an array of xs * xsteps * ys * ysteps CL_Vectors -> the surface
	//- Returns - the curve (xs * xsteps * ys * ystepes CL_Vectors)
	virtual CL_Vector* get_surface() const { return surface; }
	
	//: Returns the 16 * xs * ys control points
	//- Returns - the 16 * xs * ys control points
	virtual CL_Vector* get_control_points() const { return cp; }

//! Operations:
	//: Sets the number of steps
	//- steps - number of steps
	virtual void set_xsteps(int xsteps);
	
	//: Sets the number of steps
	//- steps - number of steps
	virtual void set_ysteps(int ysteps);
	
	//: Sets the stepping
	//- stepping - whether steps is the number of points per segment or per curve.
	virtual void set_stepping(bool stepping);

//! Operators:
	//: Returns the n-th vector of the curve (_doesn't_ return a reference)
	//- n - the vector to return (counting starts at 0)
	//- Returns - the n-th vector of the curve (_doesn't_ return a reference)
	CL_Vector operator [] (int n) { return surface[n]; }
};

#endif
