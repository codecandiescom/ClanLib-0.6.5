/*
	$Id: mouse_be.cpp,v 1.1 2001/03/06 15:09:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifdef USE_GGI
/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
*/
#include <ggi/ggi.h>

#include <Display/Display/GGI/display_ggi.h>
#include <API/Display/Input/inputaxis.h>
#include <API/Display/Input/inputbuffer.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/GGI/mouse_ggi.h>
#include <API/Display/Input/inputcursor.h>
#include <Display/Input/GGI/mouse_ggi.h>
#include <API/Display/Input/inputhat.h>
#include <Display/Input/GGI/mouse_ggi.h>
#include <Core/System/Be/app_beos.h>

/*************************************
  CL_Mouse_GGI
*************************************/

CL_Mouse_GGI::CL_Mouse_GGI(CL_GGI_DisplayCard *_card)
{
	card = _card;
	m_vis = card->vis;

	cursor = new CL_InputCursor_Mouse_GGI(card);
	buttons.add(new CL_InputButton_Mouse_GGI(),0);
	buttons.add(new CL_InputButton_Mouse_GGI(),1);
	buttons.add(new CL_InputButton_Mouse_GGI(),2);

	CL_System_Generic::keep_alives.add(this);
}

CL_Mouse_GGI::~CL_Mouse_GGI()
{
	delete cursor;

	int num_buttons = buttons.get_num_items();
	for (int i=0; i<num_buttons; i++) delete buttons[i];

	CL_System_Generic::keep_alives.del(this);
}


int CL_Mouse_GGI::get_num_buttons() const
{
	return 3;
}

CL_InputButton *CL_Mouse_GGI::get_button(int button_num)
{
	if (buttons[button_num] == NULL)
		buttons.add(new CL_InputButton_Mouse_GGI(),button_num);
	return buttons[button_num];
}

int CL_Mouse_GGI::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_Mouse_GGI::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_Mouse_GGI::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_Mouse_GGI::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_Mouse_GGI::get_num_buffers() const
{
	return 0;
}

CL_InputBuffer *CL_Mouse_GGI::get_buffer(int /*buffer_num*/)
{
	return NULL;
}

int CL_Mouse_GGI::get_num_cursors() const
{
	return 1;
}

bool CL_Mouse_GGI::keep_alive()
{
	ggi_event_mask	mask;
	ggi_event		event;
	struct timeval	tv = {0,0};

	mask = ggiEventPoll( m_vis, emPointer, &tv );
	
	while (mask)
	{
		ggiEventRead( m_vis, &event, emPointer );
	
		switch (event.any.type)
		{
			case evPtrButtonPress:
				buttons[event.pbutton.button-1]->button_state = true;
				break;
			case evPtrButtonRelease:
				buttons[event.pbutton.button-1]->button_state = false;
				break;
			case evPtrAbsolute:
				cursor->x = event.pmove.x ;
				cursor->y = event.pmove.y;
				if (cursor->x<0) cursor->x = 0;
				if (cursor->y<0) cursor->y = 0;
				if (cursor->x>cursor->get_max_x()) cursor->x = cursor->get_max_x();
				if (cursor->y>cursor->get_max_y()) cursor->y = cursor->get_max_y();
				break;
			default:
				break;
		}
		
		mask = ggiEventPoll( m_vis, emPointer, &tv );
	}

	return false;
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

#endif
