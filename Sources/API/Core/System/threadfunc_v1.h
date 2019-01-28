
#ifndef header_threadfunc_v1
#define header_threadfunc_v1

#include "thread.h"

template<class Class, class Param>
class CL_ThreadFunc_Runnable_v1 : public CL_Runnable
{
public:
	typedef void (Class::*MemberFunc)(Param &param);

	CL_ThreadFunc_Runnable_v1(Class *self, MemberFunc func, const Param &param)
	: self(self), func(func), param(param)
	{
	}

private:
	virtual void run()
	{
		(*self.*func)(param);
	}

	Class *self;
	MemberFunc func;
	Param param;
};

//: Member function based thread callback interface.
// The CL_ThreadFunc_v1<MyClass, MyParam> is an interface used to call a member function
// in a new thread.
//
// Following code demonstrates how it is used:
// <code>
// class MyClass
// {
//   CL_Thread thread;
//
//   MyClass() : thread(CL_ThreadFunc_v1(this, MyClass::worker_function, 15))
//   {
//     thread.start();
//   }
//
//   void worker_function(int value) { assert(value == 15); }
// };
// </code>
#define CL_ThreadFunc_v1(a, b, c) CL_Thread(new CL_ThreadFunc_Runnable_v1(a, b, c), true)

/* Following code procudes internal compiler error with msvc++
template<class Class, class Param>
CL_Thread CL_ThreadFunc_v1(
	Class *self,
	CL_ThreadFunc_Runnable_v1<Class, Param>::MemberFunc func,
	const Param &param)
{
	return CL_Thread(new CL_ThreadFunc_Runnable_v1<Class, Param>(self, func, param), true);
}
*/

#endif
