/*
	$Id: keyboard_fbdev.cpp,v 1.1 2001/03/06 15:09:20 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"

#ifndef USE_TTY_INPUT
#ifdef USE_SVGALIB
#define USE_TTY_INPUT
#else
#ifdef USE_FBDEV
#define USE_TTY_INPUT
#endif
#endif
#endif

#ifdef USE_TTY_INPUT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_KD_H
#include <sys/kd.h>
#else
#include <linux/kd.h>
#endif
#ifdef HAVE_SYS_VT_H
#include <sys/vt.h>
#else
#include <linux/vt.h>
#endif
#include <linux/tty.h>
#include <linux/keyboard.h>

#include "keyboard_fbdev.h"

#include <API/Core/System/error.h>
#include <Display/Input/FBDev/keyboard_fbdev.h>
#include <Core/System/Unix/implementation_fbdev.h>
#include <API/Display/Input/inputaxis.h>
#include <Display/Input/FBDev/keyboard_fbdev.h>
#include <API/Display/Input/inputbutton.h>
#include <Display/Input/FBDev/keyboard_fbdev.h>
#include <API/Display/Input/inputcursor.h>
#include <API/Display/Input/inputhat.h>

#include "Core/System/Generic/string_asm.h"

/******************
  Keyboard
******************/

CL_FBDevKeyboard::CL_FBDevKeyboard()
{
	memset( &keymap, 0, sizeof(keymap) );

	/* open the tty */
	fd = open("/dev/tty", O_RDWR | O_NONBLOCK);

	if (fd < 0)
	{
		throw CL_Error("Couldn't open /dev/tty.");
	}

	/* put tty into "straight through" mode. */
	struct termios newterm;

	if (tcgetattr(fd, &old_termios) < 0)
	{
		perror("tcgetattr failed");
	}

	memcpy ( &newterm, &old_termios, sizeof(termios) );

	newterm.c_lflag &= ~(ICANON | ECHO  | ISIG);
	newterm.c_iflag &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	newterm.c_iflag |= IGNBRK;
	newterm.c_cc[VMIN]  = 0;
	newterm.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSANOW, &newterm) < 0)
	{
		perror("tcsetattr failed");
	}

	/* save old mode and set to mediumraw */
	if (ioctl(fd, KDGKBMODE, &old_mode) < 0)
	{
		perror("Couldn't get keyboard mode");
		old_mode = K_XLATE;
	}
	if (ioctl(fd, KDSKBMODE, K_MEDIUMRAW) < 0)
	{
		perror("Couldn't set keyboard mode to K_MEDIUMRAW");
	}
	
	/* save old kd mode and set to graphics */
	if (ioctl(fd, KDGETMODE, &old_kd) < 0)
	{
		perror("Couldn't get kd mode");
		old_kd = KD_TEXT;
	}
	if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0)
	{
		perror("Couldn't set kd mode to KD_GRAPHICS");
	}
	
	buttons = new CL_InputButton_FBDevKeyboard*[CL_NUM_KEYS];
	for (int i=0; i<CL_NUM_KEYS; i++) buttons[i] = NULL;
}

CL_FBDevKeyboard::~CL_FBDevKeyboard()
{
	restore();
#ifdef USE_FBDEV
	CL_Implementation_FBDev::clean_up();
#endif

	for (int i=0; i<CL_NUM_KEYS; i++) delete buttons[i];
	delete[] buttons;
}

void CL_FBDevKeyboard::restore()
{
	if (fd>=0)
	{
		ioctl(fd, KDSKBMODE, old_mode);
		ioctl(fd, KDSETMODE, old_kd);
		if (tcsetattr(fd, TCSANOW, &old_termios) < 0)
		{
			cerr << "Could not restore old terminal input settings! Please run 'reset'!" << endl;
			perror( "System error message" );
		}
		close(fd);
	}
}

void CL_FBDevKeyboard::keep_alive()
{
	int readlen;
	unsigned char buf[256];
	
	// Read keyboard data
	while ((readlen = read(fd, buf, 256)) > 0)
	{
		for (int i = 0; i < readlen; i++)
		{
			handle_code( buf[i] );
		}
	}

	return;
}

void CL_FBDevKeyboard::handle_code(char code)
{
	bool keydown;
	kbentry entry;
	
	if (code & 0x80)
	{
		code &= 0x7f;
		keydown = false;
	} else
	{
		keydown = true;
	}
			
	//fetch the keycode
	entry.kb_table = 0;
	entry.kb_index = code;
	ioctl(fd,KDGKBENT,&entry);
	
	CL_Key key;
	key.id = translate(entry.kb_value);
	key.ascii = -1;
	key.state = keydown ? CL_Key::Pressed : CL_Key::Released;

	if (keydown) CL_Input::chain_button_press.on_button_press(this, key);
	else CL_Input::chain_button_release.on_button_release(this, key);
	
	keymap[translate(entry.kb_value)] = keydown;
}

char CL_FBDevKeyboard::translate(int kb_value)
{
	switch (kb_value)
	{
	case K_F1:	return CL_KEY_F1;
	case K_F2:	return CL_KEY_F2;
	case K_F3:	return CL_KEY_F3;
	case K_F4:	return CL_KEY_F4;
	case K_F5:	return CL_KEY_F5;
	case K_F6:	return CL_KEY_F6;
	case K_F7:	return CL_KEY_F7;
	case K_F8:	return CL_KEY_F8;
	case K_F9:	return CL_KEY_F9;
	case K_F10:	return CL_KEY_F10;
	case K_F11:	return CL_KEY_F11;
	case K_F12:	return CL_KEY_F12;
	
	case 2816+'a':	return CL_KEY_A;
	case 2816+'b':	return CL_KEY_B;
	case 2816+'c':	return CL_KEY_C;
	case 2816+'d':	return CL_KEY_D;
	case 2816+'e':	return CL_KEY_E;
	case 2816+'f':	return CL_KEY_F;
	case 2816+'g':	return CL_KEY_G;
	case 2816+'h':	return CL_KEY_H;
	case 2816+'i':	return CL_KEY_I;
	case 2816+'j':	return CL_KEY_J;
	case 2816+'k':	return CL_KEY_K;
	case 2816+'l':	return CL_KEY_L;
	case 2816+'m':	return CL_KEY_M;
	case 2816+'n':	return CL_KEY_N;
	case 2816+'o':	return CL_KEY_O;
	case 2816+'p':	return CL_KEY_P;
	case 2816+'q':	return CL_KEY_Q;
	case 2816+'r':	return CL_KEY_R;
	case 2816+'s':	return CL_KEY_S;
	case 2816+'t':	return CL_KEY_T;
	case 2816+'u':	return CL_KEY_U;
	case 2816+'v':	return CL_KEY_V;
	case 2816+'w':	return CL_KEY_W;
	case 2816+'x':	return CL_KEY_X;
	case 2816+'y':	return CL_KEY_Y;
	case 2816+'z':	return CL_KEY_Z;
	
	case 48:		return CL_KEY_0;
	case 49:		return CL_KEY_1;
	case 50:		return CL_KEY_2;
	case 51:		return CL_KEY_3;
	case 52:		return CL_KEY_4;
	case 53:		return CL_KEY_5;
	case 54:		return CL_KEY_6;
	case 55:		return CL_KEY_7;
	case 56:		return CL_KEY_8;
	case 57:		return CL_KEY_9;
	
	case 27:		return CL_KEY_ESCAPE;
	case K_LEFT:	return CL_KEY_LEFT;
	case K_RIGHT:	return CL_KEY_RIGHT;
	case K_UP:	return CL_KEY_UP;
	case K_DOWN:	return CL_KEY_DOWN;
	case K_ENTER:	return CL_KEY_ENTER;

	case K_CTRLL:	return CL_KEY_LCTRL;
	case K_CTRLR:	return CL_KEY_RCTRL;
	case K_SHIFTL:	return CL_KEY_LSHIFT;
	case K_SHIFTR:	return CL_KEY_RSHIFT;
	case K_ALT:	return CL_KEY_ALT;
	case K_ALTGR:	return CL_KEY_ALTGR;
	case 9:		return CL_KEY_TAB;
	case 32:		return CL_KEY_SPACE;
	case 127:		return CL_KEY_BACKSPACE;
	case K_INSERT:	return CL_KEY_INSERT;
	case K_REMOVE:	return CL_KEY_DELETE;
	case K_FIND:	return CL_KEY_HOME;
	case K_SELECT:	return CL_KEY_END;
	case K_PGUP:	return CL_KEY_PAGEUP;
	case K_PGDN:	return CL_KEY_PAGEDOWN;
	case K_CAPS:	return CL_KEY_CAPSLOCK;
	case K_NUM:	return CL_KEY_NUMLOCK;
	case K_HOLD:	return CL_KEY_SCRLOCK;
	case 28:		return CL_KEY_PRINT;
	case K(1,29):	return CL_KEY_PAUSE;
	case K_PSLASH:	return CL_KEY_KP_DIV;
	case K_PSTAR:	return CL_KEY_KP_MULT;
	case K_PMINUS:	return CL_KEY_KP_MINUS;
	case K_PPLUS:	return CL_KEY_KP_PLUS;
	case K_PENTER:	return CL_KEY_KP_ENTER;
	}
	
	return CL_KEY_NONE_OF_THE_ABOVE;
}

char *CL_FBDevKeyboard::get_name() const
{
	return "FBDev keyboard";
}

int CL_FBDevKeyboard::get_num_buttons() const
{
	return CL_NUM_KEYS;
}

CL_InputButton *CL_FBDevKeyboard::get_button(int button_num)
{
	if (button_num < 0 || button_num >= CL_NUM_KEYS) return NULL;

	if (buttons[button_num] == NULL)
		buttons[button_num] = new CL_InputButton_FBDevKeyboard(button_num, keymap);

	return buttons[button_num];
}

int CL_FBDevKeyboard::get_num_axes() const
{
	return 0;
}

CL_InputAxis *CL_FBDevKeyboard::get_axis(int /*axis_num*/)
{
	return NULL;
}

int CL_FBDevKeyboard::get_num_hats() const
{
	return 0;
}

CL_InputHat *CL_FBDevKeyboard::get_hat(int /*hat_num*/)
{
	return NULL;
}

int CL_FBDevKeyboard::get_num_cursors() const
{
	return 0;
}

CL_InputCursor *CL_FBDevKeyboard::get_cursor(int /*cursor_num*/)
{
	return NULL;
}

/***************************
  CL_InputButton_FBDevKeyboard
***************************/

CL_InputButton_FBDevKeyboard::CL_InputButton_FBDevKeyboard(
	int _key, char *_keymap)
{
	key = _key;
	keymap = _keymap;
}

bool CL_InputButton_FBDevKeyboard::is_pressed()
{
	return keymap[key];
}

#endif
