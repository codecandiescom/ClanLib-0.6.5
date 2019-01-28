/*
	$Id: frame_generic.h,v 1.7 2001/12/27 23:23:34 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_frame_generic
#define header_frame_generic

#include "API/GUI/frame.h"

class CL_Frame_Generic
{
// Construction:
public:
	CL_Frame_Generic(CL_Frame *self);
	~CL_Frame_Generic() {};

// Attributes:
public:
	bool fill;

// Implementation:
private:
	CL_Frame *frame;
};  

#endif
