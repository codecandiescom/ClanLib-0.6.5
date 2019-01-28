/*
	$Id: component_type.h,v 1.22 2002/01/04 15:20:42 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component_type
#define header_component_type

#include <set>
#include <string>

class CL_Component;
class CL_StyleManager;

#include "component_options.h"

//: Component Type
class CL_ComponentType
{
public:
//! Construction:
	//: Component Type Constructor
	CL_ComponentType(bool _is_container) : container(_is_container) {;}

	//: Component Type Destructor
	virtual ~CL_ComponentType() {;}

	//: Create component
	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style) = 0;

//! Attributes:
	//: Is container
	virtual bool is_container() const
	{
		return container;
	}

	//: Has option
	virtual bool has_option(const std::string &option_name) const
	{
		return options.find(option_name) != options.end();
	}

	struct SOptionType
	{
		enum EDatatype
		{
			STRING,
			BOOL,
			NUMBER
		} type;
	};
	//: Options
	typedef std::map<std::string, SOptionType> option_map_t;
	option_map_t options;

protected:
	bool container;
};

#endif

