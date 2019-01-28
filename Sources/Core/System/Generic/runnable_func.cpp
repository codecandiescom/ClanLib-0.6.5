/*
	$Id: runnable_func.cpp,v 1.2 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifndef header_runnable_func
#define header_runnable_func

#include "API/Core/System/thread.h"

class CL_Runnable_Func : public CL_Runnable
{
public:
	CL_Runnable_Func(int (*func)(void*), void* value)
	{
		this->func = func;
		this->value = value;
	}
	
	virtual void run()
	{
		func(value);
	}

private:
	int (*func)(void*);
	void* value;
};

CL_Thread *CL_Thread::create(int (*func)(void*), void* value)
{
	return CL_Thread::create(
		new CL_Runnable_Func(func, value));
}
	
#endif
