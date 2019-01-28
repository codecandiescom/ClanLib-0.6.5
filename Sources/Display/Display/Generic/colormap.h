/*
	$Id: colormap.h,v 1.2 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_colormap
#define header_colormap

#include "API/Display/Display/target.h"

class CL_ColorMap
{
public:
	CL_ColorMap(
		unsigned int red_mask,
		unsigned int green_mask,
		unsigned int blue_mask,
		unsigned int alpha_mask)
	{
		init(red_mask, green_mask, blue_mask, alpha_mask);
	}

	CL_ColorMap(CL_Target *target)
	{
		init(
			target->get_red_mask(),
			target->get_green_mask(),
			target->get_blue_mask(),
			target->get_alpha_mask());
	}

	unsigned int calc_color(float r, float g, float b, float a)
	{
		// paranoia:
		if (r>1) r=1; if (g>1) g=1;
		if (b>1) b=1; if (a>1) a=1;
		if (r<0) r=0; if (g<0) g=0;
		if (b<0) b=0; if (a<0) a=0;

		unsigned int red = (unsigned int) (r*((1<<m_red_length)-1)+0.5);
		unsigned int green = (unsigned int) (g*((1<<m_green_length)-1)+0.5);
		unsigned int blue = (unsigned int) (b*((1<<m_blue_length)-1)+0.5);
		unsigned int alpha = (unsigned int) (a*((1<<m_alpha_length)-1)+0.5);

		return
			(red<<m_red_start)+
			(green<<m_green_start)+
			(blue<<m_blue_start)+
			(alpha<<m_alpha_start);
	}

	unsigned int m_red_length, m_red_start;
	unsigned int m_green_length, m_green_start;
	unsigned int m_blue_length, m_blue_start;
	unsigned int m_alpha_length, m_alpha_start;
private:

	void init(
		unsigned int red_mask,
		unsigned int green_mask,
		unsigned int blue_mask,
		unsigned int alpha_mask)
	{
		m_red_start = start_bit(red_mask);
		m_green_start = start_bit(green_mask);
		m_blue_start = start_bit(blue_mask);
		m_alpha_start = start_bit(alpha_mask);

		m_red_length = length(red_mask, m_red_start);
		m_green_length = length(green_mask, m_green_start);
		m_blue_length = length(blue_mask, m_blue_start);
		m_alpha_length = length(alpha_mask, m_alpha_start);
	}

	int start_bit(unsigned int mask)
	{
		if (mask == 0) return 0;
	
		unsigned int bitval=1;
		unsigned int counter=0;

		while ((bitval&mask)==0)
		{
			counter++;
			bitval = bitval << 1;
		}

		return counter;
	}

	int length(unsigned int mask, unsigned int start_bit)
	{
		if (mask == 0) return 0;

		unsigned int bitval=1;
		unsigned int counter=0;

		mask = mask >> start_bit;

		while ((bitval&mask)!=0)
		{
			counter++;
			bitval = bitval << 1;
		}

		return counter;
	}

/*	friend std::ostream &operator <<(std::ostream &os, CL_ColorMap &map)
	{
		os << "ColorLength(" << map.m_red_length << "," << map.m_green_length << "," << map.m_blue_length << ") : "
			<< "ColorStart(" << map.m_red_start << "," << map.m_green_start << "," << map.m_blue_start << ")";
		return os;
	}
*/
};

#endif
