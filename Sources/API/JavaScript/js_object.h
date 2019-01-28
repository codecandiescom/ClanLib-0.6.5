//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_object
#define header_js_object

#include "js_config.h"
#include "js_value.h"
#include "js_arguments.h"
#include <string>
#include <map>
#include <vector>

class CL_JSObject
{
// Construction:
public:
	CL_JSObject(const char *name = "Anonymous");

	~CL_JSObject();

// Attributes:
public:
	typedef CL_JSValue (CL_JSObject::*MemberFuncCall)(
		CL_JSContext *context, CL_JSArguments &args);

	typedef CL_JSValue (CL_JSObject::*MemberFuncPropertyGetter)(CL_JSContext *cx);

	typedef void (CL_JSObject::*MemberFuncPropertySetter)(const CL_JSValue &value);

	const JSClass &getClass() const { return jsClass; }

	JSClass &getClass() { return jsClass; }

	const JSObject *getObject() const { return jsObject; }

	JSObject *getObject() { return jsObject; }

// Operations:
public:
	void newObject(JSContext *cx, JSObject *proto = 0, JSObject *parent = 0);

	void defineObject(JSContext *cx, JSObject *obj, const char *name, JSObject *proto, uintN flags);

	void initStandardClasses(JSContext *cx);

	void initClass(
		JSContext *cx, JSObject *obj, JSObject *parent,
		JSNative constructor, uintN nargs,
		JSPropertySpec *ps,
		JSFunctionSpec *fs,
		JSPropertySpec *staticPs,
		JSFunctionSpec *staticFs);

	template<class Class>
	void defineFunction(
		JSContext *cx, const char *name,
		CL_JSValue(Class::*func)(CL_JSContext *cx, CL_JSArguments &args),
		int argc)
	{
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, call, argc, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class RetVal, class Param1>
	void defineFunction_1(
		JSContext *cx, const char *name,
		RetVal(Class::*func)(Param1))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_1<RetVal, Param1>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 1, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class RetVal, class Param1, class Param2>
	void defineFunction_2(
		JSContext *cx, const char *name,
		RetVal(Class::*func)(Param1, Param2))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_2<RetVal, Param1, Param2>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 2, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class RetVal, class Param1, class Param2, class Param3>
	void defineFunction_3(
		JSContext *cx, const char *name,
		RetVal(Class::*func)(Param1, Param2, Param3))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_3<RetVal, Param1, Param2, Param3>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 3, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class RetVal, class Param1, class Param2, class Param3, class Param4>
	void defineFunction_4(
		JSContext *cx, const char *name,
		RetVal(Class::*func)(Param1, Param2, Param3, Param4))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_4<RetVal, Param1, Param2, Param3, Param4>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 4, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class RetVal, class Param1, class Param2, class Param3, class Param4, class Param5>
	void defineFunction_5(
		JSContext *cx, const char *name,
		RetVal(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_5<RetVal, Param1, Param2, Param3, Param4, Param5>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 5, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class Param1>
	void defineFunction_v1(
		JSContext *cx, const char *name,
		void(Class::*func)(Param1))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_v1<Param1>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 1, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class Param1, class Param2>
	void defineFunction_v2(
		JSContext *cx, const char *name,
		void(Class::*func)(Param1, Param2))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_v2<Param1, Param2>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 2, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class Param1, class Param2, class Param3>
	void defineFunction_v3(
		JSContext *cx, const char *name,
		void(Class::*func)(Param1, Param2, Param3))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_v3<Param1, Param2, Param3>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 3, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class Param1, class Param2, class Param3, class Param4>
	void defineFunction_v4(
		JSContext *cx, const char *name,
		void(Class::*func)(Param1, Param2, Param3, Param4))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_v4<Param1, Param2, Param3, Param4>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 4, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	template<class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
	void defineFunction_5(
		JSContext *cx, const char *name,
		void(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		JSBool (*ptrCall)(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		ptrCall = CL_JSObject::Call_v5<Param1, Param2, Param3, Param4, Param5>::call;
		JSFunction *jsFunc = JS_DefineFunction(cx, jsObject, name, ptrCall, 5, 0);
		functionMap[jsFunc] = (MemberFuncCall) func;
	}

	void defineProperty(
		JSContext *cx,
		const char *name,
		const CL_JSValue &value,
		int flags = 0)
	{
		JSBool result = JS_DefineProperty(cx, jsObject, name, value, 0, 0, flags);
	}

	template<class Class>
	void defineProperty(
		JSContext *cx,
		const char *name,
		CL_JSValue(Class::*getter)(CL_JSContext *cx),
		void(Class::*setter)(const CL_JSValue &value),
		int flags = 0)
	{
		int tinyid = getters.size();
		getters.push_back((MemberFuncPropertyGetter) getter);
		setters.push_back((MemberFuncPropertySetter) setter);

		JS_DefinePropertyWithTinyId(
			cx, jsObject, name, tinyid, JSVAL_NULL, CL_JSObject::getter, CL_JSObject::setter, flags);
	}

	template<class Class, class GetType, class SetType>
	void defineProperty_1(
		JSContext *cx,
		const char *name,
		GetType(Class::*getter)(),
		void(Class::*setter)(SetType),
		int flags = 0)
	{
		int tinyid = getters.size();
		getters.push_back((MemberFuncPropertyGetter) getter);
		setters.push_back((MemberFuncPropertySetter) setter);

		JS_DefinePropertyWithTinyId(
			cx, jsObject, name, tinyid, JSVAL_NULL,
			Property_1<GetType, SetType>::getter,
			Property_1<GetType, SetType>::setter,
			flags);
	}

	template<class VarType>
	void defineProperty_ptr(
		JSContext *cx,
		const char *name,
		VarType *varPtr,
		int flags = 0)
	{
		int tinyid = vars.size();
		vars.push_back(varPtr);

		JS_DefinePropertyWithTinyId(
			cx, jsObject, name, tinyid, JSVAL_NULL,
			Property_ptr<VarType>::getter,
			Property_ptr<VarType>::setter,
			flags);
	}

	template<class VarType>
	void defineProperty_obj(
		JSContext *cx,
		const char *name,
		VarType *varPtr,
		int flags = 0)
	{
		int tinyid = vars.size();
		vars.push_back(varPtr);

		JS_DefinePropertyWithTinyId(
			cx, jsObject, name, tinyid, JSVAL_NULL,
			Property_obj<VarType>::getter,
			Property_obj<VarType>::setter,
			flags);
	}

	template<class VarType>
	void defineProperty_objPtr(
		JSContext *cx,
		const char *name,
		VarType *varPtr,
		int flags = 0)
	{
		int tinyid = vars.size();
		vars.push_back(varPtr);

		JS_DefinePropertyWithTinyId(
			cx, jsObject, name, tinyid, JSVAL_NULL,
			Property_objPtr<VarType>::getter,
			Property_objPtr<VarType>::setter,
			flags);
	}

	static CL_JSObject *tempFromHandle(JSObject *obj);

	static CL_JSObject *permanentFromHandle(JSObject *obj);

// Overrideables:
public:
	virtual JSBool onConvert(JSContext *cx, JSObject *obj, JSType type, jsval *vp);

	virtual void onFinalize(JSContext *cx, JSObject *obj);

	virtual CL_JSValue onConstruct(CL_JSContext *context, CL_JSArguments &args);

// Implementation:
public:
	CL_JSObject(JSObject *obj, bool temp);

	MemberFuncCall findMemberFunc(JSContext *cx);

	JSClass jsClass;

	JSObject *jsObject;

	bool temp;

	std::map<JSFunction *, MemberFuncCall> functionMap;

	std::vector<MemberFuncPropertyGetter> getters;

	std::vector<MemberFuncPropertySetter> setters;

	std::vector<void *> vars;

	static CL_JSObject *getThis(JSContext *cx, JSObject *obj);

	static std::map<JSObject*, CL_JSObject *> tempMap;

	static std::map<JSObject*, CL_JSObject *> permanentMap;

// JSClass, Mandatory non-null function pointer members.
private:
	static JSBool convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp);

	static void finalize(JSContext *cx, JSObject *obj);

	static JSBool getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	static JSBool setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	template<class GetType, class SetType>
	class Property_1
	{
	public:
		static JSBool getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			GetType(CL_JSObject::*getter)() = (GetType(CL_JSObject::*)()) self->getters[index];

			try
			{
				*vp = CL_JSValue(context, (*self.*getter)());
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}

		static JSBool setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			void(CL_JSObject::*setter)(SetType) = (void(CL_JSObject::*)(SetType)) self->setters[index];

			try
			{
				(*self.*setter)(CL_JSValue(context, *vp));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class VarType>
	class Property_ptr
	{
	public:
		static JSBool getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			VarType *ptr = (VarType *) self->vars[index];

			try
			{
				*vp = CL_JSValue(context, *ptr);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}

		static JSBool setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			VarType *ptr = (VarType *) self->vars[index];

			try
			{
				*ptr = CL_JSValue(context, *vp);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class VarType>
	class Property_obj
	{
	public:
		static JSBool getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			VarType *ptr = (VarType *) self->vars[index];

			try
			{
				*vp = CL_JSValue(context, ptr);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}

		static JSBool setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			return JS_FALSE; // read only property.
		}
	};

	template<class VarType>
	class Property_objPtr
	{
	public:
		static JSBool getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			VarType *ptr = (VarType *) self->vars[index];

			try
			{
				*vp = CL_JSValue(context, *ptr);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}

		static JSBool setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			int index = CL_JSValue(context, id);
			VarType *ptr = (VarType *) self->vars[index];

			try
			{
				VarType dyncast = dynamic_cast<VarType>((CL_JSObject *) CL_JSValue(context, *vp));
				if (dyncast == 0) throw CL_JSError();
				*ptr = dyncast;
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

// JSClass, Optionally non-null members.
public:
	static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	template<class RetVal, class Param1>
	class Call_1
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			RetVal(CL_JSObject::*func)(Param1) = (RetVal(CL_JSObject::*)(Param1)) self->findMemberFunc(cx);
			try
			{
				*rval = CL_JSValue(context, (*self.*func)(args[0]));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class RetVal, class Param1, class Param2>
	class Call_2
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			RetVal(CL_JSObject::*func)(Param1, Param2) = (RetVal(CL_JSObject::*)(Param1, Param2)) self->findMemberFunc(cx);
			try
			{
				*rval = CL_JSValue(context, (*self.*func)(args[0], args[1]));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class RetVal, class Param1, class Param2, class Param3>
	class Call_3
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			RetVal(CL_JSObject::*func)(Param1, Param2, Param3) = (RetVal(CL_JSObject::*)(Param1, Param2, Param3)) self->findMemberFunc(cx);
			try
			{
				*rval = CL_JSValue(context, (*self.*func)(args[0], args[1], args[2]));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class RetVal, class Param1, class Param2, class Param3, class Param4>
	class Call_4
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			RetVal(CL_JSObject::*func)(Param1, Param2, Param3, Param4) = (RetVal(CL_JSObject::*)(Param1, Param2, Param3, Param4)) self->findMemberFunc(cx);
			try
			{
				*rval = CL_JSValue(context, (*self.*func)(args[0], args[1], args[2], args[3]));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class RetVal, class Param1, class Param2, class Param3, class Param4, class Param5>
	class Call_5
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			RetVal(CL_JSObject::*func)(Param1, Param2, Param3, Param4, Param5) = (RetVal(CL_JSObject::*)(Param1, Param2, Param3, Param4, Param5)) self->findMemberFunc(cx);
			try
			{
				*rval = CL_JSValue(context, (*self.*func)(args[0], args[1], args[2], args[3], args[4]));
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class Param1>
	class Call_v1
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			void(CL_JSObject::*func)(Param1) = (void(CL_JSObject::*)(Param1)) self->findMemberFunc(cx);
			try
			{
				(*self.*func)(args[0]);
				*rval = CL_JSValue(context);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class Param1, class Param2>
	class Call_v2
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			void(CL_JSObject::*func)(Param1, Param2) = (void(CL_JSObject::*)(Param1, Param2)) self->findMemberFunc(cx);
			try
			{
				(*self.*func)(args[0], args[1]);
				*rval = CL_JSValue(context);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class Param1, class Param2, class Param3>
	class Call_v3
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			void(CL_JSObject::*func)(Param1, Param2, Param3) = (void(CL_JSObject::*)(Param1, Param2, Param3)) self->findMemberFunc(cx);
			try
			{
				(*self.*func)(args[0], args[1], args[2]);
				*rval = CL_JSValue(context);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class Param1, class Param2, class Param3, class Param4>
	class Call_v4
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			void(CL_JSObject::*func)(Param1, Param2, Param3, Param4) = (void(CL_JSObject::*)(Param1, Param2, Param3, Param4)) self->findMemberFunc(cx);
			try
			{
				(*self.*func)(args[0], args[1], args[2], args[3]);
				*rval = CL_JSValue(context);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	template<class Param1, class Param2, class Param3, class Param4, class Param5>
	class Call_v5
	{
	public:
		static JSBool call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			CL_JSObject *self = getThis(cx, obj);
			CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
			CL_JSArguments args(context, argc, argv);

			void(CL_JSObject::*func)(Param1, Param2, Param3, Param4, Param5) = (void(CL_JSObject::*)(Param1, Param2, Param3, Param4, Param5)) self->findMemberFunc(cx);
			try
			{
				(*self.*func)(args[0], args[1], args[2], args[3], args[4]);
				*rval = CL_JSValue(context);
				return JS_TRUE;
			}
			catch (CL_JSError &)
			{
				return JS_FALSE;
			}
		}
	};

	static JSBool construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
};

#endif
