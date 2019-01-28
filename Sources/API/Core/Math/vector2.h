/*
	$Id: vector2.h,v 1.6 2001/10/23 07:32:35 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	Credits for this class:
	  Magic Software - www.magic-software.com
*/

//! clanCore="Math"
//! header=core.h

#ifndef header_vector2d
#define header_vector2d

//: A 2D Vector class
//- CL_Vector2 provides a 2D vector class.
//- This is different from {link CL_Vector CL_Vector.html}
class CL_Vector2
{
public:
//! Construction:
	//: Default constructor
	CL_Vector2();
	
	//: Constructor with info
	//- fX - The X coordinate.
	//- fY - The Y coordinate.
	CL_Vector2(float fX, float fY);

	//: FIXME!
	CL_Vector2(float afCoordinate[2]);

	//: Copy constructor
	//- rkVector = Vector to copy
	CL_Vector2(const CL_Vector2& rkVector);

//! Operators:
	// access vector V as V[0] = V.x, V[1] = V.y
	//
	// WARNING.  These member functions rely on
	// (1) CL_Vector2 not having virtual functions
	// (2) the data packed in a 2*sizeof(float) memory block
	
	//: Accessor
	//: Use vector V[0] for V.x
	//: Use vector V[1] for V.y
	//- Returns - The requested parameter
	float& operator[] (int i) const;

	//: Unknown
	operator float* ();

	//: assignment operator
	CL_Vector2& operator= (const CL_Vector2& rkVector);

// comparison (supports fuzzy arithmetic when FUZZ > 0)

	//: Equivalancy operator
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator== (const CL_Vector2& rkVector) const;
	
	//: Not-equals operator
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator!= (const CL_Vector2& rkVector) const;
	
	//: Less than operator
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator<  (const CL_Vector2& rkVector) const;
	
	//: Less than or equal operator
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator<= (const CL_Vector2& rkVector) const;
	
	//: Greater than operator
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator>  (const CL_Vector2& rkVector) const;
	
	//: Greater than or equal operator		
	//- Supports fuzzy arimatic when FUZZ > 0
	bool operator>= (const CL_Vector2& rkVector) const;

// arithmetic operations

	//: Addition operator
	CL_Vector2 operator+ (const CL_Vector2& rkVector) const;
	
	//: Subtraction operator
	CL_Vector2 operator- (const CL_Vector2& rkVector) const;
	
	//: Multiplication operator
	CL_Vector2 operator* (float fScalar) const;
	
	//: Division operator
	CL_Vector2 operator/ (float fScalar) const;
	
	//: Negation operator
	//- Return - The negative of the Vector
	CL_Vector2 operator- () const;

//	MAGICFM friend CL_Vector2 operator* (float fScalar, const CL_Vector2& rkVector);

// arithmetic updates

	//: Addition assignment operator
	CL_Vector2& operator+= (const CL_Vector2& rkVector);
	
	//: Subtraction assignment operator
	CL_Vector2& operator-= (const CL_Vector2& rkVector);
	
	//: Multiplication assignment operator
	CL_Vector2& operator*= (float fScalar);
	
	//: Division assignment operator
	CL_Vector2& operator/= (float fScalar);

//! Attributes:

	//: Enable Fuzzy arithmatic
	void set_fuzz() { FUZZ = 1.0; }

	//: Disable Fuzzy arithmatic
	void disable_fuzz() { FUZZ = 0.0; }

	//: Using fuzzy arimatic
	//- Returns - True if using fuzzy arithmatic
	bool get_fuzz() { return (FUZZ > 0.0)? true : false ;}

// vector operations
	//: The length of the vector
	//- Return - sqrt(x^2+y^2)
	float length() const;

	//: The squared length of the vector
	//- Returns - x^2+y^2
	float squared_length() const;

	//: FIXME!!!
	float dot(const CL_Vector2& rkVector) const;

	//: FIXME!!!
	float unitize(float fTolerance = 1e-06f);

	//: Cross
	//- FIXME
	//- Returns - Return y,-x
	CL_Vector2 cross() const;

	//: Unit Cross
	//- Returns - (y,-x)/sqrt(x^2+y^2)
	CL_Vector2 unit_cross() const;

	// Gram-Schmidt orthonormalization.
//	static void Orthonormalize (CL_Vector2 akVector[/*2*/]);

//! Variables:
	// coordinates
	float x, y;

	// special points
	static const CL_Vector2 ZERO;
	static const CL_Vector2 UNIT_X;
	static const CL_Vector2 UNIT_Y;

	// fuzzy arithmetic (set FUZZ > 0 to enable)
	static float FUZZ;
};

#include "vector2.inl"

#endif
