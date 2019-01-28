
#include "API/JavaScript/js_value.h"
#include "API/JavaScript/js_object.h"
#include "API/JavaScript/js_context.h"

/////////////////////////////////////////////////////////////////////////////
// CL_JSValue Construction:

CL_JSValue::CL_JSValue()
: context(0)
{
	setNull();
}

CL_JSValue::CL_JSValue(CL_JSContext *context)
: context(context)
{
	setNull();
}

CL_JSValue::CL_JSValue(const CL_JSValue &copy)
: context(copy.context), val(copy.val)
{
}

CL_JSValue::CL_JSValue(CL_JSContext *context, jsval &value)
: context(context), val(value)
{
}

CL_JSValue::CL_JSValue(CL_JSContext *context, int i)
: context(context)
{
	setInt(i);
}

CL_JSValue::CL_JSValue(CL_JSContext *context, float f)
: context(context)
{
	setFloat(f);
}

CL_JSValue::CL_JSValue(CL_JSContext *context, double d)
: context(context)
{
	setDouble(d);
}

CL_JSValue::CL_JSValue(CL_JSContext *context, const std::string &str)
: context(context)
{
	setString(str);
}

CL_JSValue::CL_JSValue(CL_JSContext *context, const CL_JSObject *obj)
: context(context)
{
	setObject(obj);
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSValue Attributes:

jsval CL_JSValue::getJsval() const
{
	return val;
}

int CL_JSValue::getInt() const
{
	return JSVAL_TO_INT(val);
}

bool CL_JSValue::getBool() const
{
	return (JSVAL_TO_BOOLEAN(val) != 0);
}

float CL_JSValue::getFloat() const
{
	return (float) getDouble();
}

double CL_JSValue::getDouble() const
{
	return *JSVAL_TO_DOUBLE(val);
}

CL_JSObject *CL_JSValue::getObject() const
{
	return CL_JSObject::tempFromHandle(JSVAL_TO_OBJECT(val));
}

std::string CL_JSValue::getString() const
{
	JSString *str = JSVAL_TO_STRING(val);
	return std::string(JS_GetStringBytes(str), JS_GetStringLength(str));
}

CL_JSValue::operator jsval() const
{
	return getJsval();
}

CL_JSValue::operator int() const
{
	return getInt();
}

CL_JSValue::operator bool() const
{
	return getBool();
}

CL_JSValue::operator float() const
{
	return getFloat();
}

CL_JSValue::operator double() const
{
	return getDouble();
}

CL_JSValue::operator std::string() const
{
	return getString();
}

CL_JSValue::operator CL_JSObject *() const
{
	return getObject();
}

bool CL_JSValue::isBoolean() const
{
	return JSVAL_IS_BOOLEAN(val);
}

bool CL_JSValue::isDouble() const
{
	return JSVAL_IS_DOUBLE(val);
}

bool CL_JSValue::isGCThing() const
{
	return JSVAL_IS_GCTHING(val);
}

bool CL_JSValue::isInt() const
{
	return JSVAL_IS_INT(val);
}

bool CL_JSValue::isNull() const
{
	return JSVAL_IS_NULL(val);
}

bool CL_JSValue::isNumber() const
{
	return JSVAL_IS_NUMBER(val);
}

bool CL_JSValue::isObject() const
{
	return JSVAL_IS_OBJECT(val);
}

bool CL_JSValue::isPrimitive() const
{
	return JSVAL_IS_PRIMITIVE(val);
}

bool CL_JSValue::isString() const
{
	return JSVAL_IS_STRING(val);
}

bool CL_JSValue::isVoid() const
{
	return JSVAL_IS_VOID(val);
}

JSType CL_JSValue::getType() const
{
	return JS_TypeOfValue(*context, val);
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSValue Operations:

void CL_JSValue::setJsval(const jsval &value)
{
	val = value;
}

void CL_JSValue::setInt(int i)
{
	val = INT_TO_JSVAL(i);
}

void CL_JSValue::setBool(bool b)
{
	val = BOOLEAN_TO_JSVAL(b);
}

void CL_JSValue::setFloat(float f)
{
	setDouble(f);
}

void CL_JSValue::setDouble(double d)
{
	val = DOUBLE_TO_JSVAL(d);
}

void CL_JSValue::setObject(const CL_JSObject *obj)
{
	val = OBJECT_TO_JSVAL(obj->getObject());
}

void CL_JSValue::setString(const std::string &str)
{
	char *ptr = (char *) JS_malloc(*context, str.length());
	memcpy(ptr, str.c_str(), str.length());

	JSString *jsStr = JS_NewString(*context, ptr, str.length());
	val = STRING_TO_JSVAL(jsStr);
}

void CL_JSValue::setNull()
{
	val = JSVAL_NULL;
}

void CL_JSValue::setVoid()
{
	val = JSVAL_VOID;
}

CL_JSValue &CL_JSValue::operator =(const int &b)
{
	setInt(b);
	return *this;
}

CL_JSValue &CL_JSValue::operator =(const bool &b)
{
	setBool(b);
	return *this;
}

CL_JSValue &CL_JSValue::operator =(const float &b)
{
	setFloat(b);
	return *this;
}

CL_JSValue &CL_JSValue::operator =(const double &b)
{
	setDouble(b);
	return *this;
}

CL_JSValue &CL_JSValue::operator =(const std::string &b)
{
	setString(b);
	return *this;
}

CL_JSValue &CL_JSValue::operator =(const CL_JSObject *b)
{
	setObject(b);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JSValue Implementation:

