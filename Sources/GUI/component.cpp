/*
	$Id: component.cpp,v 1.78 2002/02/03 18:03:30 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Core/System/cl_assert.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/component_options.h"
#include "API/Display/Input/mouse.h"
#include "component_generic.h"
#include "component_manager_generic.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Component::CL_Component(
	CL_Component *parent,
	CL_StyleManager *style)
:
	impl(NULL)
{
	impl = new CL_Component_Generic(this, CL_Rect(0,0,0,0), parent, style);

	if (get_style_manager() != NULL)
		get_style_manager()->connect_styles("component", this);

	if (parent != NULL) parent->add_child(this);
}

CL_Component::CL_Component(const CL_Rect &pos, CL_Component *parent, CL_StyleManager *style)
: impl(NULL)
{
	impl = new CL_Component_Generic(this, pos, parent, style);

	if (get_style_manager() != NULL)
		get_style_manager()->connect_styles("component", this);

	if (parent != NULL) parent->add_child(this);
}

CL_Component::~CL_Component()
{
//	if (impl->parent != NULL) impl->parent->remove_child(this);
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_Component::get_parent() const
{
	return impl->parent;
}

CL_Component *CL_Component::get_root_parent()
{
	if(impl->parent)
		return impl->parent->get_root_parent();
	else
		return this;
}

CL_GUIManager *CL_Component::get_gui_manager() const
{
	if (impl->gui != NULL) return impl->gui;
	if (impl->parent != NULL) return impl->parent->get_gui_manager();
	return NULL;
}

CL_StyleManager *CL_Component::get_style_manager() const
{
	return impl->style_manager;
}

int CL_Component::get_width() const
{
	return impl->position.x2 - impl->position.x1;
}

int CL_Component::get_height() const
{
	return impl->position.y2 - impl->position.y1;
}

CL_Rect CL_Component::get_screen_rect() const
{
	if (impl->parent == NULL)
	{
		return impl->position;
	}
	else
	{
		CL_Rect r = impl->parent->get_screen_rect();

		return CL_Rect(
			r.x1 + impl->position.x1, 
			r.y1 + impl->position.y1,
			r.x1 + impl->position.x2, 
			r.y1 + impl->position.y2);
	}
}

const CL_Rect &CL_Component::get_position() const
{
	return impl->position;
}

bool CL_Component::has_child(CL_Component *component) const
{
	for (
		std::list<CL_Component *>::iterator it = impl->children.begin();
		it != impl->children.end();
		it++)
	{
		if (*it == component || (*it)->has_child(component)) return true;
	}

	return false;
}

const std::list<CL_Component *> &CL_Component::get_children() const
{
	return impl->children;
}

CL_Rect CL_Component::get_children_rect()
{
	CL_Rect rect(0, 0, 0, 0);
	
	for (
		std::list<CL_Component *>::iterator it = impl->children.begin();
		it != impl->children.end();
		it++)
	{
		CL_Component *comp = (*it);
		CL_Rect childrect = comp->get_position();

		if(childrect.x1 < rect.x1)
			rect.x1 = childrect.x1;
		if(childrect.y1 < rect.y1)
			rect.y1 = childrect.y1;
		if(childrect.x2 > rect.x2)
			rect.x2 = childrect.x2;
		if(childrect.y2 > rect.y2)
			rect.y2 = childrect.y2;
	}

	return rect;
}

CL_Component *CL_Component::get_component_at(int pos_x, int pos_y)
{
	std::list<CL_Component *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); it++)
	{
		CL_Component *cur_component = (*it);

		if(cur_component->is_visible())
		{
			const CL_Rect &r = cur_component->get_position();

			if (pos_x >= r.x1 && pos_x < r.x2 &&
				pos_y >= r.y1 && pos_y < r.y2)
			{
				return cur_component->get_component_at(pos_x - r.x1, pos_y - r.y1);
			}
		}
	}

	return this;
}

CL_Component *CL_Component::get_focus() const
{
	return get_gui_manager()->get_focus();
}

bool CL_Component::has_focus() const
{
	return get_focus() == this;
}

bool CL_Component::has_mouse_over() const
{
	int x = CL_Mouse::get_x();
	int y = CL_Mouse::get_y();

	CL_Rect rect = get_screen_rect();

	return rect.inside(x, y);
}

bool CL_Component::is_visible() const
{
	return impl->visible;
}

bool CL_Component::is_enabled() const
{
	return impl->enabled;
}

bool CL_Component::is_popup() const
{
	return impl->popup;
}

bool CL_Component::is_mouse_captured() const
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		return gui->get_capture() == this;
	}
	return false;
}

bool CL_Component::is_removeflag_set() const
{
	return impl->removeflag;
}

CL_Point CL_Component::get_mouse_position() const
{
	CL_Point pos(CL_Mouse::get_x(), CL_Mouse::get_y());
	CL_Rect screen = get_screen_rect();
	pos.x -= screen.x1;
	pos.y -= screen.y1;
	return pos;
}

int CL_Component::get_tab_id() const
{
	return impl->tab_id;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Component::set_tab_id(int id)
{
	impl->tab_id = id;
}

void CL_Component::set_position(const CL_Rect &new_pos)
{
	int old_x = impl->position.x1;
	int old_y = impl->position.y1;

	int old_width = get_width();
	int old_height = get_height();

	impl->position = new_pos;

	if(old_x != impl->position.x1 || old_y != impl->position.y1)
		impl->sig_move(old_x, old_y);
	if(old_width != get_width() || old_height != get_height())
		impl->sig_resize(old_width, old_height);
}

void CL_Component::set_position(int new_x, int new_y)
{
	int old_x = impl->position.x1;
	int old_y = impl->position.y1;

	impl->position.x2 = get_width() + new_x;
	impl->position.y2 = get_height() + new_y;
	impl->position.x1 = new_x;
	impl->position.y1 = new_y;

	if(old_x != impl->position.x1 || old_y != impl->position.y1)
		impl->sig_move(old_x, old_y);
}

void CL_Component::set_size(int new_width, int new_height)
{
	int old_width = get_width();
	int old_height = get_height();

	impl->position.x2 = impl->position.x1 + new_width;
	impl->position.y2 = impl->position.y1 + new_height;

	if(old_width != get_width() || old_height != get_height())
		impl->sig_resize(old_width, old_height);
}

void CL_Component::set_width(int width)
{
	int old_width = get_width();

	impl->position.x2 = impl->position.x1 + width;

	if(old_width != get_width())
		impl->sig_resize(old_width, get_height());
}

void CL_Component::set_height(int height)
{
	int old_height = get_height();

	impl->position.y2 = impl->position.y1 + height;

	if(old_height != get_height())
		impl->sig_resize(get_width(), old_height);
}

void CL_Component::set_parent(CL_Component *parent)
{
	impl->parent = parent;
}

void CL_Component::set_gui_manager(CL_GUIManager *gui)
{
	impl->gui = gui;
}

void CL_Component::show(bool show)
{
	impl->visible = show;
}

void CL_Component::enable(bool enable)
{
	impl->enabled = enable;
}

void CL_Component::popup(bool popup)
{
	impl->popup = popup;
}

void CL_Component::set_focus()
{
	get_gui_manager()->set_focus(this);
}

void CL_Component::raise()
{
	impl->parent->impl->children.remove(this);
	impl->parent->impl->children.push_front(this);
}

void CL_Component::lower()
{
	impl->parent->impl->children.remove(this);
	impl->parent->impl->children.push_back(this);
}

void CL_Component::capture_mouse()
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		gui->gui_capture_mouse(this);
	}
}

void CL_Component::release_mouse()
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		gui->gui_release_mouse();
	}
}

void CL_Component::update()
{
	impl->sig_begin_paint();
	impl->sig_paint();
	impl->sig_paint_children();
	impl->sig_end_paint();
}

void CL_Component::run(CL_Component *parent)
{
	if(get_parent())
		std::cout << "When creating modal components, don't create them with a parent!" << std::endl;

	CL_GUIManager modal_gui(parent, get_style_manager());
	modal_gui.add_child(this);
	modal_gui.run();
}

void CL_Component::quit()
{
	get_gui_manager()->quit();
}

void CL_Component::close()
{
//	show(false);
//	impl->removeflag = true;
	if(get_gui_manager() != NULL)
		get_gui_manager()->quit();
	else
		std::cout << "gui manager == NULL" << std::endl;

	if(get_parent() != NULL)
		get_parent()->remove_child(this);
	else
		std::cout << "parent == NULL" << std::endl;
//	set_gui_manager(NULL);
}

void CL_Component::clear_removeflag()
{
	impl->removeflag = false;
}

void CL_Component::add_child(CL_Component *child, bool delete_child)
{
	if(has_child(child))
		return;

	impl->sig_child_before_add(child);

	impl->children.push_front(child);
	child->set_parent(this);

	if (delete_child) impl->delete_children.push_front(child);

	impl->sig_child_add(child);
}

void CL_Component::remove_child(CL_Component *child)
{
	impl->sig_child_before_remove(child);

	child->set_parent(NULL);
	impl->children.remove(child);

	impl->sig_child_remove(child);
}

void CL_Component::add_components(CL_ComponentManager *component_manager)
{
	std::vector<CL_Component *> components;
	((CL_ComponentManager_Generic *) component_manager)->get_root_component_list(components);

	for (
		std::vector<CL_Component *>::iterator it = components.begin();
		it != components.end();
		it++)
	{
		add_child(*it);
	}
}

void CL_Component::attach_style(CL_ComponentStyle *style)
{
	impl->styles.push_back(style);
}

void CL_Component::detach_style(CL_ComponentStyle *style)
{
	impl->styles.remove(style);
}

void CL_Component::find_preferred_size()
{
	impl->find_preferred_size();
}

CL_Component *CL_Component::find_tab_id(int tab_id)
{
	if (impl->tab_id == tab_id) return this;

	std::list<CL_Component *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); it++)
	{
		CL_Component *comp = (*it)->find_tab_id(tab_id);
		if (comp) return comp;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const CL_ComponentOptions &> &CL_Component::sig_set_options()
{
	return impl->sig_set_options;
}

CL_Signal_v0 &CL_Component::sig_paint()
{
	return impl->sig_paint;
}

CL_Signal_v0 &CL_Component::sig_paint_children()
{
	return impl->sig_paint_children;
}

CL_Signal_v0 &CL_Component::sig_begin_paint()
{
	return impl->sig_begin_paint;
}

CL_Signal_v0 &CL_Component::sig_end_paint()
{
	return impl->sig_end_paint;
}

CL_Signal_v2<CL_InputDevice *, const CL_Key &> &CL_Component::sig_input_down()
{
	return impl->sig_input_down;
}

CL_Signal_v2<CL_InputDevice *, const CL_Key &> &CL_Component::sig_input_up()
{
	return impl->sig_input_up;
}

CL_Signal_v1<const CL_Key &> &CL_Component::sig_key_down()
{
	return impl->sig_key_down;
}

CL_Signal_v1<const CL_Key &> &CL_Component::sig_key_up()
{
	return impl->sig_key_up;
}

CL_Signal_v1<const CL_Key &> &CL_Component::sig_mouse_down()
{
	return impl->sig_mouse_down;
}

CL_Signal_v1<const CL_Key &> &CL_Component::sig_mouse_up()
{
	return impl->sig_mouse_up;
}

CL_Signal_v2<int, int> &CL_Component::sig_mouse_move()
{
	return impl->sig_mouse_move;
}

CL_Signal_v0 &CL_Component::sig_mouse_enter()
{
	return impl->sig_mouse_enter;
}

CL_Signal_v0 &CL_Component::sig_mouse_leave()
{
	return impl->sig_mouse_leave;
}

CL_Signal_v0 &CL_Component::sig_got_focus()
{
	return impl->sig_got_focus;
}

CL_Signal_v0 &CL_Component::sig_lost_focus()
{
	return impl->sig_lost_focus;
}

CL_Signal_v2<int, int> &CL_Component::sig_resize()
{
	return impl->sig_resize;
}

CL_Signal_v2<int, int> &CL_Component::sig_move()
{
	return impl->sig_move;
}

CL_Signal_v2<int &, int &> &CL_Component::sig_transform_coords()
{
	return impl->sig_transform_coords;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_add()
{
	return impl->sig_child_add;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_remove()
{
	return impl->sig_child_remove;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_before_add()
{
	return impl->sig_child_before_add;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_before_remove()
{
	return impl->sig_child_before_remove;
}

CL_Signal_v1<CL_Point &> &CL_Component::sig_get_minimum_size()
{
	return impl->sig_get_minimum_size;
}

CL_Signal_v1<CL_Point &> &CL_Component::sig_get_maximum_size()
{
	return impl->sig_get_maximum_size;
}

CL_Signal_v1<CL_Point &> &CL_Component::sig_get_preferred_size()
{
	return impl->sig_get_preferred_size;
}
