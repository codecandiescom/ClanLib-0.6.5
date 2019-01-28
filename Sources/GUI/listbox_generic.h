/*
	$Id: listbox_generic.h,v 1.28 2002/03/01 19:36:55 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_listbox_generic
#define header_listbox_generic

#include <string>
#include <vector>

#include "API/signals.h"
#include "API/GUI/layout_manager.h"
#include "API/GUI/listitem.h"

class CL_ListBox;
class CL_ScrollBar;
class CL_Component;
class CL_ComponentOptions;
class CL_StyleManager;
class CL_Key;
class CL_InputDevice;
class CL_Rect;

class CL_ListBox_Generic
{
// Construction:
public:
	CL_ListBox_Generic(
		CL_ListBox *self,
		int max_visible_items = -1);

// Attributes:
public:
	int get_count() const;

	std::vector<CL_ListItem> &get_items();
	std::vector<std::string> get_selected_items();
	const std::string &get_text(int index) const;
	const std::string &get_current_text() const;
	int get_current_item() const;
	bool is_selected(int index) const;
	bool is_multi_selection() const;
	bool has_scrollbar() const;

	int get_item_height() const;
	int get_top_item() const;
	int get_max_visible_items() const;

	CL_Component *client_area;

// Operations:
public:
	int insert_item(const std::string &text, int index);
	void remove_item(int index);
	void change_item(const std::string &text, int index);

	void set_current_item(int index);
	void clear_selection();
	void set_selected(int index, bool select);
	void select_all(bool select);
	void invert_selection();
	void set_multi_selection(bool enable);

	void set_item_height(int height);
	void set_max_visible_items(int count);
	void set_top_item(int index, bool update_scrollbar);

	void sort(bool ascending);

	void clear();

// Signals:
public:
	CL_Signal_v1<int> sig_highlighted;
	CL_Signal_v1<int> sig_activated;
	CL_Signal_v0 sig_selection_changed;

// Slots:
private:
	CL_Slot slot_set_options;
	CL_Slot slot_mouse_down;
	CL_Slot slot_scroll_change;
	CL_Slot slot_child_add, slot_child_remove;
	CL_Slot slot_resize;

// Callbacks:
private:
	void on_set_options(const CL_ComponentOptions &options);
	void on_mouse_down(const CL_Key &key);
	void on_scroll_change(int new_offset);
	void on_child_add(CL_Component *child);
	void on_child_remove(CL_Component *child);
	void on_resize(int old_width, int old_height);

// Implementation:
protected:
	void update_scrollbar();

	int item_height;
	int max_visible_items;

	int scroll_offset;

	bool visible_scrollbar;
	bool multi_selection;

	CL_ListBox *listbox;
	CL_ScrollBar *scrollbar;
	CL_LayoutManager layout;

	std::vector<CL_ListItem> items;
};

#endif
