
#ifndef header_threadfunc_v0
#define header_threadfunc_v0

#include "thread.h"

template<class Class>
class CL_ThreadFunc_Runnable_v0 : public CL_Runnable
{
public:
	typedef void (Class::*MemberFunc)();

	CL_ThreadFunc_Runnable_v0(Class *self, MemberFunc func)
	: self(self), func(func)
	{
	}

private:
	virtual void run()
	{
		(*self.*func)();
	}

	Class *self;
	MemberFunc func;
};

//: Member function based thread callback interface.
// The CL_ThreadFunc_v0<MyClass> is an interface used to call a member function
// in a new thread.
//
// Following code demonstrates how it is used:
// <code>
// class MyClass
// {
//   CL_Thread thread;
//
//   MyClass() : thread(CL_ThreadFunc_v0(this, MyClass::worker_function))
//   {
//     thread.start();
//   }
//
//   void worker_function() { return; }
// };
// </code>
#define CL_ThreadFunc_v0(a, b) CL_Thread(new CL_ThreadFunc_Runnable_v0(a, b), true)

/* Following code procudes internal compiler error with msvc++
template<class Class>
CL_Thread CL_ThreadFunc_v0(
	Class *self,
	CL_ThreadFunc_Runnable_v0<Class>::MemberFunc func)
{
	return CL_Thread(new CL_ThreadFunc_Runnable_v0<Class>(self, func), true);
}
*/

#endif
