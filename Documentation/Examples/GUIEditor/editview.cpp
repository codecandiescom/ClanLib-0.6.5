
#ifdef WIN32
#pragma warning(disable : 4355)
#pragma warning(disable : 4786)
#endif

#include "editview.h"
#include "mainframe.h"
#include "component_node.h"
#include <ClanLib/GUI/button.h>
#include <ClanLib/GUI/component_type.h>
#include <ClanLib/display.h>

/////////////////////////////////////////////////////////////////////////////
// EditView construction:

EditView::EditView(MainFrame *mainframe)
: CL_Component(mainframe), root(0), mode(mode_select),
	mouse_capture_mode(mousemove_none), resize_grabber(0), mainframe(mainframe)
{
	create_simulation();

	Document &doc = get_document();
	slots.connect(doc.sig_node_added, this, &EditView::on_node_added);
	slots.connect(doc.sig_node_removed, this, &EditView::on_node_removed);
	slots.connect(doc.sig_node_options_changed, this, &EditView::on_node_options_changed);
	slots.connect(doc.sig_node_type_changed, this, &EditView::on_node_type_changed);
	slots.connect(sig_resize(), this, &EditView::on_resize);
	slots.connect(sig_paint(), this, &EditView::on_paint);
	slots.connect(sig_mouse_down(), this, &EditView::on_mouse_down);
	slots.connect(sig_mouse_up(), this, &EditView::on_mouse_up);
	slots.connect(sig_mouse_move(), this, &EditView::on_mouse_move);
	slots.connect(mainframe->sig_selection_changed, this, &EditView::on_selection_changed);

	on_resize(get_width(), get_height());
}

EditView::~EditView()
{
	delete root;
}

/////////////////////////////////////////////////////////////////////////////
// EditView attributes:

Document &EditView::get_document()
{
	return mainframe->document;
}

/////////////////////////////////////////////////////////////////////////////
// EditView operations:

void EditView::set_mode(EditView::Mode new_mode)
{
	mode = new_mode;
}

/////////////////////////////////////////////////////////////////////////////
// EditView implementation:

void EditView::create_simulation()
{
	delete root;
	root = new CL_GUIManager(get_style_manager());
	root->disable_input();

	Document &doc = get_document();
	std::list<ComponentNode *>::iterator it;
	for (it = doc.nodes.begin(); it != doc.nodes.end(); it++)
	{
		on_node_added(*it);
	}
}

void EditView::on_paint()
{
	CL_Display::fill_rect(0, 0, get_width(), get_height(), 0.6f,0.6f,0.6f);

	// All this shit (except root->show()) should not be needed, but it
	// seems to be some kind of bug in clanDisplay, and frankly, I prefer
	// a hack than to try and fix clanDisp... -mbn 4th Jan 2002
	CL_Display::push_clip_rect();
	CL_Display::set_clip_rect(CL_ClipRect(0,0,CL_Display::get_width(),CL_Display::get_height()));
	int tx = CL_Display::get_translate_offset_x();
	int ty = CL_Display::get_translate_offset_y();
	CL_Display::set_translate_offset(0, 0);
	root->show();
	CL_Display::set_translate_offset(tx, ty);
	CL_Display::pop_clip_rect();

	// Draw component selection:
	std::list<ComponentNode *>::iterator it;
	for (it = selection.begin(); it != selection.end(); it++)
	{
		ComponentNode *cur_selection = *it;

		CL_Component *cur_component = components[cur_selection];
		CL_Rect rect = get_rect(cur_component);
		CL_Display::draw_rect(rect.x1-1, rect.y1-1, rect.x2+1, rect.y2+1, 0.3f, 0.3f, 1.0f);

		// Dont draw grabbers while moving the gui.
		if (mouse_capture_mode == mousemove_move) continue;

		std::vector<CL_Rect> grabbers = get_grab_rects(cur_component);
		for (int i=0; i<8; i++)
		{
			CL_Display::fill_rect(
				grabbers[i].x1, grabbers[i].y1, grabbers[i].x2, grabbers[i].y2,
				0.4f, 0.4f, 1.0f);
		}
	}

	// Draw selection gfx:
	CL_Point mpos = get_mouse_position();
	CL_Point delta(mpos.x - mouse_anker.x, mpos.y - mouse_anker.y);
	switch (mouse_capture_mode)
	{
	case mousemove_select_net:
		CL_Display::draw_rect(mouse_anker.x, mouse_anker.y, mpos.x, mpos.y, 1.0f, 0.4f, 0.4f, 0.7f);
		break;

	case mousemove_resize:
		break;

	case mousemove_move:
		for (it = selection.begin(); it != selection.end(); it++)
		{
			ComponentNode *cur_selection = *it;
			CL_Component *cur_component = components[cur_selection];
			CL_Rect rect = get_rect(cur_component);
			rect.move(delta.x, delta.y);
			CL_Display::draw_rect(rect.x1-1, rect.y1-1, rect.x2+1, rect.y2+1, 0.8f, 0.8f, 1.0f, 0.5f);
		}
		break;
	}

}

void EditView::on_resize(int old_width, int old_height)
{
	// Position GUI simulation where we are.
	
	// root->set_position(CL_Rect(4, 4, get_width()-4, get_height()-4));

	CL_Rect screen_rect = get_screen_rect();
	int scr_x = screen_rect.x1+4;
	int scr_y = screen_rect.y1+4;
	int width = screen_rect.get_width()-8;
	int height = screen_rect.get_height()-8;
	root->set_position(CL_Rect(scr_x, scr_y, scr_x+width, scr_y+height));
}

void EditView::on_node_added(ComponentNode *node)
{
	CL_Component *parent = root;
	if (node->get_parent() != 0)
	{
		std::map<ComponentNode *, CL_Component *>::iterator it_comp;
		it_comp = components.find(node->get_parent());
		if (it_comp != components.end()) parent = it_comp->second;
	}

	CL_ComponentType *comp_type = node->find_component_type();
	CL_Component *component = comp_type->create_component(parent, root->get_style_manager());
	component->sig_set_options().call(node->get_options());
	components[node] = component;

	std::list<ComponentNode *>::iterator it;
	for (it = node->nodes.begin(); it != node->nodes.end(); it++)
	{
		on_node_added(*it);
	}
}

void EditView::on_node_removed(ComponentNode *node)
{
	// todo: find node in simulation and remove it.
	// todo: remove node from selection, if its in it.
}

void EditView::on_node_options_changed(ComponentNode *node)
{
	CL_Component *component = components[node];
	component->sig_set_options()(node->get_options());
}

void EditView::on_node_type_changed(ComponentNode *node)
{
	// todo: find node in simulation, remove it
	// todo: create new node (eg by calling on_node_added).
}

void EditView::on_selection_changed(std::list<ComponentNode *> &new_selection)
{
	selection = new_selection;
}

void EditView::on_mouse_down(const CL_Key &key)
{
	mouse_anker = CL_Point(key.x, key.y);

	std::list<ComponentNode *>::iterator it;

	switch (mode)
	{
	case mode_select:
		for (it = selection.begin(); it != selection.end(); it++)
		{
			ComponentNode *cur_selection = *it;
			CL_Component *cur_component = components[cur_selection];

			// Try and see if click was on a grabber:
			bool gribber = false;
			std::vector<CL_Rect> grabbers = get_grab_rects(cur_component);
			for (int i=0; i<8; i++)
			{
				if (grabbers[i].inside(key.x, key.y))
				{
					mouse_capture_mode = mousemove_resize;
					resize_grabber = i;
					gribber = true;
					break;
				}
			}
			if (gribber) break;

			// Clicked one of the selected components - switch to move mode:
			CL_Rect rect = get_rect(cur_component);
			if (rect.inside(key.x, key.y))
			{
				mouse_capture_mode = mousemove_move;
			}
		}
		if (mouse_capture_mode == mousemove_none)
		{
			// Since it was none of the above, it has to be
			// a new net selection:
			mouse_capture_mode = mousemove_select_net;

			// Clear current selection:
			std::list<ComponentNode *> empty_list;
			mainframe->sig_selection_changed(empty_list);
		}
		break;

	case mode_new:
		break;
	}

	if (mouse_capture_mode != mousemove_none) capture_mouse();
}

void EditView::on_mouse_up(const CL_Key &key)
{
	if (mouse_capture_mode == mousemove_none) return;

	switch (mouse_capture_mode)
	{
	case mousemove_select_net:
		if (mouse_anker.x == key.x && mouse_anker.y == key.y)
		{
			// Check if component was clicked:
			CL_Component *comp = root->get_component_at(key.x - 4, key.y - 4);

			std::map<ComponentNode *, CL_Component *>::iterator it;
			for (it = components.begin(); it != components.end(); it++)
			{
				if (it->second == comp)
				{
					selection.push_back(it->first);
					break;
				}
			}

			mainframe->sig_selection_changed(selection);
		}
		else
		{
			CL_Rect selection_rect(mouse_anker.x, mouse_anker.y, key.x, key.y);
			selection_rect.normalize();

			std::map<ComponentNode *, CL_Component *>::iterator it;
			for (it = components.begin(); it != components.end(); it++)
			{
				CL_Rect comp_rect = get_rect(it->second);
				if (
					selection_rect.inside(comp_rect.x1, comp_rect.y1) &&
					selection_rect.inside(comp_rect.x2, comp_rect.y2))
				{
					selection.push_back(it->first);
				}
			}

			mainframe->sig_selection_changed(selection);
		}
		break;

	case mousemove_resize:
		break;

	case mousemove_move:
		{
			int delta_x = key.x - mouse_anker.x;
			int delta_y = key.y - mouse_anker.y;

			std::list<ComponentNode *>::iterator it;
			for (it = selection.begin(); it != selection.end(); it++)
			{
				ComponentNode *node = *it;

				CL_ComponentOptions options = node->get_options();
				int x = options.get_value_as_int("x") + delta_x;
				int y = options.get_value_as_int("y") + delta_y;
				options.remove_option("x");
				options.remove_option("y");
				options.add_option("x", x);
				options.add_option("y", y);

				node->set_options(options);
			}
		}
		break;
	}

	release_mouse();
	mouse_capture_mode = mousemove_none;
}

void EditView::on_mouse_move(int x, int y)
{
	switch (mouse_capture_mode)
	{
	case mousemove_select_net:
		break;

	case mousemove_resize:
		break;

	case mousemove_move:
		break;
	}
}

std::vector<CL_Rect> EditView::get_grab_rects(CL_Component *component)
{
	std::vector<CL_Rect> rects;

	CL_Rect rect = get_rect(component);
	int cx = rect.x1 + rect.get_width()/2;
	int cy = rect.y1 + rect.get_height()/2;

	rects.push_back(CL_Rect(rect.x1-2, rect.y1-2, rect.x1+2, rect.y1+2));
	rects.push_back(CL_Rect(rect.x1-2, rect.y2-2, rect.x1+2, rect.y2+2));
	rects.push_back(CL_Rect(rect.x2-2, rect.y1-2, rect.x2+2, rect.y1+2));
	rects.push_back(CL_Rect(rect.x2-2, rect.y2-2, rect.x2+2, rect.y2+2));
	rects.push_back(CL_Rect(cx-2, rect.y1-2, cx+2, rect.y1+2));
	rects.push_back(CL_Rect(cx-2, rect.y2-2, cx+2, rect.y2+2));
	rects.push_back(CL_Rect(rect.x1-2, cy-2, rect.x1+2, cy+2));
	rects.push_back(CL_Rect(rect.x2-2, cy-2, rect.x2+2, cy+2));

	return rects;
}

CL_Rect EditView::get_rect(CL_Component *component)
{
	CL_Rect rect = component->get_screen_rect();
	// Adjust rect from screen position to editview position:
	rect.move(-root->get_position().x1, -root->get_position().y1);
	rect.move(4, 4); // we got a 4 pixel border.
	return rect;
}
