/*
	$Id: resourcetype_ttf.h,v 1.10 2001/11/30 11:05:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include <API/Core/Resources/resourcetype.h>
#include "API/Core/System/clanstring.h"
#include "API/Core/Resources/resource_manager.h"
#include <Display/Font/resource_font_generic.h>

class CL_Font_TTF;
class CL_ResourceType;


class CL_ResourceType_TTF : public CL_ResourceType
{
public:
	CL_ResourceType_TTF();

private:
	void connect_data(CL_Resource &resource);

	CL_ResourceData *create_from_location(CL_Resource &resource);
};

class CL_Font_Resource_TTF : public CL_ResourceData_Font
{
public:
	CL_Font_Resource_TTF(CL_Resource &resource);
//	virtual ~CL_Font_Resource_TTF();

private:

	CL_Font_TTF *ttf;
	int load_count;

	bool loaded;

	void on_load_file();
	void on_load_datafile(class CL_InputSourceProvider *input_provider);
	void on_save_datafile(class CL_OutputSourceProvider *output_provider);
	void on_unload();

	//These load mem with the actual font.
	void load_from_memory();
	//This on the other hand preps the implementation and creates the fonts
};
