/*
	$Id: generic_surfaceprovider.h,v 1.6 2001/09/22 15:52:12 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_surfaceprovider_generic
#define header_surfaceprovider_generic

#include "../Display/surfaceprovider.h"
#include "../Display/surface.h"

//: Reference counting lock/unlock surface provider.
class CL_SurfaceProvider_Generic : public CL_SurfaceProvider
{
public:
//! Construction:
	//: Surface Provider Generic Constructor
	CL_SurfaceProvider_Generic();

	//: Surface Provider Generic Destructor
	virtual ~CL_SurfaceProvider_Generic() { ; }

//! Operations:
	//: Lock
	virtual void lock();

	//: UnLock
	virtual void unlock();

protected:
	virtual void perform_lock()=0;
	virtual void perform_unlock()=0;

private:
	int ref_count;
};

#endif
