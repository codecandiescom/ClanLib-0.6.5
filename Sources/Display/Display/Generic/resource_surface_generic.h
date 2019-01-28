/*
	$Id: resource_surface_generic.h,v 1.5 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_resource_surface_generic
#define header_resource_surface_generic

#include "API/Core/Resources/resourcedata.h"

class CL_Surface;
class CL_SurfaceProvider;
class CL_ResourceSource_Surface;

class CL_ResourceData_Surface : public CL_ResourceData
{
//!Construction:
public:
	CL_ResourceData_Surface(class CL_Resource &resource);
	virtual ~CL_ResourceData_Surface();

//!Attributes:
public:
	// Loaded surface for this resource.
	CL_Surface *surface;

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
