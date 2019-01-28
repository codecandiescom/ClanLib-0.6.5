/*
	$Id: fadefilter.h,v 1.6 2001/10/10 12:51:05 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="Filters"
//! header=sound.h

#ifndef header_fadefilter
#define header_fadefilter

#include "../soundfilter.h"

class CL_FadeFilter_Generic;

//: Fade Filter Class
class CL_FadeFilter : public CL_SoundFilter
{
public:
//! Construction:
	//: Fade Filter Constructor
	CL_FadeFilter(int initial_volume);

	//: Fade Filter Destructor
	virtual ~CL_FadeFilter();

//! Operations:
	//: Fade to volume
	void fade_to_volume(int new_volume, int duration = 1000);

	//: Filter
	virtual void filter(int *data, int size);

//! Implementation:
private:
	CL_FadeFilter(const CL_FadeFilter &copy) { return; } // disallow copy construction.
	CL_FadeFilter_Generic *impl;
};

#endif
