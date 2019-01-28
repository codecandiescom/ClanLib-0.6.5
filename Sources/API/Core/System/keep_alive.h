/*
	$Id: keep_alive.h,v 1.8 2001/09/22 15:52:11 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_keepalive
#define header_keepalive

//: Keep alive callback interface.
//- <p>If you want to add something that gets called in a
//- CL_System::keep_alive() call, inherit this class and create an instance
//- of it.</p>
class CL_KeepAlive
{
public:
//! Construction:
	//: Constructs and registers the object as a keep_alive listener.
	CL_KeepAlive();
	
	//: Unregisters the object as a keep_alive listener.
	virtual ~CL_KeepAlive();

//! Operations:
	//: Called when CL_System::keep_alive() is called.
	virtual void keep_alive()=0;
};

#endif
