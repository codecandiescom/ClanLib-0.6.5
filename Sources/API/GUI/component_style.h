/*
	$Id: component_style.h,v 1.12 2001/12/11 21:30:59 mbn Exp $

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

#ifndef header_component_style
#define header_component_style

class CL_Component;

//: Component Style interface.
// The component style class is part of the theme customization system of the
// GUI. Each 'theme' for a component is implemented by inheriating
// CL_ComponentStyle and then attaching it to the component it is responsible
// of visualizing.
// When a component is constructed, it will contact its CL_StyleManager and
// ask it to attach the component styles for its theme to the component.
// The general principle in the component style system is that a inheriated
// CL_ComponentStyle object will hook itself into signals in the component it
// is visualizing, thus keeping a kind of document/view relationship with its
// component.
// For source examples of how this is done, have a look at the GUIGL example.
class CL_ComponentStyle
{
//! Construction:
public:
	//: Construct a component style.
	CL_ComponentStyle(CL_Component *component) : comp(component) { return; }

	//: Destructor.
	virtual ~CL_ComponentStyle() { return; }

//! Attributes:
public:
	//: Returns the component that is being styled.
	CL_Component *get_component() { return comp; }

	//: Returns the component that is being styled.
	const CL_Component *get_component() const { return comp; }

//! Implementation:
private:
	CL_Component *comp;
};

#endif
