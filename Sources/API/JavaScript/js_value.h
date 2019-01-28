//! clanJavaScript="Framework"
//! header=javascript.h

#ifndef header_js_value
#define header_js_value

#include <string>
#include "js_config.h"

class CL_JSContext;
class CL_JSObject;
class CL_JSClass;

class CL_JSValue
{
// Construction:
public:
	CL_JSValue();

	CL_JSValue(CL_JSContext *context);

	CL_JSValue(const CL_JSValue &copy);

	CL_JSValue(CL_JSContext *context, jsval &value);

	CL_JSValue(CL_JSContext *context, int i);

	CL_JSValue(CL_JSContext *context, float f);

	CL_JSValue(CL_JSContext *context, double d);

	CL_JSValue(CL_JSContext *context, const std::string &str);

	CL_JSValue(CL_JSContext *context, const CL_JSObject *obj);

// Attributes:
public:
	jsval getJsval() const;

	int getInt() const;

	bool getBool() const;

	float getFloat() const;

	double getDouble() const;

	CL_JSObject *getObject() const;

	std::string getString() const;

	operator jsval() const;

	operator int() const;

	operator bool() const;

	operator float() const;

	operator double() const;

	operator std::string() const;

	operator CL_JSObject *() const;

	bool isBoolean() const;

	bool isDouble() const;

	bool isGCThing() const;

	bool isInt() const;

	bool isNull() const;

	bool isNumber() const;

	bool isObject() const;

	bool isPrimitive() const;

	bool isString() const;

	bool isVoid() const;

	JSType getType() const;

// Operations:
public:
	void setJsval(const jsval &value);

	void setInt(int i);

	void setBool(bool b);

	void setFloat(float f);

	void setDouble(double d);

	void setObject(const CL_JSObject *obj);

	void setString(const std::string &str);

	void setNull();

	void setVoid();

	CL_JSValue &operator =(const int &b);

	CL_JSValue &operator =(const bool &a);

	CL_JSValue &operator =(const float &b);

	CL_JSValue &operator =(const double &b);

	CL_JSValue &operator =(const std::string &b);

	CL_JSValue &operator =(const CL_JSObject *b);

// Implementation:
private:
	CL_JSContext *context;

	jsval val;
};

#endif
