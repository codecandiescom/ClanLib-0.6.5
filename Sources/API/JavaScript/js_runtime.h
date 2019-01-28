//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_runtime
#define header_js_runtime

#include "js_config.h"
#include <map>

class CL_JSContext;

class CL_JSRuntime
{
// Construction:
public:
	CL_JSRuntime(long gc_memory);

	~CL_JSRuntime();

// Attributes:
public:
	JSRuntime *getRuntime();

	operator JSRuntime *();

// Operations:
public:
	static CL_JSRuntime *tempFromHandle(JSRuntime *rt);

	static CL_JSRuntime *permanentFromHandle(JSRuntime *rt);

// Implementation:
private:
	CL_JSRuntime(JSRuntime *runtime, bool tempObject);

	JSRuntime *runtime;

	bool tempObject;

	static std::map<JSRuntime *, CL_JSRuntime *> tempMap;

	static std::map<JSRuntime *, CL_JSRuntime *> permanentMap;
};

#endif
