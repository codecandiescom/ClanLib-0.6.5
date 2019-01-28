//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_classfactory
#define header_js_classfactory

#include "js_object.h"
#include "js_value.h"

template<class Object>
class CL_JSClassFactory : public CL_JSObject
{
public:
	CL_JSClassFactory(const char *name, int argc, CL_JSContext &context, CL_JSObject &obj)
	: CL_JSObject(name)
	{
		initClass(context, obj.getObject(), 0, 0, argc, 0, 0, 0, 0);
	}

	virtual CL_JSValue onConstruct(CL_JSContext *context, CL_JSArguments &args)
	{
		return CL_JSValue(context, new Object(context, args));
	}
};

#endif
