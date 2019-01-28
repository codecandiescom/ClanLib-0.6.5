/*
	$Id: mutex_beos.cpp,v 1.1 2000/07/18 17:04:27 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include "mutex_beos.h"

CL_Mutex *CL_Mutex::create()
{
	return new CL_Mutex_BeOS;
}

CL_Mutex_BeOS::CL_Mutex_BeOS()
{
	sem = create_sem(1, "ClanLibMutex");
	owner = -1;
}

CL_Mutex_BeOS::~CL_Mutex_BeOS()
{
	delete_sem(sem);
}
	
void CL_Mutex_BeOS::enter()
{
	status_t err = acquire_sem(sem);
	if(!err)
		owner = find_thread(NULL);
}

void CL_Mutex_BeOS::leave()
{
	if(owner != find_thread(NULL))
		return;

	owner = -1;
	release_sem(sem);
}

void CL_Mutex_BeOS::wait()
{
//	pthread_cond_wait(&cond,&mutex);
}

void CL_Mutex_BeOS::notify()
{
//	pthread_cond_signal(&cond);
}

void CL_Mutex_BeOS::notify_all()
{
//	pthread_cond_broadcast(&cond);
}
