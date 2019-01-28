#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

// An about dialog
class AboutDialog : public CL_Window
{
public:
	AboutDialog(CL_StyleManager *style) :
		CL_Window(
			CL_Rect::center(CL_Display::get_width() / 2, CL_Display::get_height() / 2, 400, 300),
			"About",
			NULL,
			style),
		button(
			CL_Rect(0,0,100,20),
			"Quit",
			get_client_area())
	{
		slot_quit = button.sig_clicked().connect((CL_Component *) this, &CL_Component::quit);
	}

private:
	CL_Button button;
	CL_Slot slot_quit;
};

// The main application
class App : public CL_ClanApplication
{
public:
	char *get_title() { return "Dynamic GUI"; }

	bool quit;

	CL_GUIManager *gui;
	CL_StyleManager_Default *style;
		
	// These components are created here since they are used in other functions.
	CL_ListBox *listbox;
	CL_Label *label_scroll1;
	CL_Label *label_scroll2;
	CL_Label *label_radio;
	CL_ProgressBar *progress_x;
	CL_ProgressBar *progress_y;

	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			std::cout << "This example shows how to create components dynamically" << std::endl;
			std::cout << "(without using .gui files)" << std::endl;
			std::cout << std::endl;

			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGUI::init();

			CL_Display::set_videomode(640, 480, 32, false);

			// Create the GUI using the default theme
			CL_ResourceManager gui_resources("../GUIThemeDefault/gui.scr", false);
			style = new CL_StyleManager_Default(&gui_resources);
			gui = new CL_GUIManager(style);

			// Load some extra resources
			CL_ResourceManager resources("resources.scr", false);

			// Create some standard buttons (with tab-id)
			CL_Frame frame1(CL_Rect(10, 30, 113, 193), gui);
			CL_Button button_quit(CL_Rect(10, 10, 93, 33), "Quit", &frame1);
			button_quit.set_tab_id(0);
			CL_Button button_disabled(CL_Rect(10, 40, 93, 63), "Disabled", &frame1);
			button_disabled.enable(false);
			CL_Button button_about(CL_Rect(10, 70, 93, 93), "About", &frame1);
			button_about.set_tab_id(1);
			CL_Button button_message1(CL_Rect(10, 100, 93, 123), "Message 1", &frame1);
			button_message1.set_tab_id(2);
			CL_Button button_file(CL_Rect(10, 130, 93, 153), "Open File", &frame1);
			button_file.set_tab_id(3);

			// Create some inputboxes
			CL_InputBox input1(CL_Rect(140, 30, 280, 0), "Input here", gui);
			CL_InputBox input2(CL_Rect(140, 60, 280, 0), "Disabled input here", gui);
			input2.enable(false);

			// Create a checkbox
			CL_CheckBox checkbox(CL_Point(140, 90), "Check this out", gui);
			
			// Create progressbars
			progress_x = new CL_ProgressBar(CL_Rect(300, 30, 440, 50), 640, gui);
			progress_y = new CL_ProgressBar(CL_Rect(300, 60, 440, 80), 480, gui);

			// Create some bitmapped buttons
			CL_Surface *surface_up = CL_Surface::load("button_surface1", &resources);
			CL_Surface *surface_highlighted = CL_Surface::load("button_surface2", &resources);
			CL_Surface *surface_down = CL_Surface::load("button_surface3", &resources);
			CL_Surface *surface_disabled = CL_Surface::load("button_surface4", &resources);
			CL_Button button_bitmap(CL_Point(140, 120), "", surface_up, surface_down, surface_disabled, surface_highlighted, gui);
			CL_Button button_bitmap_disabled(CL_Point(140, 150), "", surface_up, surface_down, surface_disabled, surface_highlighted, gui);
			button_bitmap_disabled.enable(false);

			// Create a listbox and some extras
			CL_Frame frame2(CL_Rect(10, 200, 200, 468), gui);
			CL_Button button_add(CL_Rect(10, 10, 180, 29), "Add item", &frame2);
			CL_Button button_remove(CL_Rect(10, 29, 180, 48), "Remove item", &frame2);
			CL_Button button_view5(CL_Rect(10, 48, 180, 67), "View 5", &frame2);
			CL_Button button_view10(CL_Rect(10, 67, 180, 86), "View 10", &frame2);
			CL_Button button_view12(CL_Rect(10, 86, 180, 105), "View 12", &frame2);

			listbox = new CL_ListBox(CL_Rect(10, 105, 180), 5, &frame2);
//			listbox->set_multi_selection();

			// Add some dummy data to the listbox
			for(int i=0; i<12; i++)
			{
				CL_String string = "gqListitem ";
				string += i + 1;
				string += "...";
				listbox->insert_item(string);
			}

			// Create some scrollbars
			CL_Frame frame3(CL_Rect(210, 200, 450, 468), gui);
			CL_Frame frame4(CL_Rect(10, 10, 450-210-10, 468-180-30), &frame3);
			CL_ScrollBar scrollbar1(CL_Rect(10, 10, 30, 218), 1, 1, 0, &frame4);
			CL_ScrollBar scrollbar2(CL_Rect(40, 10, 60, 218), 1, 2, 0, &frame4);
			CL_ScrollBar scrollbar3(CL_Rect(70, 10, 90, 218), 1, 3, 0, &frame4);
			CL_ScrollBar scrollbar4(CL_Rect(100, 10, 120, 218), 1, 10, 0, &frame4);
			CL_ScrollBar scrollbar5(CL_Rect(130, 10, 150, 218), 1, 100, 0, &frame4);
			CL_ScrollBar scrollbar6(CL_Rect(160, 10, 180, 218), 100, 1000, 0, &frame4);
			CL_ScrollBar scrollbar7(CL_Rect(10, 220, 180, 240), 1, 10, 1, &frame4);
			scrollbar6.set_tracking(false);
			label_scroll1 = new CL_Label(CL_Point(190, 30), "100  ", &frame4);
			label_scroll2 = new CL_Label(CL_Point(190, 50), "100  ", &frame4);

			// Create a treeview
/*			CL_TreeView treeview(CL_Rect(500, 90, 630, 470), gui);
			CL_Button treebutton("Item 2", NULL, gui->get_style_manager());	// Create a button as a treeitem to show off :)
			CL_TreeNode *node1 =  treeview.insert_item("Item 1");
			node1->insert_item("Item 1-1");
			CL_TreeNode *node2 =  treeview.insert_item(&treebutton);
			CL_TreeNode *node21 = node2->insert_item("Item 2-1");
			node21->insert_item("Item 2-1-1");
			node21->insert_item("Item 2-1-2");
*/
			// Create a popupmenu
			CL_PopupMenu popup(CL_Rect(500, 90, 630, 470), gui);
			CL_Button menubutton("Menu Button", NULL, gui->get_style_manager());
			popup.insert_item("Open File...");
			popup.insert_item("Save File");
			CL_MenuNode *menu3 = popup.insert_item("About");
			menu3->insert_item("ClanLib");
			menu3->insert_item("Example");
			popup.insert_item(&menubutton);

			// Create some radiobuttons
			CL_RadioGroup radio;
			CL_RadioButton r1(CL_Point(350, 100), "Radio 1", gui);
			CL_RadioButton r2(CL_Point(350, 120), "Radio 2", gui);
			CL_RadioButton r3(CL_Point(350, 140), "Radio 3", gui);
			radio.add(&r1);
			radio.add(&r2);
			radio.add(&r3);
			label_radio = new CL_Label(CL_Point(350, 160), "No radiobutton selected", gui);

			// Background draw (needed for modal dialogs)
			CL_Slot slot_paint = gui->sig_paint().connect(this, &App::on_paint);

			// Mouse movement
			CL_Slot slot_mouse_move = CL_Mouse::sig_move().connect(this, &App::on_mouse_move);

			// Quit
			CL_Slot slot_quit = button_quit.sig_clicked().connect(this, &App::on_quit);
			
			// Radio button selection changed
			CL_Slot slot_radio = radio.sig_selection_changed().connect(this, &App::on_radio);

			// Show some about information (in a custom dialog)
			CL_Slot slot_about = button_about.sig_clicked().connect(this, &App::on_about);

			// Show a messagebox
			CL_Slot slot_message1 = button_message1.sig_pressed().connect(this, &App::on_message);

			// Show a filedialog
			CL_Slot slot_file = button_file.sig_clicked().connect(this, &App::on_file);

			// Add an item to the listbox
			CL_Slot slot_add_item = button_add.sig_clicked().connect(this, &App::on_add_item);

			// Remove selected items from listbox
			CL_Slot slot_remove_item = button_remove.sig_clicked().connect(this, &App::on_remove_item);

			// View 5 items in the listbox
			CL_Slot slot_view5_item = button_view5.sig_clicked().connect(this, &App::on_listbox_view, 5);

			// View 10 items in the listbox
			CL_Slot slot_view10_item = button_view10.sig_clicked().connect(this, &App::on_listbox_view, 10);

			// View 12 items in the listbox
			CL_Slot slot_view15_item = button_view12.sig_clicked().connect(this, &App::on_listbox_view, 12);

			// Display scroll values
			CL_Slot slot_scroll_moved = scrollbar6.sig_slider_moved().connect(this, &App::on_scroll_moved);
			CL_Slot slot_scroll_changed = scrollbar6.sig_value_changed().connect(this, &App::on_scroll_changed);

			// Open a popupmenu when rightclicking label
//			CL_Slot slot_right_click = label_rightclick.sig_mouse_down().connect(this, &App::on_right_click_label);

			quit = false;

			// Main loop
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && quit == false)
			{
				gui->show();

				CL_System::keep_alive();
				CL_Display::flip_display();
			}

			delete progress_x;
			delete progress_y;
			delete listbox;
			delete label_scroll1;
			delete label_scroll2;
			delete label_radio;

			delete style;
			delete gui;

			CL_SetupGUI::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	void on_paint()
	{
		CL_Display::clear_display(235.0f/255.0f, 235.0f/255.0f, 227.0f/255.0f);
	}

	void on_mouse_move(int x, int y)
	{
		progress_x->set_progress(x);
		progress_y->set_progress(y);
	}

	void on_radio(CL_RadioButton *button)
	{
		label_radio->set_text(button->get_text());
	}

	void on_about()
	{
		AboutDialog about(style);
		about.run(gui);
	}
	
	void on_scroll_moved(int value)
	{
		label_scroll1->set_text(value);
	}

	void on_scroll_changed(int value)
	{
		label_scroll2->set_text(value);
	}

	void on_message()
	{
		CL_MessageBox::info(
			"Message for the user",
			"This is a sample message...",
			"Yes", "I don't know", "No",
			gui);
	}

	void on_quit()
	{
		quit = true;
	}

	void on_remove_item()
	{
		int item = listbox->get_current_item();

		// If someone didn't select anything, just remove the first item from the list.
		if (item < 0)
			item = 0;

		listbox->remove_item(item);
	}

	void on_add_item()
	{
		CL_String string("New item ");
		string += listbox->get_count() + 1;
		listbox->insert_item(string);
	}

	void on_listbox_view(int items)
	{
		listbox->set_max_visible_items(items);
	}

	void on_file()
	{
		const std::string filename = CL_FileDialog::open("", "*.cpp", gui);
		if(filename != "")
			CL_MessageBox::info("You selected a filename", filename, gui);
	}

/*	void on_right_click_label(const CL_Key &key)
	{
		if(key.id != CL_MOUSE_RIGHTBUTTON)
			return;

		// Create a menu (TODO: This leaks memory!)
		CL_PopupMenu *popupmenu1 = new CL_PopupMenu(CL_Point(CL_Mouse::get_x(), CL_Mouse::get_y()), gui);
		CL_MenuItem *item1 = new CL_MenuItem("Open File", popupmenu1);
		CL_MenuItem *item2 = new CL_MenuItem(popupmenu1);
		CL_Button *button = new CL_Button("A normal button", popupmenu1);
		CL_MenuItem *item3 = new CL_MenuItem(popupmenu1);
		CL_MenuItem *item4 = new CL_MenuItem("Save File", popupmenu1);

		CL_PopupMenu *popupmenu2 = new CL_PopupMenu(CL_Point(0, 0), popupmenu1);
		CL_MenuItem *item21 = new CL_MenuItem("Sub-popupmenu", popupmenu2);
		CL_MenuItem *item22 = new CL_MenuItem("Cool!", popupmenu2);

		CL_MenuItem *item5 = new CL_MenuItem("Quit", popupmenu1);

		popupmenu1->set_focus();
	}
*/
} app;
