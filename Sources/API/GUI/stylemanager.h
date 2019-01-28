/*
	$Id: stylemanager.h,v 1.29 2001/12/27 22:17:37 mbn Exp $

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

#ifndef header_stylemanager
#define header_stylemanager

#include <string>

class CL_ComponentOptions;
class CL_Component;
class CL_ResourceManager;
class CL_StyleManager_Generic;

// Manages the overall style of the system.
// The style manager is responsible of attaching CL_ComponentStyle objects to
// CL_Component objects.
// When a component is constructed, it will contact its style manager's
// connect_styles() function. The style manager will then construct and attach
// component styles to the component.
// The style manager also works as a 'component factory' for the component
// manager. By using create_component(), it is possible to create a component
// based on a name and component options describing it.
class CL_StyleManager
{
//! Construction:
public:
	//: Construct a style manager using the specified resources.
	CL_StyleManager(CL_ResourceManager *resources);

	//: Destructor.
	virtual ~CL_StyleManager();

//! Attributes:
public:
	//: Returns style manager's resources.
	CL_ResourceManager *get_resources();

//! Overrideables:
public:
	//: Create a component based on a type name.
	virtual CL_Component *create_component(
		const std::string &type,
		CL_Component *parent)=0;

	//: Connect component styles to component.
	//: The 'type' parameter indicates what type the component is.
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner)=0;

//! Implementation:
private:
	// Disallow copy contruction of style managers.
	CL_StyleManager(const CL_StyleManager &copy) { return; }
	
	CL_StyleManager_Generic *impl;
};

#endif
