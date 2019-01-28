//! component="GUI"
/*
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_stylemanager_opengl
#define header_stylemanager_opengl

#include <ClanLib/GUI/stylemanager_default.h>
#include <ClanLib/GUI/component_options.h>

class CL_ResourceManager;
class CL_Component;

class CL_StyleManager_OpenGL : public CL_StyleManager_Default
{
public:
	CL_StyleManager_OpenGL(CL_ResourceManager *resources);
	virtual ~CL_StyleManager_OpenGL();

// Overridables:
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner);
};

#endif
