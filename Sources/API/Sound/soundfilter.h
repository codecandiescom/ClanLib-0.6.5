/*
	$Id: soundfilter.h,v 1.5 2001/09/22 15:52:18 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_soundfilter
#define header_soundfilter

//: Sound Filter Class
class CL_SoundFilter
{
public:
//! Construction:
	//: Sound Filter Destructor
	virtual ~CL_SoundFilter() { return; }
	
//! Operations:
	//: <p>Filter callback. All sound data is passed through this function,
	//: which modifies the sample data accordingly to the function of the
	//: filter.</p>
	//:
	//: The format of the sample data is always 16 bit stereo. 
	virtual void filter(int *sample_data, int num_samples)=0;
};

#endif
