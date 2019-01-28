/*
	$Id: echofilter.h,v 1.6 2001/10/10 12:51:05 sphair Exp $

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

#ifndef header_echofilter
#define header_echofilter

#include "../soundfilter.h"

class CL_EchoFilter_Generic;

//: Echo Filter Class
class CL_EchoFilter : public CL_SoundFilter
{
public:
//! Construction:
	//: Echo Filter Constructor
	CL_EchoFilter(int buffer_size = 32*1024, int shift_factor = 2);

	//: Echo Filter Destructor
	virtual ~CL_EchoFilter();

//! Operations:
	//: Filter
	virtual void filter(int *data, int size);

//! Implementation:
private:
	CL_EchoFilter(const CL_EchoFilter &copy) { return; } // disallow copy construction.
	CL_EchoFilter_Generic *impl;
};

#endif
