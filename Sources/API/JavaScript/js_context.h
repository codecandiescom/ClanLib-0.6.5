//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_context
#define header_js_context

#include "js_config.h"
#include "js_value.h"
#include <map>
#include <string>

class CL_JSRuntime;
class CL_JSObject;

class CL_JSContext
{
// Construction:
public:
	CL_JSContext(CL_JSRuntime &runtime, int stackSize);

	~CL_JSContext();

// Attributes:
public:
	JSContext *getContext();

	operator JSContext *();

// Operations:
public:
	CL_JSValue evaluateScript(
		CL_JSObject &globalObject,
		const std::string &code,
		const std::string &filename = "",
		int lineNumber = 1);

	static CL_JSContext *tempFromHandle(JSContext *cx);

	static CL_JSContext *permanentFromHandle(JSContext *cx);

// Implementation:
private:
	CL_JSContext(JSContext *context, bool tempObject);

	JSContext *context;

	bool tempObject;

	static std::map<JSContext*, CL_JSContext *> tempMap;

	static std::map<JSContext*, CL_JSContext *> permanentMap;
};

#endif
