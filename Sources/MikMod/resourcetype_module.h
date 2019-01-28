/*
	$Id: resourcetype_module.h,v 1.6 2001/09/08 19:12:54 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
//! component="Resources"

#ifndef header_resourcetype_module
#define header_resourcetype_module

#include "../API/Core/Resources/resource_manager.h"
#include "../API/Core/Resources/resourcetype.h"
#include "../API/Core/Resources/resource.h"
#include "../API/Core/System/clanstring.h"
#include "../API/Core/System/error.h"
#include "../API/Core/IOData/inputsource.h"
#include "../API/Core/IOData/inputsource_provider.h"

#include "API/MikMod/streamed_mikmod_sample.h"

class CL_ResourceData_Module : public CL_ResourceData
{
//!Construction:
public:
	CL_ResourceData_Module(class CL_Resource &resource);

//!Attributes:
public:
	MODULE *module;

//!Implementation:
private:
	void on_load_file();
	void on_load_datafile(class CL_InputSourceProvider *provider);
	void on_save_datafile(class CL_OutputSourceProvider *provider);
	void on_unload();

	CL_Slot slot_load_file;
	CL_Slot slot_load_datafile;
	CL_Slot slot_save_datafile;
	CL_Slot slot_unload;
};

#endif
