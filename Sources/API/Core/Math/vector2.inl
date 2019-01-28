/*
	$Id: vector2.inl,v 1.2 2001/09/04 08:54:17 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	Credits for this class:
	  Magic Software - www.magic-software.com
*/

inline CL_Vector2::CL_Vector2 ()
{
    // For efficiency in construction of large arrays of vectors, the
    // default constructor does not initialize the vector.
}

inline float& CL_Vector2::operator[](int i) const
{
    return ((float*)this)[i];
}

inline CL_Vector2::operator float*()
{
    return (float*)this;
}

inline float CL_Vector2::squared_length() const
{
    return x*x + y*y;
}
