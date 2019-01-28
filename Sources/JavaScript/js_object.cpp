
#include "API/JavaScript/js_object.h"
#include "API/JavaScript/js_context.h"
#include "API/JavaScript/js_error.h"
#include <iostream>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_JSObject::CL_JSObject(const char *name)
: jsObject(0), temp(false)
{
	memset(&jsClass, 0, sizeof(JSClass));

	jsClass.name = name;
	jsClass.flags = JSCLASS_HAS_PRIVATE;
	jsClass.addProperty = JS_PropertyStub;
	jsClass.delProperty = JS_PropertyStub;
	jsClass.getProperty = JS_PropertyStub;
	jsClass.setProperty = JS_PropertyStub;
	jsClass.enumerate = JS_EnumerateStub;
	jsClass.resolve = JS_ResolveStub;
	jsClass.convert = convert;
	jsClass.finalize = finalize;
	jsClass.getObjectOps = 0;
	jsClass.checkAccess = 0;
	jsClass.call = 0;
	jsClass.construct = construct;
	jsClass.xdrObject = 0;
	jsClass.hasInstance = 0;
	jsClass.mark = 0;
	jsClass.spare = 0;
}

CL_JSObject::~CL_JSObject()
{
	if (!temp) permanentMap.erase(permanentMap.find(jsObject));
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_JSObject::newObject(JSContext *cx, JSObject *proto, JSObject *parent)
{
	jsObject = JS_NewObject(cx, &jsClass, 0, 0);

	JSBool result = JS_SetPrivate(cx, jsObject, this);
	assert(result == JS_TRUE);

	permanentMap[jsObject] = this;
}

void CL_JSObject::defineObject(JSContext *cx, JSObject *obj, const char *name, JSObject *proto, uintN flags)
{
	jsObject = JS_DefineObject(cx, obj, name, &jsClass, proto, flags);

	JSBool result = JS_SetPrivate(cx, jsObject, this);
	assert(result == JS_TRUE);

	permanentMap[jsObject] = this;
}

void CL_JSObject::initStandardClasses(JSContext *cx)
{
	JS_InitStandardClasses(cx, jsObject);
}

void CL_JSObject::initClass(
	JSContext *cx, JSObject *obj, JSObject *parent,
	JSNative constructor, uintN nargs,
	JSPropertySpec *ps,
	JSFunctionSpec *fs,
	JSPropertySpec *staticPs,
	JSFunctionSpec *staticFs)
{
	jsObject = JS_InitClass(cx, obj, parent, &jsClass, constructor, nargs, ps, fs, staticPs, staticFs);

	JSBool result = JS_SetPrivate(cx, jsObject, this);
	assert(result == JS_TRUE);

	permanentMap[JS_GetConstructor(cx, jsObject)] = this;
	permanentMap[jsObject] = this;
}

CL_JSObject *CL_JSObject::tempFromHandle(JSObject *obj)
{
	std::map<JSObject*, CL_JSObject *>::iterator it;

	// First see if handle is in perm map:
	it = permanentMap.find(obj);
	if (it != permanentMap.end()) return it->second;

	// Maybe we already got a temp handle:
	it = tempMap.find(obj);
	if (it != tempMap.end()) return it->second;

	// Ok, create temp handle:
	CL_JSObject *object = new CL_JSObject(obj, true);
	return object;
}

CL_JSObject *CL_JSObject::permanentFromHandle(JSObject *obj)
{
	std::map<JSObject*, CL_JSObject *>::iterator it;
	it = permanentMap.find(obj);
	if (it != permanentMap.end()) return it->second;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Overrideables:

JSBool CL_JSObject::onConvert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
{
	return JS_ConvertStub(cx, obj, type, vp);
}

void CL_JSObject::onFinalize(JSContext *cx, JSObject *obj)
{
	JS_FinalizeStub(cx, obj);
}

CL_JSValue CL_JSObject::onConstruct(CL_JSContext *context, CL_JSArguments &args)
{
	return CL_JSValue(context);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

CL_JSObject::CL_JSObject(JSObject *obj, bool temp)
: jsObject(obj), temp(temp)
{
	memset(&jsClass, 0, sizeof(JSClass));
	if (temp) tempMap[jsObject] = this;
	else permanentMap[jsObject] = this;
}

CL_JSObject::MemberFuncCall CL_JSObject::findMemberFunc(JSContext *cx)
{
	// Lookup the function calling stack:
	JSStackFrame *fp = 0;
	JS_FrameIterator(cx, &fp);
	assert(fp != 0);
	if (fp == 0) return 0;

	// Retrieve what function we are:
	JSFunction *jsFunc = JS_GetFrameFunction(cx, fp);
	assert(jsFunc != 0);
	if (jsFunc == 0) return 0;

	// Find the member function we are calling:
	std::map<JSFunction *, MemberFuncCall>::iterator it;
	it = functionMap.find(jsFunc);
	if (it == functionMap.end()) return 0;
	return it->second;
}

CL_JSObject *CL_JSObject::getThis(JSContext *cx, JSObject *obj)
{
	return (CL_JSObject *) JS_GetPrivate(cx, obj);
}

std::map<JSObject*, CL_JSObject *> CL_JSObject::tempMap;

std::map<JSObject*, CL_JSObject *> CL_JSObject::permanentMap;

/////////////////////////////////////////////////////////////////////////////
// JSClass, Mandatory non-null function pointer members.

JSBool CL_JSObject::convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
{
	CL_JSObject *self = getThis(cx, obj);
	assert(self != 0);
	if (self == 0) return JS_FALSE;

	return self->onConvert(cx, obj, type, vp);
}

void CL_JSObject::finalize(JSContext *cx, JSObject *obj)
{
	CL_JSObject *self = getThis(cx, obj);
//	assert(self != 0);
	if (self == 0) return;

	self->onFinalize(cx, obj);
}

JSBool CL_JSObject::getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	CL_JSObject *self = getThis(cx, obj);
	assert(self != 0);
	if (self == 0) return JS_FALSE;

	CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
	int index = CL_JSValue(context, id);

	MemberFuncPropertyGetter &getter = self->getters[index];

	try
	{
		*vp = (*self.*getter)(context);
		return JS_TRUE;
	}
	catch (CL_JSError &)
	{
		// todo: Somehow pipe error message into javascript engine.
		return JS_FALSE;
	}
}

JSBool CL_JSObject::setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	CL_JSObject *self = getThis(cx, obj);
	assert(self != 0);
	if (self == 0) return JS_FALSE;

	CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
	int index = CL_JSValue(context, id);

	MemberFuncPropertySetter &setter = self->setters[index];

	try
	{
		(*self.*setter)(CL_JSValue(context, *vp));
		return JS_TRUE;
	}
	catch (CL_JSError &)
	{
		// todo: Somehow pipe error message into javascript engine.
		return JS_FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// JSClass, Optionally non-null members.

JSBool CL_JSObject::call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	// Lookup the function calling stack:
	JSStackFrame *fp = 0;
	JS_FrameIterator(cx, &fp);
	assert(fp != 0);
	if (fp == 0) return JS_FALSE;

	// Retrieve what function we are:
	JSFunction *jsFunc = JS_GetFrameFunction(cx, fp);
	assert(jsFunc != 0);
	if (jsFunc == 0) return JS_FALSE;

	// Find the 'this' pointer of our object:
	CL_JSObject *self = getThis(cx, obj);
	assert(self != 0);
	if (self == 0) return JS_FALSE;

	// Find the member function we are calling:
	std::map<JSFunction *, MemberFuncCall>::iterator it;
	it = self->functionMap.find(jsFunc);
	if (it == self->functionMap.end()) return JS_FALSE;

	// Wrap low level JavaScript variables to nice C++ classes:
	MemberFuncCall &func = it->second;
	CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
	CL_JSArguments args(context, argc, argv);

	// Call the member function:
	try
	{
		*rval = (*self.*func)(context, args);
		return JS_TRUE;
	}
	catch (CL_JSError &)
	{
		// todo: Somehow pipe error message into javascript engine.
		return JS_FALSE;
	}
}

JSBool CL_JSObject::construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	CL_JSObject *self = permanentFromHandle(JS_GetConstructor(cx, obj)); // grr @ mozilla developers!!!
//	CL_JSObject *self = getThis(cx, JS_GetParent(cx, obj));
	assert(self != 0);
	if (self == 0) return JS_FALSE;

	CL_JSContext *context = CL_JSContext::tempFromHandle(cx);
	CL_JSArguments args(context, argc, argv);

	try
	{
		*rval = self->onConstruct(context, args);
		return JS_TRUE;
	}
	catch (CL_JSError &)
	{
		// todo: Somehow pipe error message into javascript engine.
		return JS_FALSE;
	}
}
