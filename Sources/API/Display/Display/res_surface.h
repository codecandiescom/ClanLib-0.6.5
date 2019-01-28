/*
	$Id: res_surface.h,v 1.12 2002/06/13 14:43:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_res_surface
#define header_res_surface

#include <string>
#include <list>
class CL_Resource;
class CL_ResourceManager;
class CL_ResourceOptions;
class CL_SurfaceProvider;

//: Plugin interface for surface resources.
//- <p>CL_ResourceSource_Surface is a plugin interface for the surface resource
//- type.</p>
//-
//- <p>Applications and libraries can inherit this class to add their own
//- customized surface sources. Just as any other plugin in ClanLib, the
//- application or library has to keep one global instance of each plugin -
//- otherwise the resource manager will not be able to use the new resource
//- source.</p>
class CL_ResourceSource_Surface
{
public:
//! Construction:
	//: Resource Source Surface Constructor
	CL_ResourceSource_Surface();

	//: Resource Source Surface Destructor
	virtual ~CL_ResourceSource_Surface();

	//: Create
	virtual CL_SurfaceProvider *create(const std::string &filename)=0;

//! Attributes:
	//: Get Name
	virtual const char *get_name()=0;

	//: Can Create
	virtual bool can_create(
		std::string file_extension,
		CL_ResourceOptions &options)=0;

	//: Create
	virtual CL_SurfaceProvider *create(
		CL_Resource &resource,
		CL_ResourceOptions &options,
		CL_ResourceManager &parent)=0;

	//: Global list of surface types available (pcx, targa, png, etc etc).
	static std::list<CL_ResourceSource_Surface*> surface_sources;
};

#endif
