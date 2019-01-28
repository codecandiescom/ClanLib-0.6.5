/*
	$Id: mouse_ggi.cpp,v 1.6 2002/01/23 11:47:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <ggi/ggi.h>

#include "mouse_ggi.h"

#include <API/Display/Input/input.h>
#include <API/Display/Input/mouse.h>
#include <API/Display/Input/key.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbutton.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>
#include <API/Core/System/cl_assert.h>
#include <Display/Display/GGI/display_ggi.h>
#include <Display/Input/GGI/mouse_ggi.h>

/*************************************
  CL_Mouse_GGI
*************************************/

CL_Mouse_GGI::CL_Mouse_GGI(CL_GGI_DisplayCard *_card)
{
	card = _card;
	m_vis = card->vis;

	cursor = new CL_InputCursor_Mouse_GGI(card);
	axes = new CL_InputAxis_Mouse_GGI[2];
	
	buttons = new CL_InputButton_Mouse_GGI*[3];
	buttons[0] = new CL_InputButton_Mouse_GGI();
	buttons[1] = new CL_InputButton_Mouse_GGI();
	buttons[2] = new CL_InputButton_Mouse_GGI();
}

CL_Mouse_GGI::~CL_Mouse_GGI()
{
	delete cursor;

	for (int i=0; i<3; i++) delete buttons[i];
	delete[] buttons;
}

CL_InputDevice::InputDeviceType CL_Mouse_GGI::get_type() const
{
	return CL_InputDevice::type_mouse;
}

int CL_Mouse_GGI::get_num_buttons() const
{
	return 3;
}

CL_InputButton *CL_Mouse_GGI::get_button(int button_num)
{
	if (button_num >= 3 || button_num < 0) return NULL;
	
	if (buttons[button_num] == NULL)
		buttons[button_num] = new CL_InputButton_Mouse_GGI();

	return buttons[button_num];
}

int CL_Mouse_GGI::get_num_axes() const
{
	return 2;
}

CL_InputAxis *CL_Mouse_GGI::get_axis(int axis_num)
{
	cl_assert(((unsigned int) axis_num)<2);
	return &axes[axis_num];
}

int CL_Mouse_GGI::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_Mouse_GGI::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_Mouse_GGI::get_num_cursors() const
{
	return 1;
}

void CL_Mouse_GGI::keep_alive()
{
	ggi_event_mask	mask;
	ggi_event		event;
	struct timeval	tv = {0,0};

	mask = ggiEventPoll( m_vis, emPointer, &tv );
	
	axes[0].center = card->get_width()/2.0;
	axes[1].center = card->get_height()/2.0;
	
	while (mask)
	{
		ggiEventRead( m_vis, &event, emPointer );
	
		switch (event.any.type)
		{
			case evPtrButtonPress:
				buttons[event.pbutton.button-1]->button_state = true;
				CL_Input::sig_button_press()(
					this, 
					CL_Key(event.pbutton.button-1, CL_Key::Pressed, -1, cursor->x, cursor->y));
				break;
			case evPtrButtonRelease:
				buttons[event.pbutton.button-1]->button_state = false;
				CL_Input::sig_button_release()(
					this, 
					CL_Key(event.pbutton.button-1, CL_Key::Released, -1, cursor->x, cursor->y));
				break;
			case evPtrAbsolute:
				cursor->x = event.pmove.x ;
				cursor->y = event.pmove.y;
				if (cursor->x<0) cursor->x = 0;
				if (cursor->y<0) cursor->y = 0;
				if (cursor->x>cursor->get_max_x()) cursor->x = cursor->get_max_x();
				if (cursor->y>cursor->get_max_y()) cursor->y = cursor->get_max_y();
				axes[0].pos = cursor->x;
				axes[1].pos = cursor->y;
				CL_Input::sig_mouse_move()(this, cursor->x, cursor->y);
				CL_Mouse::sig_move()(cursor->x, cursor->y);
				break;
			default:
				break;
		}
		
		mask = ggiEventPoll( m_vis, emPointer, &tv );
	}
}

CL_InputCursor *CL_Mouse_GGI::get_cursor(int /*cursor_num*/)
{
	return cursor;
}

/*******************************
  CL_InputButton_Mouse_GGI
*******************************/

CL_InputButton_Mouse_GGI::CL_InputButton_Mouse_GGI()
{
	button_state = false;
}

CL_InputButton_Mouse_GGI::~CL_InputButton_Mouse_GGI()
{
}

bool CL_InputButton_Mouse_GGI::is_pressed()
{
	return button_state;
}

/*******************************
  CL_InputCursor_Mouse_GGI
*******************************/

CL_InputCursor_Mouse_GGI::CL_InputCursor_Mouse_GGI(
	CL_GGI_DisplayCard *_card)
{
	card = _card;
	x = 0;
	y = 0;
}

CL_InputCursor_Mouse_GGI::~CL_InputCursor_Mouse_GGI()
{
}


float CL_InputCursor_Mouse_GGI::get_x()
{
	return x;
}

float CL_InputCursor_Mouse_GGI::get_y()
{
	return y;
}

float CL_InputCursor_Mouse_GGI::get_max_x()
{
	return card->get_width();
}

float CL_InputCursor_Mouse_GGI::get_max_y()
{
	return card->get_height();
}

/*******************************
  CL_InputAxis_Mouse_GGI
*******************************/

CL_InputAxis_Mouse_GGI::CL_InputAxis_Mouse_GGI()
{
	pos = 0;
	center = 1;
}

CL_InputAxis_Mouse_GGI::~CL_InputAxis_Mouse_GGI()
{
}


float CL_InputAxis_Mouse_GGI::get_pos()
{
	return (pos - center) / center;
}

#endif
