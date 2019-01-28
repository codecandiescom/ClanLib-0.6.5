/*
	$Id: mouse_x11.cpp,v 1.11 2002/05/17 08:47:58 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_X11

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "API/Core/System/cl_assert.h"

#include "mouse_x11.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/mouse.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/key.h>
#include <API/Display/Input/inputhat.h>
#include <Display/Input/X11/mouse_x11.h>
#include <Display/Display/X11/display_xwindow.h>

#define NUM_MOUSE_BUTTONS 3

/*************************************
  CL_Mouse_XWin
*************************************/

CL_Mouse_XWin::CL_Mouse_XWin(CL_XWindow_CompatibleCard *_card)
{
	card = _card;

	cursor = new CL_InputCursor_Mouse_XWin(card);
	axes = new CL_InputAxis_Mouse_XWin[2]; 

	buttons = new CL_InputButton_Mouse_XWin*[NUM_MOUSE_BUTTONS];
	for (int i=0; i<NUM_MOUSE_BUTTONS; i++)
		buttons[i] = new CL_InputButton_Mouse_XWin();
	have_focus = false;
}

CL_Mouse_XWin::~CL_Mouse_XWin()
{
	delete cursor;
	delete[] axes;

	for (int i=0; i<NUM_MOUSE_BUTTONS; i++) delete buttons[i];
	delete[] buttons;
}

CL_InputDevice::InputDeviceType CL_Mouse_XWin::get_type() const
{
	return CL_InputDevice::type_mouse;
}

int CL_Mouse_XWin::get_num_buttons() const
{
	return 3;
}

CL_InputButton *CL_Mouse_XWin::get_button(int button_num)
{
	if (button_num < 0 || button_num >= NUM_MOUSE_BUTTONS) return NULL;

	return buttons[button_num];
}

int CL_Mouse_XWin::get_num_axes() const
{
	return 2;
}

CL_InputAxis *CL_Mouse_XWin::get_axis(int axis_num)
{
	cl_assert(((unsigned int) axis_num)<2);
	return &axes[axis_num];
}

int CL_Mouse_XWin::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_Mouse_XWin::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_Mouse_XWin::get_num_cursors() const
{
	return 1;
}

void CL_Mouse_XWin::keep_alive()
{
	if (card->is_initialized() == false) return;

	CL_Key key;
	Window child;
	Window root;
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	unsigned int mask;

	axes[0].center = (card->get_width())/2.0;
	axes[1].center = (card->get_height())/2.0;

	// Check if window has focus right now:
	Window focus_win;
	int focus_state;
	XGetInputFocus(
		card->get_display(),
		&focus_win,
		&focus_state);

	XQueryPointer(card->get_display(),
		      card->get_window(),
		      &root,
		      &child,
		      &root_x,
		      &root_y,
		      &win_x,
		      &win_y,
		      &mask);

	// Cutting the mouse position to the effective window size
	if (win_x < 0) win_x = 0;
	if (win_y < 0) win_y = 0;
	
	if (win_x >= cursor->get_max_x()) win_x = (int)cursor->get_max_x() - 1;
	if (win_y >= cursor->get_max_y()) win_y = (int)cursor->get_max_y() - 1;

	// get_window() returns NULL if in DGA mode.	
	if (card->get_window() == 0 || card->get_window() == focus_win)
	{
		axes[0].pos = cursor->x;
		axes[1].pos = cursor->y;
		
		if (cursor->x != win_x || cursor->y != win_y) 
		{
			//the mouse is on our display:
			cursor->x = win_x;
			cursor->y = win_y;
			
			CL_Input::sig_mouse_move()(this, (int)cursor->x, (int)cursor->y);
			CL_Mouse::sig_move()((int)cursor->x, (int)cursor->y);
		}
		
		// Handle the key events
		if (mask & 0x100)
		{
			if (buttons[0]->button_state == false)
			{
				buttons[0]->button_state = true;
				CL_Input::sig_button_press()(
					this,
					CL_Key(
						CL_MOUSE_LEFTBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_press()(
					CL_Key(
						CL_MOUSE_LEFTBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
			}
		}
		else
		{
			if (buttons[0]->button_state == true)
			{
				buttons[0]->button_state = false;
				CL_Input::sig_button_release()(
					this,
					CL_Key(
						CL_MOUSE_LEFTBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_release()(
					CL_Key(
						CL_MOUSE_LEFTBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
			}
		}
		
		if (mask & 0x200)
		{
			if (buttons[1]->button_state == false)
			{
				buttons[1]->button_state = true;
				CL_Input::sig_button_press()(
					this,
					CL_Key(
						CL_MOUSE_MIDDLEBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_press()(
					CL_Key(
						CL_MOUSE_MIDDLEBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
			}
		}
		else
		{
			if (buttons[1]->button_state == true)
			{
				buttons[1]->button_state = false;
				CL_Input::sig_button_release()(
					this,
					CL_Key(
						CL_MOUSE_MIDDLEBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_release()(
					CL_Key(
						CL_MOUSE_MIDDLEBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
			}
		}

		if(mask & 0x400) 
		{
			if (buttons[2]->button_state == false)
			{
				buttons[2]->button_state = true;
				CL_Input::sig_button_press()(
					this,
					CL_Key(
						CL_MOUSE_RIGHTBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_press()(
					CL_Key(
						CL_MOUSE_RIGHTBUTTON,
						CL_Key::Pressed,
						-1,
						cursor->x,
						cursor->y));
			}
		}
		else
		{
			if (buttons[2]->button_state == true)
			{
				buttons[2]->button_state = false;
				CL_Input::sig_button_release()(
					this,
					CL_Key(
						CL_MOUSE_RIGHTBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
				CL_Mouse::sig_button_release()(
					CL_Key(
						CL_MOUSE_RIGHTBUTTON,
						CL_Key::Released,
						-1,
						cursor->x,
						cursor->y));
			}
		}

		if (!have_focus)
		{
			have_focus = true;
		}
	}
	else // we don't have window focus. keep last coordinates and release buttons
	{
		XQueryPointer(card->get_display(),
			      card->get_window(),
			      &root,
			      &child,
			      &root_x,
			      &root_y,
			      &win_x,
			      &win_y,
			      &mask);

		cursor->x = win_x;
		cursor->y = win_y;
	
		if (have_focus)
		{
			have_focus = false;
//			CL_Input::sig_mouse_exit(this);
		}
		
		// Handle the key events
		key.state = CL_Key::Released;     

		if (buttons[0]->button_state == true) 
		{
			key.id = CL_MOUSE_LEFTBUTTON;
			buttons[0]->button_state = false;
			CL_Input::sig_button_release()(this, key);
			CL_Mouse::sig_button_release()(key);
		}
	  
		if (buttons[1]->button_state == true)
		{
			key.id = CL_MOUSE_MIDDLEBUTTON;
			buttons[1]->button_state = false;
			CL_Input::sig_button_release()(this, key);
			CL_Mouse::sig_button_release()(key);
		}

		if (buttons[2]->button_state == true)
		{
			key.id = CL_MOUSE_RIGHTBUTTON;
			buttons[2]->button_state = false;
			CL_Input::sig_button_release()(this, key);
			CL_Mouse::sig_button_release()(key);
		}	  
	}
	
	if (card->is_fullscreen())
	{
		int w = card->get_win_width();
		int h = card->get_win_height();

		cursor->x -= (w - card->get_width())/2;
		cursor->y -= (h - card->get_height())/2;

		if (cursor->x < 0)
			cursor->x = 0;
		if (cursor->x > card->get_width())
			cursor->x = card->get_width();
		if (cursor->y < 0)
			cursor->y = 0;
		if (cursor->y > card->get_height())
			cursor->y = card->get_height();
	}
}

CL_InputCursor *CL_Mouse_XWin::get_cursor(int /*cursor_num*/)
{
	return cursor;
}

/*******************************
  CL_InputButton_Mouse_XWin
*******************************/

CL_InputButton_Mouse_XWin::CL_InputButton_Mouse_XWin()
{
	button_state = false;
}

CL_InputButton_Mouse_XWin::~CL_InputButton_Mouse_XWin()
{
}

bool CL_InputButton_Mouse_XWin::is_pressed()
{
	return button_state;
}

/*******************************
  CL_InputCursor_Mouse_XWin
*******************************/

CL_InputCursor_Mouse_XWin::CL_InputCursor_Mouse_XWin(
	CL_XWindow_CompatibleCard *_card)
{
	card = _card;
	x = 0;
	y = 0;
}

CL_InputCursor_Mouse_XWin::~CL_InputCursor_Mouse_XWin()
{
}


float CL_InputCursor_Mouse_XWin::get_x()
{
	return x;
}

float CL_InputCursor_Mouse_XWin::get_y()
{
	return y;
}

void CL_InputCursor_Mouse_XWin::set_position (float arg_x, float arg_y)
{
	// We set the values here, since they are updated only after
	// the next keep_alive()
	x = arg_x;
	y = arg_y;

	XWarpPointer(card->get_display(), 
		     card->get_window(),
		     card->get_window(),
		     0, 0, 
		     0, 0,
		     int(x), int(y));
}

float CL_InputCursor_Mouse_XWin::get_max_x()
{
	return card->get_width();
}

float CL_InputCursor_Mouse_XWin::get_max_y()
{
	return card->get_height();
}

/*******************************
  CL_InputAxis_Mouse_XWin
*******************************/

CL_InputAxis_Mouse_XWin::CL_InputAxis_Mouse_XWin()
{
	center = 1;
	pos = 0;
}

CL_InputAxis_Mouse_XWin::~CL_InputAxis_Mouse_XWin()
{
}


float CL_InputAxis_Mouse_XWin::get_pos()
{
	return (pos - center) / center;
}

#endif
