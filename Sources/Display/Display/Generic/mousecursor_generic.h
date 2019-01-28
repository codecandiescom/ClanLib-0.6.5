/*
	$Id: mousecursor_generic.h,v 1.4 2001/09/08 19:12:48 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_mousecursor_generic
#define header_mousecursor_generic

#include "displaycard_generic.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Display/Display/mousecursor.h"
#include "API/Display/Display/surface.h"
#include "mousecursor_provider_generic.h"

class CL_MouseCursor_Generic 
	: public CL_DisplayCard_Generic::CL_FlipDisplayCallback_Generic
	, public CL_Runnable
{
public:
	CL_MouseCursor_Generic();
	virtual ~CL_MouseCursor_Generic();

	void set_cursor(
		CL_MouseCursorProvider *provider,
		int frame=0);

	void show(bool show_async=true);
	void hide();

	// CL_Runnable overrides
	virtual void run();

	// CL_DisplayCard_Generic::CL_FlipDisplayCallback_Generic overrides
	virtual void pre_flip();
	virtual void post_flip();

private:
	friend class CL_MouseCursor;

	void invalidate_region();
	void save_region(CL_Target *source, const CL_ClipRect &r);
	void load_region(CL_Target *dest);
	void show_cursor(CL_Target *dest, bool load_saved);

	CL_ClipRect m_rect;
	unsigned char *m_buffer;

	bool volatile m_visible;
	bool volatile m_async;
	int volatile m_last_x;
	int volatile m_last_y;
	bool m_callback_added;

	CL_MouseCursorProvider *m_provider;
	CL_Surface *m_cursor;
	unsigned int m_cur_frame;
	unsigned int m_frame_timer;

	CL_Thread *m_thread;
	CL_Mutex *m_mux;

	static CL_MouseCursor_Generic m_instance;
};

#endif

