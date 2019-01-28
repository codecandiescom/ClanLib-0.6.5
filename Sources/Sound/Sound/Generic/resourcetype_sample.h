/*
	$Id: resourcetype_sample.h,v 1.3 2001/04/13 21:45:15 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Sample resourcetype header file

*/

//! l2_component="Resources"

#ifndef header_resourcetype_sample
#define header_resourcetype_sample

#include "API/Core/Resources/resourcedata.h"

class CL_SoundBuffer;
class CL_InputSourceProvider;
class CL_OutputSourceProvider;

class CL_ResourceData_Sample : public CL_ResourceData
{
//!Construction:
public:
	CL_ResourceData_Sample(class CL_Resource &resource);
	virtual ~CL_ResourceData_Sample();

//!Attributes:
public:
	CL_SoundBuffer *soundbuffer;
	// Loaded soundbuffer for this resource.

//!Implementation:
private:
	void on_load_file();
	void on_load_datafile(class CL_InputSourceProvider *input_provider);
	void on_save_datafile(class CL_OutputSourceProvider *output_provider);
	void on_unload();

	CL_Slot slot_load_file;
	CL_Slot slot_load_datafile;
	CL_Slot slot_save_datafile;
	CL_Slot slot_unload;
};

#endif
