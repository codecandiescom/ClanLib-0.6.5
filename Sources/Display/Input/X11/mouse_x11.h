/*
	$Id: mouse_x11.h,v 1.3 2002/05/17 08:47:59 grumbel Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_mouse_x11
#define header_mouse_x11

#ifdef USE_X11

#include "API/Display/Input/inputdevice.h"
#include "API/Display/Input/inputbutton.h"
#include "API/Display/Input/inputaxis.h"
#include "API/Display/Input/inputcursor.h"
#include "Display/Display/X11/display_xwindow.h"
#include "Core/System/Unix/init_linux.h"
#include "API/Core/System/keep_alive.h"

class CL_InputButton_Mouse_XWin : public CL_InputButton
{
public:
	bool button_state;

	CL_InputButton_Mouse_XWin();
	virtual ~CL_InputButton_Mouse_XWin();

	virtual bool is_pressed();
};

class CL_InputCursor_Mouse_XWin : public CL_InputCursor
{
friend class CL_Mouse_XWin;

protected:
	CL_XWindow_CompatibleCard *card;
	float x, y;

public:

	CL_InputCursor_Mouse_XWin(CL_XWindow_CompatibleCard *card);
	virtual ~CL_InputCursor_Mouse_XWin();

	virtual float get_x();
	virtual float get_y();
	virtual float get_max_x();
	virtual float get_max_y();
	virtual void  set_position (float x, float y);
};

class CL_InputAxis_Mouse_XWin : public CL_InputAxis
{
friend class CL_Mouse_XWin;

protected:
	float pos, center;

public:
	CL_InputAxis_Mouse_XWin();
	virtual ~CL_InputAxis_Mouse_XWin();

	virtual float get_pos();
};

class CL_Mouse_XWin : public CL_InputDevice, CL_KeepAlive
{
protected:
	bool have_focus;
	CL_XWindow_CompatibleCard *card;
	CL_InputButton_Mouse_XWin **buttons;
	CL_InputCursor_Mouse_XWin *cursor;
	CL_InputAxis_Mouse_XWin *axes;

public:
	CL_Mouse_XWin(CL_XWindow_CompatibleCard *card);
	virtual ~CL_Mouse_XWin();

	virtual char *get_name() const { return "XMouse"; };

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

#endif /* USE_X11 */

#endif
