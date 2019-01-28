#include <ClanLib/javascript.h>
#include <iostream>

class MemberObject : public CL_JSObject
{
public:
	MemberObject(CL_JSContext &context)
	: x(10), y(20)
	{
		newObject(context);
		defineProperty_ptr(context, "x", &x);
		defineProperty_ptr(context, "y", &y);
	}

	int x, y;
};

class GlobalObject : public CL_JSObject
{
//!Construction:
public:
	GlobalObject(CL_JSContext &context)
	: obj(context), obj2(new MemberObject(context)), printerName("LAS-HP"), debugLevel(1)
	{
		newObject(context);
		initStandardClasses(context);

		defineFunction(context, "print", &GlobalObject::print, 1);
		defineFunction_1(context, "test", &GlobalObject::test);
		defineFunction_2(context, "sum", &GlobalObject::sum);
		defineFunction_3(context, "sum3", &GlobalObject::sum3);
		defineFunction_4(context, "sum4", &GlobalObject::sum4);
		defineFunction_5(context, "sum5", &GlobalObject::sum5);
		defineFunction_v1(context, "voidFunc", &GlobalObject::voidFunc);

		defineProperty(context, "debugLevel", &GlobalObject::getDebugLevel, &GlobalObject::setDebugLevel);
		defineProperty_1(context, "value", &GlobalObject::getValue, &GlobalObject::setValue);
		defineProperty_ptr(context, "printerName", &printerName);

		defineProperty_obj(context, "memberObj", &obj);
		defineProperty_objPtr(context, "memberObj2", &obj2);
	}

	~GlobalObject()
	{
//		delete obj2;
	}

//!Implementation:
private:
	MemberObject obj;
	MemberObject *obj2;
	std::string printerName;

	std::string getValue()
	{
		return value;
	}

	void setValue(std::string v)
	{
		value = v;
	}

	int test(std::string str)
	{
		std::cout << "test was called with " << str.c_str() << std::endl;
		return 42;
	}

	float sum(float a, float b)
	{
		return a+b;
	}

	float sum3(float a, float b, float)
	{
		return a+b;
	}

	float sum4(float a, float b, float, float)
	{
		return a+b;
	}

	std::string sum5(float a, float b, float, float, float)
	{
		return "bla bla!";
	}

	void voidFunc(int a)
	{
	}

	CL_JSValue print(CL_JSContext *cx, CL_JSArguments &args)
	{
		if (debugLevel > 0)
		{
			std::string msg = args[0];
			printf("Script Debug: %s\r\n", msg.c_str());
		}

		return CL_JSValue(cx);
	}

	void setDebugLevel(const CL_JSValue &value)
	{
		debugLevel = value;
	}

	CL_JSValue getDebugLevel(CL_JSContext *context)
	{
		return CL_JSValue(context, debugLevel);
	}

	int debugLevel;

	std::string value;
};

class ActiveXObject : public CL_JSObject
{
//!Construction:
public:
	ActiveXObject(CL_JSContext *context, CL_JSArguments &args)
	{
		clsId = args[0];

		newObject(*context);
		defineFunction(*context, "calc", &ActiveXObject::calc, 1);
	}

//!Implementation:
private:
	CL_JSValue calc(CL_JSContext *cx, CL_JSArguments &args)
	{
		char str[1024];
		sprintf(str, "%s.calc called with %d", clsId.c_str(), (int) args[0]);

		return CL_JSValue(cx, std::string(str));
	}

	std::string clsId;
};

int main(int argc, char **argv)
{
	CL_JSRuntime runtime(1024*1024);
	CL_JSContext context(runtime, 8*1024);
	GlobalObject globalObj(context);
	CL_JSClassFactory<ActiveXObject> staticObj("ActiveXObject", 1, context, globalObj);

	char *evalStr = "var obj = new ActiveXObject(\"odbc.connection\"); print(obj.calc(test(\"fisk\"))); debugLevel = 0; print(\"Bla bla\"); printerName;";
	CL_JSValue result = context.evaluateScript(globalObj, evalStr);
	std::cout << "Result: " << result.getString().c_str() << std::endl;

	evalStr = "memberObj2.x;";
	int i = context.evaluateScript(globalObj, evalStr);
	std::cout << "Result: " << i << std::endl;

	return 0;
}
