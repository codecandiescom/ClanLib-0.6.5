/*
	$Id: vector2.cpp,v 1.5 2001/09/15 14:54:51 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	Credits for this class:
	  Magic Software - www.magic-software.com
*/

#include "Core/precomp.h"
#include "API/Core/Math/vector2.h"
#include <math.h>
#include <float.h>

const CL_Vector2 CL_Vector2::ZERO(0.0f,0.0f);
const CL_Vector2 CL_Vector2::UNIT_X(1.0f,0.0f);
const CL_Vector2 CL_Vector2::UNIT_Y(0.0f,1.0f);
float CL_Vector2::FUZZ = 0.0f;

CL_Vector2::CL_Vector2(float fX, float fY)
{
    x = fX;
    y = fY;
}

CL_Vector2::CL_Vector2(float afCoordinate[2])
{
    x = afCoordinate[0];
    y = afCoordinate[1];
}

CL_Vector2::CL_Vector2(const CL_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
}

CL_Vector2& CL_Vector2::operator=(const CL_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
    return *this;
}

bool CL_Vector2::operator==(const CL_Vector2& rkVector) const
{
    if ( FUZZ == 0.0f )
        return x == rkVector.x && y == rkVector.y;
    else
        return fabs(x-rkVector.x) <= FUZZ && fabs(y-rkVector.y) <= FUZZ;
}

bool CL_Vector2::operator!=(const CL_Vector2& rkVector) const
{
    if ( FUZZ == 0.0f )
        return x != rkVector.x || y != rkVector.y;
    else
        return fabs(x-rkVector.x) > FUZZ || fabs(y-rkVector.y) > FUZZ;
}

bool CL_Vector2::operator<(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 < uiTest1 )
        return true;
    if ( uiTest0 > uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 < uiTest1;
}

bool CL_Vector2::operator<=(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 < uiTest1 )
        return true;
    if ( uiTest0 > uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 <= uiTest1;
}

bool CL_Vector2::operator>(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 > uiTest1 )
        return true;
    if ( uiTest0 < uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 > uiTest1;
}

bool CL_Vector2::operator>=(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 > uiTest1 )
        return true;
    if ( uiTest0 < uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 >= uiTest1;
}

CL_Vector2 CL_Vector2::operator+(const CL_Vector2& rkVector) const
{
    return CL_Vector2(x+rkVector.x,y+rkVector.y);
}

CL_Vector2 CL_Vector2::operator-(const CL_Vector2& rkVector) const
{
    return CL_Vector2(x-rkVector.x,y-rkVector.y);
}

CL_Vector2 CL_Vector2::operator*(float fScalar) const
{
    return CL_Vector2(fScalar*x,fScalar*y);
}

CL_Vector2 CL_Vector2::operator-() const
{
    return CL_Vector2(-x,-y);
}

/*
CL_Vector2 Mgc::operator* (float fScalar, const CL_Vector2& rkVector)
{
    return CL_Vector2(fScalar*rkVector.x,fScalar*rkVector.y);
}
*/
CL_Vector2 CL_Vector2::operator/(float fScalar) const
{
    CL_Vector2 kQuot;

    if ( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f/fScalar;
        kQuot.x = fInvScalar*x;
        kQuot.y = fInvScalar*y;
        return kQuot;
    }
    else
    {
        return CL_Vector2(FLT_MAX,FLT_MAX);
    }
}

CL_Vector2& CL_Vector2::operator+=(const CL_Vector2& rkVector)
{
    x += rkVector.x;
    y += rkVector.y;
    return *this;
}

CL_Vector2& CL_Vector2::operator-=(const CL_Vector2& rkVector)
{
    x -= rkVector.x;
    y -= rkVector.y;
    return *this;
}

CL_Vector2& CL_Vector2::operator*=(float fScalar)
{
    x *= fScalar;
    y *= fScalar;
    return *this;
}

CL_Vector2& CL_Vector2::operator/=(float fScalar)
{
    if ( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f/fScalar;
        x *= fInvScalar;
        y *= fInvScalar;
    }
    else
    {
        x = FLT_MAX;
        y = FLT_MAX;
    }

    return *this;
}

float CL_Vector2::dot(const CL_Vector2& rkVector) const
{
    return x*rkVector.x + y*rkVector.y;
}

float CL_Vector2::length() const
{
    return sqrtf(x*x +y*y);
}

CL_Vector2 CL_Vector2::cross() const
{
    return CL_Vector2(y,-x);
}

CL_Vector2 CL_Vector2::unit_cross() const
{
    CL_Vector2 kCross(y,-x);
    kCross.unitize();
    return kCross;
}

float CL_Vector2::unitize(float fTolerance)
{
    float fLength = length();

    if ( fLength > fTolerance )
    {
        float fInvLength = 1.0f/fLength;
        x *= fInvLength;
        y *= fInvLength;
    }
    else
    {
        fLength = 0.0f;
    }

    return fLength;
}

/*
void CL_Vector2::Orthonormalize (CL_Vector2 akVector[2])	// CL_Vector2 akVector[2]
{
    // If the input vectors are v0 and v1, then the Gram-Schmidt
    // orthonormalization produces vectors u0 and u1 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    akVector[0].Unitize();

    // compute u1
    float fDot0 = akVector[0].Dot(akVector[1]); 
    akVector[1] -= fDot0*akVector[0];
    akVector[1].Unitize();
}
*/
