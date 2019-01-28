/*
	$Id: component_manager.h,v 1.27 2001/12/27 22:17:37 mbn Exp $

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

#ifndef header_component_manager
#define header_component_manager

#include "component.h"
#include "component_type.h"
#include "../Core/System/error.h"

#include <string>
#include <map>

class CL_StyleManager;
class CL_ResourceManager;
class CL_ComponentType;
class CL_ComponentManager_Generic;

//: Manages components loaded from a gui definition file.
// The component manager is responsible for loading a set of components from a gui definition
// file (for an example of such a file, have a look at the login_view in the CTalk example).
// After a successful construction of a component manager, it is possible get pointers to
// components defined in the definition file, thus making it easy to seperate component layout
// from the user interface logic.
class CL_ComponentManager
{
//! Construction:
public:
	//: Depricated; 'new's a component manager. Please use the constructor directly instead.
	static CL_ComponentManager *create(
		const std::string &resource_id,
		CL_ResourceManager *res_manager,
		CL_StyleManager *style, 
		CL_Component *parent);

	//: Depricated; 'new's a component manager. Please use the constructor directly instead.
	static CL_ComponentManager *create(
		const std::string &filename,
		bool is_datafile,
		CL_StyleManager *style, 
		CL_Component *parent);

	// Constructs a component manager, loading the gui definition file from a resource ID.
	CL_ComponentManager(
		const std::string &resource_id,
		CL_ResourceManager *res_manager,
		CL_StyleManager *style, 
		CL_Component *parent);

	// Constructs a component manager, loading the gui definition file from a resource ID.
	// Inherits style manager and resources from parent component.
	CL_ComponentManager(
		const std::string &resource_id,
		CL_Component *parent);

	// Constructs a component manager, loading the gui definition file from a text file.
	CL_ComponentManager(
		const std::string &filename,
		bool is_datafile,
		CL_StyleManager *style, 
		CL_Component *parent);

	// Copy constructor.
	CL_ComponentManager(const CL_ComponentManager &copy);
	
	//: Component manager destructor.
	virtual ~CL_ComponentManager();

//! Attributes:
public:
	//: List of known component types.
	typedef std::map<std::string, CL_ComponentType *> component_type_map_t;
	static component_type_map_t component_types;

	//: Creates a component of type 'component_type_name'. If no such component
	//: type exists, a CL_Error is thrown. The method invokes the specific component types
	//: virtual 'create_component' method with the passed arguments. 
	static CL_Component *create_component(
		const std::string &type,
		CL_Component *parent,
		CL_StyleManager *style)
	{
		component_type_map_t::const_iterator it(component_types.find(type));
		if (it != component_types.end())
			return (*it).second->create_component(parent, style);

		cl_throw_error(std::string("Failed to create component of type ") + type);
		return NULL;
	}

	//: Returns the component identified by the specified name in the gui definition file.
	//: If the component is not found, a CL_Error exception is thrown.
	CL_Component *get_component(const std::string &name) const;

	//: Get component identifed by the specified name in the gui definition file.
	//: If the component is not of the correct type, a CL_Error exception is thrown.
	template<class Comp>
	void get_component(const std::string &name, Comp **component) const
	{
		*component = dynamic_cast<Comp*>(get_component(name.c_str()));
		if (*component == 0) cl_throw_error("Wrong component type!");
	}

	CL_StyleManager *get_style_manager() const;

//! Operations:
public:
	// Copy assignment operator.
	CL_ComponentManager &operator =(const CL_ComponentManager &copy);

//! Implementation:
private:
	CL_ComponentManager_Generic *impl;
};

#endif
