#include <ClanLib/core.h>
#include <ClanLib/lua.h>

class LuaTestApp : public CL_ClanApplication
{
public:
	int a,b,c;
	virtual char *get_title(){return "test";}

	virtual int main(int argc,char **argv)
		{	    	
			a=10;
			b=5;
			c=0;
			CL_LuaValue alua(a);
			alua.store("a");
			CL_LuaValue blua(b);
			blua.store("b");
			CL_LuaValue clua(c);
			clua.store("c");
			cout << (int)alua << endl;	
			cout << (int)blua << endl;
			cout << (int)clua << endl;
			CL_Lua::dostring("c =a+b ");
			clua.get("c");
			c=clua;		       
			cout << c <<" ; " << (int)clua << endl;
		}
} app;	
