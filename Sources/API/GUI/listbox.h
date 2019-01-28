/*
	$Id: listbox.h,v 1.48 2002/03/01 19:36:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_listbox
#define header_listbox

#include "component.h"
#include "listitem.h"

class CL_ListBox_Generic;

//: Listbox component
class CL_ListBox : public CL_Component
{
//! Construction:
public:
	//: Listbox constructor
	CL_ListBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);
	
	//: Listbox constructor
	CL_ListBox(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: <p>Creates a listbox with max_visible_items visible. It auto-calculates
	//: the height of the listbox.</p>
	CL_ListBox(
		const CL_Rect &pos,
		int max_visible_items,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: Listbox destructor
	virtual ~CL_ListBox();

//! Attributes:
public:
	//: Returns the client area of the component.
	CL_Component *get_client_area() const;

	//: Returns the number of items in the list box. 
	int get_count() const;

	//: Returns all items in the list.
	std::vector<CL_ListItem> &get_items() const;

	//: Returns all selected  items.
	std::vector<std::string> get_selected_items() const;

	//: Returns the text of the current item.
	const std::string &get_current_text() const;

	//: Returns the text at position index.
	const std::string &get_text(int index) const;

	//: Returns the index of the current item of the listbox, or -1 if no item has been selected. 
	int get_current_item() const;

	//: Returns true if item index is selected. Returns false if it is not selected or if there is an error.
	bool is_selected(int index) const;

	//: Returns true if the listbox is in multiselection mode.
	bool is_multi_selection() const;

	//: Returns the height of one item.
	int get_item_height() const;

	//: Returns the index of the item at the top of the screen.
	int get_top_item() const;

	//: Returns number of items maximum visible in listbox (without scrolling)
	int get_max_visible_items() const;

	//: Returns true if a scrollbar is visible in the listbox.
	bool has_scrollbar() const;

//! Operations:
public:
	//: Inserts text into the list at index. 
	//- If index is negative, text is inserted at the end of the list.
	//- Returns the index of the text
	int insert_item(const std::string &text, int index = -1);

	//: Removes and deletes the item at position index.
	//: If index is equal to get_current_item(), a new item gets highlighted and the sig_highlighted() signal is emitted. 
	void remove_item(int index);

	//: Replaces the item at position index with text. 
	void change_item(const std::string &text, int index);

	//: Sets the highlighted item to the item index.
	//: The highlighting is moved and the listbox scrolled as necessary. 
	void set_current_item(int index);

	//: Selects item at index if select is true, or unselects it if select is false.
	//: If the listbox is a single-selection listbox and and select is true, set_current_item() is called. 
	//: If the listbox is a single-selection listbox and and select is false, clear_selection()
	//: is called if index is the currently selected item. 
	void set_selected(int index, bool select);

	//: Deselects current selected item.
	void clear_selection();

	//: If select is true, all items get selected, else all get unselected.
	//: This works only in Multi selection mode.
	void select_all(bool select = true);

	//: Inverts the selection.
	//: Works only in Multi selection mode.
	void invert_selection();

	//: Sets the listbox to multiselection if enable is true,
	//: single selection otherwise.
	void set_multi_selection(bool enable = true);

	//: Sets the height of each item.
	void set_item_height(int height);

	//: Sets the number of items maximum visible in listbox (without scrolling)
	void set_max_visible_items(int count);

	//: Scrolls the list box so the item at position index in the list is displayed in the top row of the listbox. 
	void set_top_item(int index);

	//: Sorts the items in ascending order, if ascending is true, or descending otherwise. 
	void sort(bool ascending = true);

	//: Deletes all items in the list. 
	void clear();

//! Signals:
public:
	//: This signal is emitted when the user highlights a new current item.
	//: The argument is the index of the new item, which is already current. 
	CL_Signal_v1<int> &sig_highlighted();

	//: This signal is emitted when the user clicks on an item or presses return when an item is highlighted.
	//: The argument is the index of the selected item. 
	CL_Signal_v1<int> &sig_activated();

	//: This signal is emitted when the selection set of a listbox changes.
	CL_Signal_v0 &sig_selection_changed();

//! Implementation:
private:
	CL_ListBox(const CL_ListBox &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_ListBox_Generic *impl;
};

#endif
