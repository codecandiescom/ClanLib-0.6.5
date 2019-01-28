/*
	$Id: thread.h,v 1.14 2001/12/19 19:23:03 mbn Exp $

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

#ifndef header_thread
#define header_thread

class CL_Thread_Generic;

//: Thread callback interface.
//- When a thread is created, it will call run() in its attached CL_Runnable interface.
class CL_Runnable
{
public:
//! Overrideables:
	//: Called when a thread is run.
	virtual void run()=0;
};

//: Thread Priority Enum
enum EThreadPriority
{
	cl_priority_above_normal,
	cl_priority_below_normal,
	cl_priority_highest,
	cl_priority_idle,
	cl_priority_lowest,
	cl_priority_normal,
	cl_priority_time_critical
};

//: Thread Class
class CL_Thread
{
//! Construction:
public:
	//: Create a thread that uses the CL_Runnable callback interface.
	//: Provided for ClanLib-0.4 backward compatibility.
	//- runnable - Class to call when thread is started.
	static CL_Thread *create(CL_Runnable *runnable, bool delete_runnable = false);

	//: Create a thread that calls the function specified, with the value specified.
	//: Provided for ClanLib-0.4 backward compatibility.
	//- func - Function that gets called at thread start.
	//- value - Value passed to the function at thread start.
	static CL_Thread *create(int (*func)(void*), void* value);

	//: Create a thread that uses the CL_Runnable callback interface.
	CL_Thread(CL_Runnable *runnable, bool delete_runnable = false);

	//: Create a thread that calls the function specified, with the value specified.
	CL_Thread(int (*func)(void*), void *value);

	//: Copy constructor.
	CL_Thread(const CL_Thread &copy);

	//: Null thread.
	CL_Thread();
	
	//: Destructor.
	~CL_Thread();
	
//! Operations:
public:
	//: Copy assignement operator.
	CL_Thread &operator =(const CL_Thread &copy);

	//: Starts the thread.
	void start();

	//: Terminate the thread. (use with caution under win98)
	void terminate();

	//: Wait until the thread finishes its execution.
	void wait();

	//: Set the thread priority.
	void set_priority(EThreadPriority priority);

//! Implementation:
private:
	CL_Thread_Generic *impl;
};

#endif
