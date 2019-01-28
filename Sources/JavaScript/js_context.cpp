
#include "API/JavaScript/js_context.h"
#include "API/JavaScript/js_runtime.h"
#include "API/JavaScript/js_object.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_JSContext::CL_JSContext(CL_JSRuntime &runtime, int stackSize)
: context(0), tempObject(false)
{
	context = JS_NewContext(runtime, stackSize);
	permanentMap[context] = this;
}

CL_JSContext::~CL_JSContext()
{
	if (!tempObject)
	{
		permanentMap.erase(permanentMap.find(context));
		JS_DestroyContext(context);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

JSContext *CL_JSContext::getContext()
{
	return context;
}

CL_JSContext::operator JSContext *()
{
	return getContext();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

CL_JSValue CL_JSContext::evaluateScript(
	CL_JSObject &globalObject,
	const std::string &code,
	const std::string &filename,
	int lineNumber)
{
	jsval val;
	JSBool result;
	
	result = JS_EvaluateScript(
		context, globalObject.getObject(), code.c_str(), code.length(), filename.c_str(), lineNumber, &val);

	return CL_JSValue(this, val);
}

CL_JSContext *CL_JSContext::tempFromHandle(JSContext *cx)
{
	std::map<JSContext*, CL_JSContext *>::iterator it;

	// First see if handle is in perm map:
	it = permanentMap.find(cx);
	if (it != permanentMap.end()) return it->second;

	// Maybe we already got a temp handle:
	it = tempMap.find(cx);
	if (it != tempMap.end()) return it->second;

	// Ok, create temp handle:
	CL_JSContext *context = new CL_JSContext(cx, true);
	return context;
}

CL_JSContext *CL_JSContext::permanentFromHandle(JSContext *cx)
{
	std::map<JSContext*, CL_JSContext *>::iterator it;
	it = permanentMap.find(cx);
	if (it != permanentMap.end()) return it->second;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

CL_JSContext::CL_JSContext(JSContext *context, bool tempObject)
: context(context), tempObject(tempObject)
{
	if (tempObject) tempMap[context] = this;
	else permanentMap[context] = this;
}

std::map<JSContext*, CL_JSContext *> CL_JSContext::tempMap;

std::map<JSContext*, CL_JSContext *> CL_JSContext::permanentMap;
