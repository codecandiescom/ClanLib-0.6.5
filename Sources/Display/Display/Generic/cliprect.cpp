/*
	$Id: cliprect.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <API/Display/Display/cliprect.h>

CL_ClipRect::CL_ClipRect()
{
}

CL_ClipRect::CL_ClipRect(const CL_ClipRect &rect)
{
	m_x1 = rect.m_x1;
	m_y1 = rect.m_y1;
	m_x2 = rect.m_x2;
	m_y2 = rect.m_y2;
}

CL_ClipRect::CL_ClipRect(int x1, int y1, int x2, int y2)
{
	if (x1 <= x2)
	{
		m_x1 = x1;
		m_x2 = x2;
	}
	else
	{
		m_x1 = x2;
		m_x2 = x1;
	}

	if (y1 <= y2)
	{
		m_y1 = y1;
		m_y2 = y2;
	}
	else
	{
		m_y1 = y2;
		m_y2 = y1;
	}
}

bool CL_ClipRect::test_clipped(const CL_ClipRect &rect) const
{
	if (m_x1 > rect.m_x1) return true;
	if (m_x2 < rect.m_x2) return true;
	if (m_y1 > rect.m_y1) return true;
	if (m_y2 < rect.m_y2) return true;

	return false;
}

bool CL_ClipRect::test_unclipped(const CL_ClipRect &rect) const
{
	return m_x1 <= rect.m_x1 &&
	       m_y1 <= rect.m_y1 &&
	       m_x2 >= rect.m_x2 &&
	       m_y2 >= rect.m_y2;
}

bool CL_ClipRect::test_all_clipped(const CL_ClipRect &rect) const
{
	CL_ClipRect clipped_rect = clip(rect);

	return (clipped_rect.m_x1 > clipped_rect.m_x2) ||
	       (clipped_rect.m_y1 > clipped_rect.m_y2);
}

CL_ClipRect CL_ClipRect::clip(const CL_ClipRect &rect) const
{
	CL_ClipRect res;

	if (m_x1 > rect.m_x1) res.m_x1 = m_x1; else res.m_x1 = rect.m_x1;
	if (m_x2 < rect.m_x2) res.m_x2 = m_x2; else res.m_x2 = rect.m_x2;
	if (m_y1 > rect.m_y1) res.m_y1 = m_y1; else res.m_y1 = rect.m_y1;
	if (m_y2 < rect.m_y2) res.m_y2 = m_y2; else res.m_y2 = rect.m_y2;
	
	return res;
}

bool CL_ClipRect::operator ==(const CL_ClipRect &rect) const
{
	return m_x1 == rect.m_x1 &&
	       m_y1 == rect.m_y1 &&
	       m_x2 == rect.m_x2 &&
	       m_y2 == rect.m_y2;
}

/*std::ostream& operator << (std::ostream &os, CL_ClipRect &rect)
{
	os << "(" << rect.m_x1 << ", " << rect.m_y1 << ")-(" << rect.m_x2 << ", " << rect.m_y2 << ")";

	return os;
}
*/
