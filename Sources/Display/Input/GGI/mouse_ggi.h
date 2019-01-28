/*
	$Id: mouse_ggi.h,v 1.2 2001/11/30 22:38:40 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mouse_ggi
#define header_mouse_ggi

#ifdef USE_GGI

#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/inputcursor.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Core/System/keep_alive.h"
#include "Display/Display/GGI/display_ggi.h"
#include "Core/System/Unix/init_linux.h"

class CL_InputButton_Mouse_GGI : public CL_InputButton
{
public:
	bool button_state;

	CL_InputButton_Mouse_GGI();
	virtual ~CL_InputButton_Mouse_GGI();

	virtual bool is_pressed();
};

class CL_InputCursor_Mouse_GGI : public CL_InputCursor
{
friend class CL_Mouse_GGI;

protected:
	CL_GGI_DisplayCard *card;
	float x, y;
public:

	CL_InputCursor_Mouse_GGI(CL_GGI_DisplayCard *card);
	virtual ~CL_InputCursor_Mouse_GGI();

	virtual float get_x();
	virtual float get_y();
	virtual float get_max_x();
	virtual float get_max_y();
};

class CL_InputAxis_Mouse_GGI : public CL_InputAxis
{
friend class CL_Mouse_GGI;

protected:
	float pos, center;
public:
	
	CL_InputAxis_Mouse_GGI();
	~CL_InputAxis_Mouse_GGI();

	virtual float get_pos();
};

class CL_Mouse_GGI : public CL_InputDevice, CL_KeepAlive
{
protected:
	CL_GGI_DisplayCard *card;
	CL_InputButton_Mouse_GGI **buttons;
	CL_InputCursor_Mouse_GGI *cursor;
	CL_InputAxis_Mouse_GGI *axes;

	ggi_visual_t m_vis;

public:
	CL_Mouse_GGI(CL_GGI_DisplayCard *card);
	virtual ~CL_Mouse_GGI();

	virtual char *get_name() const { return "ggiMouse"; };

	virtual InputDeviceType get_type() const;

	virtual int get_num_buttons() const;
	virtual CL_InputButton *get_button(int button_num);

	virtual int get_num_axes() const;
	virtual CL_InputAxis *get_axis(int axis_num);

	virtual int get_num_hats() const;
	virtual CL_InputHat *get_hat(int hat_num);

	virtual int get_num_cursors() const;
	virtual CL_InputCursor *get_cursor(int cursor_num);

	virtual void keep_alive();
};

#endif /* USE_GGI */

#endif
