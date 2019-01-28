
#ifndef header_editview
#define header_editview

#include <map>
#include <ClanLib/GUI/component.h>
#include <ClanLib/GUI/gui_manager.h>

class MainFrame;
class Document;
class ComponentNode;

class EditView : public CL_Component
{
//!Construction:
public:
	EditView(MainFrame *mainframe);

	~EditView();

//!Attributes:
public:
	enum Mode
	{
		mode_select,
		mode_new
	};

	Document &get_document();

	const Mode &get_mode() const { return mode; }

	CL_GUIManager *root;

//!Operations:
public:
	void set_mode(Mode new_mode);

//!Implementation:
private:
	void create_simulation();

	void on_paint();

	void on_resize(int old_width, int old_height);

	void on_node_added(ComponentNode *node);

	void on_node_removed(ComponentNode *node);

	void on_node_options_changed(ComponentNode *node);

	void on_node_type_changed(ComponentNode *node);

	void on_selection_changed(std::list<ComponentNode *> &selection); 

	//: Called when mouse is pressed.
	void on_mouse_down(const CL_Key &key);

	//: Called when mouse is released.
	void on_mouse_up(const CL_Key &key);

	//: Called when mouse is down.
	void on_mouse_move(int x, int y);

	//: Returns the 6 grab rectangles used to resize a component.
	std::vector<CL_Rect> get_grab_rects(CL_Component *component);

	//: Returns the rectangle of the component, relative to the editview.
	CL_Rect get_rect(CL_Component *component);

	//: Simulation map. Makes it possible to find the simulated component
	//: from a component node.
	std::map<ComponentNode *, CL_Component *> components;

	//: List of nodes currently selected.
	std::list<ComponentNode *> selection;

	//: Edit view mode (selection mode, new component mode).
	Mode mode;

	enum MouseCaptureMode
	{
		//: Not in a mouse capture.
		mousemove_none,

		//: Mouse capture is a rectangle net select.
		mousemove_select_net,

		//: Mouse capture is a selection move.
		mousemove_move,

		//: Mouse capture is a selection resize.
		mousemove_resize
	};
	
	//: Reason mouse capture was started.
	MouseCaptureMode mouse_capture_mode;

	//: Anker point for mouse down.
	//: This is the point that mouse movement is relative to.
	CL_Point mouse_anker;

	//: Grabber used in resize mode.
	int resize_grabber;

	CL_SlotContainer slots;

	MainFrame *mainframe;
};

#endif
