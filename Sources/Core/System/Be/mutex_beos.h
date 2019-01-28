/*
	$Id: mutex_beos.h,v 1.1 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mutex_beos
#define header_mutex_beos

#include "API/Core/System/mutex.h"

#include <be/kernel/OS.h>

class CL_Mutex_BeOS : public CL_Mutex
{
public:
	CL_Mutex_BeOS();
	virtual ~CL_Mutex_BeOS();
	
	virtual void enter();
	virtual void leave();

	virtual void wait();
	virtual void notify();
	virtual void notify_all();

private:	
	sem_id sem;
	thread_id owner;
};

#endif
