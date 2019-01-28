/*
	$Id: inverse_echofilter.h,v 1.6 2001/10/10 12:51:05 sphair Exp $

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

#ifndef header_inverse_echofilter
#define header_inverse_echofilter

#include "../soundfilter.h"

class CL_InverseEchoFilter_Generic;

//: Inverse Echo Filter Class
class CL_InverseEchoFilter : public CL_SoundFilter
{
public:
//! Construction:
	//: Inverse Echo Filter Constructor
	CL_InverseEchoFilter(int buffer_size = 64*1024);

	//: Inverse Echo Filter Destructor
	virtual ~CL_InverseEchoFilter();

//! Operations:
	//: Filter
	virtual void filter(int *data, int size);

//! Implementation:
private:
	CL_InverseEchoFilter(const CL_InverseEchoFilter &copy) { return; } // disallow copy construction.
	CL_InverseEchoFilter_Generic *impl;
};

#endif
