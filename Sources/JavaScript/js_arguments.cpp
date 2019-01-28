
#include "API/JavaScript/js_arguments.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_JSArguments::CL_JSArguments(CL_JSContext *cx, uintN argc, jsval *argv)
: dummy(cx)
{
	numValues = argc;
	values = new CL_JSValue[numValues];
	for (int i=0; i<numValues; i++)
	{
		values[i] = CL_JSValue(cx, argv[i]);
	}
}

CL_JSArguments::~CL_JSArguments()
{
	delete[] values;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_JSValue &CL_JSArguments::operator[](int arg)
{
	if (arg < 0 || arg >= numValues)
	{
		dummy.setNull();
		return dummy;
	}
	return values[arg];
}

int CL_JSArguments::size() const
{
	return numValues;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
