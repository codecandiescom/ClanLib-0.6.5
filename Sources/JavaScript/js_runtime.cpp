
#include "API/JavaScript/js_runtime.h"

/////////////////////////////////////////////////////////////////////////////
// CL_JSRuntime construction:

CL_JSRuntime::CL_JSRuntime(long gc_memory)
: runtime(JS_NewRuntime(gc_memory)), tempObject(false)
{
	permanentMap[runtime] = this;
}

CL_JSRuntime::~CL_JSRuntime()
{
	if (!tempObject)
	{
		permanentMap.erase(permanentMap.find(runtime));
		JS_DestroyRuntime(runtime);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSRuntime attributes:

JSRuntime *CL_JSRuntime::getRuntime()
{
	return runtime;
}

CL_JSRuntime::operator JSRuntime *()
{
	return getRuntime();
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSRuntime operations:

CL_JSRuntime *CL_JSRuntime::tempFromHandle(JSRuntime *rt)
{
	std::map<JSRuntime*, CL_JSRuntime *>::iterator it;

	// First see if handle is in perm map:
	it = permanentMap.find(rt);
	if (it != permanentMap.end()) return it->second;

	// Maybe we already got a temp handle:
	it = tempMap.find(rt);
	if (it != tempMap.end()) return it->second;

	// Ok, create temp handle:
	CL_JSRuntime *runtime = new CL_JSRuntime(rt, true);
	return runtime;
}

CL_JSRuntime *CL_JSRuntime::permanentFromHandle(JSRuntime *rt)
{
	std::map<JSRuntime*, CL_JSRuntime *>::iterator it;
	it = permanentMap.find(rt);
	if (it != permanentMap.end()) return it->second;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSRuntime implementation:

CL_JSRuntime::CL_JSRuntime(JSRuntime *runtime, bool tempObject)
: runtime(runtime), tempObject(tempObject)
{
	if (tempObject) tempMap[runtime] = this;
	else permanentMap[runtime] = this;
}

std::map<JSRuntime*, CL_JSRuntime *> CL_JSRuntime::tempMap;

std::map<JSRuntime*, CL_JSRuntime *> CL_JSRuntime::permanentMap;
