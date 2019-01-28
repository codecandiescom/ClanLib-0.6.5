/*
	$Id: resource_font_generic.h,v 1.6 2002/03/02 19:43:46 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_resource_font_generic
#define header_resource_font_generic

#include "API/Core/Resources/resourcedata.h"
#include <list>

class CL_Font;
class CL_Font_Description;

class CL_ResourceData_Font : public CL_ResourceData
{
//!Construction:
public:
	CL_ResourceData_Font(class CL_Resource &resource);
	virtual ~CL_ResourceData_Font();

//!Attributes:
public:
	CL_Font *font;
	CL_Font_Description *font_desc;

//!Implementation:
protected:
	void on_load_file();
	void on_load_datafile(class CL_InputSourceProvider *input_provider);
	void on_save_datafile(class CL_OutputSourceProvider *output_provider);
	void on_unload();

	CL_Slot slot_load_file;
	CL_Slot slot_load_datafile;
	CL_Slot slot_save_datafile;
	CL_Slot slot_unload;
	std::list<class CL_SurfaceProvider*> list_delete;
};

#endif
