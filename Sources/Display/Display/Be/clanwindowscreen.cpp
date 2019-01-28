/*
	$Id: clanwindowscreen.cpp,v 1.1 2001/03/06 15:09:17 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <Display/Input/Be/keyboard_be.h>
#include <API/Core/System/error.h>
#include <API/Display/Input/input.h>
#include <API/Core/System/system.h>
#include "Core/System/Generic/string_asm.h"

#include "clanwindowscreen.h"

#include <Screen.h>
#include <AppDefs.h>

ClanWindowScreen::ClanWindowScreen(status_t* ret) : BWindowScreen("ClanWindowScreen",B_16_BIT_640x480,ret)
{
	if (*ret < B_OK)
	{
		throw CL_Error("Could not create BWindowScreen");
	}
	
	if (!CanControlFrameBuffer())
	{
		throw CL_Error("We have no control over the framebuffer");
	}
	
	save_buffer = NULL;
	connected = false;
}

void ClanWindowScreen::ScreenConnected(bool is_connected)
{
	connected = is_connected;
	
	if (connected)
	{
		uint32 space = find_space();
		if (!space)
		{
			throw CL_Error("No valid mode found");
		}
		if (SetSpace(space)<B_OK)
		{
			throw CL_Error("Could not set mode");
		}

		frames = 2;
		if (SetFrameBuffer(width,height*frames)<B_OK)
		{
			throw CL_Error("Could not set framebuffer dimensions");
		}

		frame_buffer = (uint8*)(CardInfo()->frame_buffer);
		line_length = FrameBufferInfo()->bytes_per_row;
		write_frame = 1;
		
		sync=(sync_hook)CardHookAt(7);
		if (sync)
		{
//			std::cout << "ClanWindowScreen: Using sync after clear_dislay" << std::endl;
		}

		init_accelerations();

		memset( frame_buffer, 0, line_length*height*frames );

		if (save_buffer)
		{
			fast_memmove( frame_buffer, save_buffer, line_length*height*frames );
		} else
		{
			save_buffer = new uint8[line_length*height*frames];
		}
	} else
	{
		CL_System::suspend_time();
		fast_memmove( save_buffer, frame_buffer, line_length*height*frames );
	}
}

void ClanWindowScreen::init_accelerations()
{
	fill_rect8 = NULL;
	fill_rect16 = NULL;
	fill_rect32 = NULL;
	acc_fill_rect = false;
	switch (bpp)
	{
		case 8:
			fill_rect8=(rect8_hook)CardHookAt(5);
			if (fill_rect8)
			{
				std::cout << "ClanWindowScreen: Using accelerated fill_rect" << std::endl;
				acc_fill_rect = true;
			}
			break;
		case 16:
			fill_rect16=(rect16_hook)CardHookAt(13);
			if (fill_rect16)
			{
				std::cout << "ClanWindowScreen: Using accelerated fill_rect" << std::endl;
				acc_fill_rect = true;
			}
			break;
		case 32:
			fill_rect32=(rect32_hook)CardHookAt(6);
			if (fill_rect32)
			{
				std::cout << "ClanWindowScreen: Using accelerated fill_rect" << std::endl;
				acc_fill_rect = true;
			}
			break;
	}
}

bool ClanWindowScreen::fill_rect_accelerated()
{
	return acc_fill_rect;
}

void ClanWindowScreen::set_videomode(int _width, int _height, int _bpp, bool _fullscreen, bool _allow_resize, bool _video_memory)
{
	width = _width;
	height = _height;
	bpp = _bpp;
	fullscreen = _fullscreen;
	allow_resize = _allow_resize;
	
	Show();
}

void ClanWindowScreen::flip_display()
{
	if (connected)
	{
		MoveDisplayArea(0,height*write_frame);
		BScreen(this).WaitForRetrace();
		write_frame = !write_frame;
	} else
	{
		while(!connected)
		{
			snooze(5000);
		}
		CL_System::resume_time();
		write_frame = !write_frame;
	}
}

void ClanWindowScreen::put_display(const CL_Rect &rect)
{
	// this is not implemented! Someone with BeOS knowledge, please do so. :)
	// mbn -- 21. nov 2000
	cl_assert(false); 
}

uint8* ClanWindowScreen::get_data()
{
	if (!connected)
	{
		return save_buffer + line_length*height*write_frame;
	}
	return frame_buffer + line_length*height*write_frame;
}

uint32 ClanWindowScreen::find_space()
{
	if ((width==640) && (height==480) && (bpp==8))
		return B_8_BIT_640x480;
	if ((width==640) && (height==480) && (bpp==16))
		return B_16_BIT_640x480;
	if ((width==640) && (height==480) && (bpp==32))
		return B_32_BIT_640x480;
	
	if ((width==800) && (height==600) && (bpp==8))
		return B_8_BIT_800x600;
	if ((width==800) && (height==600) && (bpp==16))
		return B_16_BIT_800x600;
	if ((width==800) && (height==600) && (bpp==32))
		return B_32_BIT_800x600;

	if ((width==1024) && (height==768) && (bpp==8))
		return B_8_BIT_1024x768;
	if ((width==1024) && (height==768) && (bpp==16))
		return B_16_BIT_1024x768;
	if ((width==1024) && (height==768) && (bpp==32))
		return B_32_BIT_1024x768;

	return 0;
}

void ClanWindowScreen::MessageReceived(BMessage *message)
{
	switch ( message->what )
	{
		case B_KEY_DOWN:
			((CL_BeKeyboard*)(CL_Input::keyboards[0]))->handle_key( message, true );
			break;
		case B_KEY_UP:
			((CL_BeKeyboard*)(CL_Input::keyboards[0]))->handle_key( message, false );
			break;
		case B_MODIFIERS_CHANGED:
			((CL_BeKeyboard*)(CL_Input::keyboards[0]))->handle_modifiers( message );
			break;
		default:
			BWindowScreen::MessageReceived(message);
			break;
	}
}
