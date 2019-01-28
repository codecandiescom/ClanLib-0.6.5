/*
	$Id: component_generic.h,v 1.30 2002/02/03 18:03:30 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_component_generic
#define header_component_generic

#include "API/GUI/component.h"

class CL_Component_Generic
{
public:
	CL_Component_Generic(
		CL_Component *self,
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style);

	~CL_Component_Generic();

	void set_options(const CL_ComponentOptions &options);
	void find_preferred_size();

	void begin_paint();
	void paint_children();
	void end_paint();

	CL_Component *owner;
	CL_Component *parent;

	CL_StyleManager *style_manager;

	CL_Rect position;

	CL_GUIManager *gui;

	bool visible;
	bool enabled;
	bool popup;
	bool removeflag;

	int tab_id;
	
	std::list<CL_Component *> children;
	std::list<CL_Component *> delete_children;
	std::list<CL_ComponentStyle *> styles;

	CL_Signal_v1<const CL_ComponentOptions &> sig_set_options;
	CL_Signal_v0 sig_begin_paint;
	CL_Signal_v0 sig_paint;
	CL_Signal_v0 sig_end_paint;
	CL_Signal_v0 sig_paint_children;
	CL_Signal_v2<CL_InputDevice *, const CL_Key &> sig_input_down;
	CL_Signal_v2<CL_InputDevice *, const CL_Key &> sig_input_up;
	CL_Signal_v1<const CL_Key &> sig_key_down;
	CL_Signal_v1<const CL_Key &> sig_key_up;
	CL_Signal_v1<const CL_Key &> sig_mouse_down;
	CL_Signal_v1<const CL_Key &> sig_mouse_up;
	CL_Signal_v2<int, int> sig_mouse_move;
	CL_Signal_v0 sig_mouse_enter;
	CL_Signal_v0 sig_mouse_leave;
	CL_Signal_v0 sig_got_focus;
	CL_Signal_v0 sig_lost_focus;
	CL_Signal_v2<int, int> sig_resize;
	CL_Signal_v2<int, int> sig_move;
	CL_Signal_v2<int &, int &> sig_transform_coords;
	CL_Signal_v1<CL_Component *> sig_child_add;
	CL_Signal_v1<CL_Component *> sig_child_remove;
	CL_Signal_v1<CL_Component *> sig_child_before_add;
	CL_Signal_v1<CL_Component *> sig_child_before_remove;
	CL_Signal_v1<CL_Point &> sig_get_minimum_size;
	CL_Signal_v1<CL_Point &> sig_get_maximum_size;
	CL_Signal_v1<CL_Point &> sig_get_preferred_size;

	CL_Slot slot_set_options;
	CL_Slot slot_begin_paint;
	CL_Slot slot_paint_children;
	CL_Slot slot_end_paint;
};

#endif
