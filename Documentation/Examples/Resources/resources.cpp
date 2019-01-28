/*

  You are looking at the ClanLib resource example.

  This example is intended to provide information about:
  * using non-surfaces resources (string, int, boolean, etc) ... since the surface resources is very much used in other examples
  * user-defined resources; if you want to create your own resources type

*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include <list>
#include <string>
#include <algorithm>
#include <map>

CL_ResourceManager *resources = 0;

using namespace std;

void display_string(string s);
void display_string_option(pair<string,CL_ResourceOption> p);

class UClass : public CL_ResourceData
{
public:
	UClass(CL_Resource &resource) : CL_ResourceData(resource)
	{
		// Connect to the file load signal:
		slot_load_file = resource.sig_load_file().connect(this, &UClass::on_load_file);
		slot_load_datafile = resource.sig_load_datafile().connect(this, &UClass::on_load_datafile);

		// Attach resource data object to the resource.
		resource.attach_data("UClass", this);
	}

	~UClass()
	{
	}

	void on_load_file()
	{
		cout << "<on_load_file>" << endl;
			
		// display all resource options; note that for_each on a map requires a pair parameter function!
		cout << "\tresource option:" << endl;
		map<string,CL_ResourceOption> options = get_resource().get_options().get_options();
		for_each(options.begin(), options.end(), display_string_option);
		cout << endl;

		// get specific resource values and set them to an object variable
		CL_ResourceOption &strength_opt = get_resource().get_options().get_option("strength");

		strength = CL_String(strength_opt.get_value()).get_as_int();
		magic = CL_String(get_resource().get_options().get_option("magic").get_value()).get_as_int();
			
		// some resource information
		cout << "\tlocation (" << get_resource().get_location() << ")" << endl;
		cout << "\tfull location (" << get_resource().get_full_location() << ")" << endl;
		cout << "\tname	 (" << get_resource().get_name() << ")" << endl;
		cout << "\ttype     (" << get_resource().get_type()<<")" << endl;
		cout << endl;
	}

	void on_load_datafile(CL_InputSourceProvider *)
	{
		cout << "<on_load_datafile>" << endl;
		on_load_file();
	}

	int strength;
	int magic;
	string name;
	CL_Slot slot_load_file;
	CL_Slot slot_load_datafile;
};

// display a string, this can be used in a for_each stl algorithm
void display_string(string s)
{
	cout << s << endl;
}

// a map has items of a "pair", so in order to for_each on a map, we need to have a
// function with parameter of pair (same as the map is)
void display_string_option(pair<string, CL_ResourceOption> p)
{
	// <<p.second.get_name() should be equal to p.first;
	cout << "\t\t" << p.first << " = " << p.second.get_value() << endl;
}

// display a UClass , based upon it's resource (name)
void display_class(string s)
{
	//load the resource
	CL_Resource res = resources->get_resource(s);
	res.load(); // WHY is this NEEDED???

	//retrieve the UClass "this" pointer from the resource data
	UClass *some_class = (UClass *)res.get_data("UClass");

	cout << "this is the info about \"" << s << "\"" <<endl;
	cout << "strength " << some_class->strength << endl;
	cout << "magic    " << some_class->magic << endl;
	cout << endl;
}

class ResourcesApp : public CL_ClanApplication
{
public:
	virtual char *get_title() { return "Resources application"; }

	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Resource Example",80,1000); // 1000 allows a y-scrollbar to be present
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();

			cout << "Hello World" << endl;

			// register new type
			// the type name mentioned here maps to whatever type="..." you put in the resource.scr
			CL_RegisterResourceType<UClass> resource_type_map("UClass"); 

			CL_Display::set_videomode(400, 200, 16, false, false);

			// write a datafile
			CL_DatafileCompiler::write("resources.scr","resources.dat");

			// open the resource datafile
			resources = new CL_ResourceManager("resources.dat", true);

			resources->load_all();

			// perform the text demo (watch the console)
			ResourceText_Demo();

			// perform the gfx demo
			ResourceGfx_Demo();

			cout<<"2nd part of this example"<<endl;
/*
			console.wait_for_key();

			// open the resource file
			resources = new CL_ResourceManager("resources.scr", false);
			// perform the text demo (watch the console)
			ResourceText_Demo();
			// perform the gfx demo
			ResourceGfx_Demo();

			console.wait_for_key();
*/

			resources->unload_all();
			
			CL_SetupDisplay::deinit();

			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}

	void ResourceText_Demo()
	{
			// get the Configuration name
			CL_ResourceData_String res_str(resources->get_resource("Configuration/name"));
			cout << "name=" << res_str.value << endl;

			// display a list of all resources
			cout << "Current resources available:" << endl;
			list<string> *resource_list = resources->get_all_resources();
			for_each(resource_list->begin(), resource_list->end(), display_string);
			cout << endl;

			// display all UnitClass resources
			cout << "Current UnitClass'es available:" << endl;
			list<string> *type_list = resources->get_resources_of_type("UClass");	
			for_each(type_list->begin(), type_list->end(), display_class);
	}

	void ResourceGfx_Demo()
	{		
		CL_Font *font = CL_Font::load("Fonts/fnt_clansoft", resources);

		CL_Surface *sur_logo = CL_Surface::load("Gfx/logo",resources);

		CL_Display::clear_display();

		while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			sur_logo->put_screen(0,0);

			font->print_center(160, 80, "Hello World");
			
			CL_Display::flip_display();

			CL_System::keep_alive();
			CL_System::sleep(10);
		}
	}
} app;

