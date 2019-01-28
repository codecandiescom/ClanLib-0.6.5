
#ifndef header_threadfunc_v2
#define header_threadfunc_v2

#include "thread.h"

template<class Class, class Param1, class Param2>
class CL_ThreadFunc_Runnable_v2 : public CL_Runnable
{
public:
	typedef void (Class::*MemberFunc)(Param1 &param1, Param2 &param2);

	CL_ThreadFunc_Runnable_v2(Class *self, MemberFunc func, const Param1 &param1, const Param2 &param2)
	: self(self), func(func), param1(param1), param2(param2)
	{
	}

private:
	virtual void run()
	{
		(*self.*func)(param1, param2);
	}

	Class *self;
	MemberFunc func;
	Param1 param1;
	Param2 param2;
};

//: Member function based thread callback interface.
// The CL_ThreadFunc_v2<MyClass, MyParam1, MyParam2> is an interface used to call a member function
// in a new thread.
//
// Following code demonstrates how it is used:
// <code>
// class MyClass
// {
//   CL_Thread thread;
//
//   MyClass() : thread(CL_ThreadFunc_v2(this, MyClass::worker_function, 15, 16))
//   {
//     thread.start();
//   }
//
//   void worker_function(int value1, int value2) { assert(value1 == 15 && value2 == 16); }
// };
// </code>
#define CL_ThreadFunc_v2(a, b, c, d) CL_Thread(new CL_ThreadFunc_Runnable_v1(a, b, c, d), true)

/* Following code procudes internal compiler error with msvc++
template<class Class, class Param1, class Param2>
CL_Thread CL_ThreadFunc_v2(
	Class *self,
	CL_ThreadFunc_Runnable_v2<Class, Param1, Param2>::MemberFunc func,
	const Param1 &param1,
	const Param2 &param2)
{
	return CL_Thread(new CL_ThreadFunc_Runnable_v2<Class, Param1, Param2>(self, func, param1, param2), true);
}
*/

#endif
