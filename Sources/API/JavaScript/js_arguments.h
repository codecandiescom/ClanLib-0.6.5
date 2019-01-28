//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_arguments
#define header_js_arguments

#include "js_config.h"
#include "js_value.h"

class CL_JSContext;

class CL_JSArguments
{
// Construction:
public:
	CL_JSArguments(CL_JSContext *cx, uintN argc, jsval *argv);

	~CL_JSArguments();

// Attributes:
public:
	CL_JSValue &operator[](int arg);

	int size() const;

// Implementation:
private:
	CL_JSValue *values;

	int numValues;

	CL_JSValue dummy;
};

#endif
