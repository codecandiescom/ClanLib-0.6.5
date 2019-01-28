/*
	$Id: mutex.h,v 1.11 2001/12/05 21:33:41 sphair Exp $

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

#ifndef header_mutex
#define header_mutex

class CL_Mutex_Generic;

//: Mutex interface.
//- If you don't know what a mutex is, read a book. :-)
class CL_Mutex
{
public:
//! Construction:
	//: Call this to create a mutex. (provided for 0.4 backward compatibility)
	static CL_Mutex *create();

	//: Construct a new mutex.
	CL_Mutex();

	//: Destroy mutex.
	~CL_Mutex();
	
//! Operations:
	//: Enter the critical section.
	void enter();

	//: Leave the critical section.
	void leave();

	//: <p>wait releases the mutex this thread has taken out and sends the
	//: thread to sleep.</p>
	//: <p>Other threads can then acquire the mutex and
	//: modify any data. When another thread calls notify, the thread
	//: that called wait will wake up again and automatically reacquire
	//: the mutex.</p>
	//:
	//: <p>Warning: This function must not be called on an unlocked 
	//: mutex. You must call enter first.</p>
	//:
	//: <p>Warning: When wait returns (after another thread has called
	//: notify/notifyAll, the current thread will have a mutex lock. Be
	//: sure to call leave at some point.</p>
	//:
	//: <p>Warning: Not implemented under Win32. If you need it,
	//: please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//: and send me the patch. :-)</p>
	void wait();

	//: <p>Notify wakes up the first thread that has gone to sleep on this
	//: mutex in order to wait for a notification.</p>
	//: <p>For safety reasons, you are advised to acquire the mutex
	//: using 'enter' before calling notify. (Of course you have to
	//: release the mutex using 'leave' after notifying, otherwise the
	//: waiting thread cannot reacquire it).</p>
	//:
	//: <p>Warning: Not implemented under Win32. If you need it,
	//: please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//: and send me the patch. :-)</p>
	void notify();

	//: NotifyAll wakes up all threads waiting on the mutex.
	//: <p>Warning: Not implemented under Win32. If you need it,
	//: please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//: and send me the patch. :-)</p>
	//: See also: CL_Mutex::notify
	void notify_all();

private:
	// Disallow copy construction.
	CL_Mutex(const CL_Mutex &copy);

	// Disallow mutex copy assignment.
	void operator =(const CL_Mutex &copy);

	CL_Mutex_Generic *impl;
};

//: Lock a mutex until the end of a scope.
//- <p>This class is a way to ensure a mutex will be released at the end of 
//- a scope. When an instance is constructed, it will lock the mutex, and
//- when the instance is destroyed (at the exit of its scope), it will
//- unlock the mutex.</p>
class CL_MutexSection
{
//! Construction:
public:
	//: Mutex Section Constructor
	CL_MutexSection(CL_Mutex *mutex, bool lock_at_construct = true)
	: mutex(mutex), lock_count(0)
	{
		if (lock_at_construct) enter();
	}

	//: Mutex Section Destructor
	virtual ~CL_MutexSection()
	{
		while (lock_count > 0) leave();
	}

//!Operations:
public:
	void enter()
	{
		mutex->enter();
		lock_count++;
	}

	void leave()
	{
		if (lock_count == 0) return;
		mutex->leave();
		lock_count--;
	}

//!Implementation:
private:
	CL_Mutex *mutex;

	int lock_count;
};

#endif
