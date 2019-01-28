/*
	$Id: inputaxis_group.h,v 1.7 2002/06/13 14:43:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Input"
//! header=display.h

#ifndef header_inputaxis_group
#define header_inputaxis_group

#include <list>
#include "../Input/inputaxis.h"

//: CL_InputAxis container class. Groups a set of axes.
class CL_InputAxis_Group : public CL_InputAxis
{
	std::list<CL_InputAxis*> axes;
public:
//! Operations:
	//: Adds a input axis to the group.
	//- add_axis - Axis to be added.
	void add(CL_InputAxis *add_axis)
	{
		axes.push_back(add_axis);
	}

	//: Removes a input axis from the group.
	//- remove_axis - Axis to be removed.
	void remove(CL_InputAxis *remove_axis)
	{
		axes.remove(remove_axis);
	}

//! Attributes:
	//: <p>Returns the current axis position. This is calculated by sumarizing
	//: all the input axes positions and return that value.</p>
	//- Returns - Current axis position.
	virtual float get_pos()
	{
		float pos = 0;

		std::list<CL_InputAxis *>::iterator it_end(axes.end());
		for (
			std::list<CL_InputAxis *>::iterator it = axes.begin();
			it != it_end;
			++it)
		{
			pos += (*it)->get_pos();
		}

		if (pos > 1) pos = 1;
		if (pos < -1) pos = -1;

		return pos;
	}
};

#endif
