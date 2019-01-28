/*
	$Id: mousecursor_provider.h,v 1.7 2002/06/13 14:43:17 mbn Exp $

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

#ifndef header_mousecursor_provider
#define header_mousecursor_provider

#include <vector>
#include <string>

class CL_SurfaceProvider;
class CL_ResourceManager;

//: Mouse cursor image interface.
//- <p>The mouse cursor provider is an interface that describes how a mouse
//- cursor should animate, and which images it should use.</p>
//- <p>Use this class to create your cursor icon, and then use CL_MouseCursor
//- to make your cursor visible.</p>
class CL_MouseCursorProvider
{
//! Construction:
public:
	//: <p>Creates a mouse cursor source, using a surface provider as the
	//: image source.</p>
	//: <p>If the surface provider contain subframes, those will be available
	//: in the mouse cursor too. Furthermore, those frames can be used to
	//: animate the cursor, if a list of millisec delays are passed as well.</p>
	//- provider - Image of the cursor.
	//- delete_provider - If true, the surface provider will be
	//-                   deleted together with the cursor provider.
	//- animate - If true, the subframes in the surface provider will
	//-           be used to animate the cursor.
	//- frame_delays - A list of millis to wait between each frame,
	//-                when animating the cursor.
	static CL_MouseCursorProvider *create(
		CL_SurfaceProvider *provider,
		bool delete_provider=true,
		bool animate=true,
		const std::vector<int> *frame_delays=NULL);

	//: Creates a mouse provider, loaded from resources.
	//- resource_id - Resource ID of the mouse cursor.
	//- manager - Resource manager where the cursor is loaded from.
	static CL_MouseCursorProvider *load(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	//: Mouse Cursor Provider destructor
	virtual ~CL_MouseCursorProvider() { ; }

//! Attributes:
	//: Returns the surface provider associated with the mouse cursor.
	virtual CL_SurfaceProvider *get_surface_provider()=0;
	
	//: Returns true if the cursor is animated.
	virtual bool is_animated()=0;
	
	//: Returns the millisec delay between 'frame_number' and the next one.
	virtual int get_frame_delay(unsigned int frame_number)=0;
};

#endif
