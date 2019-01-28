/*
	$Id: init_linux.cpp,v 1.28 2001/12/22 11:25:19 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "implementation.h"
#include "appconf.h"
#include "init_linux.h"
// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include "API/Core/System/setupcore.h"
#include "API/Core/System/system.h"

static int init_ref_count = 0;
long _begin_time;

void init_system()
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	timeval tv;
	gettimeofday(&tv, NULL);
	_begin_time = (long) tv.tv_sec*(long) 1000+(long) tv.tv_usec/(long) 1000;

	#ifdef NDEBUG
	signal(SIGSEGV, deinit);
	#endif
}

void deinit_system()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	#ifdef NDEBUG
	signal(SIGSEGV,SIG_DFL); //restore default behavior
	#endif
}

unsigned int CL_System::get_time()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	long tid = (long) tv.tv_sec*(long) 1000 + (long) tv.tv_usec/(long) 1000 - _begin_time;

	return tid;
}

void CL_System::sleep(int millis)
{
	timeval tv;
	tv.tv_sec = millis/1000;
	tv.tv_usec = (millis%1000)*1000;
	select(0, 0, 0, 0, &tv);
}
