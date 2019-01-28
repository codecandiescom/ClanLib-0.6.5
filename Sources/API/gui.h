/*
	$Id: gui.h,v 1.40 2002/03/01 22:19:08 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>Graphical User Interface API. Provides a themeable and very flexible GUI
//: framework.</p>
//! Global=GUI

#ifndef header_gui
#define header_gui

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

// Framework
#include "GUI/component.h"
#include "GUI/component_manager.h"
#include "GUI/component_type.h"
#include "GUI/component_move_handler.h"
#include "GUI/component_resize_handler.h"
#include "GUI/component_style.h"
#include "GUI/component_options.h"
#include "GUI/gui_file_parser.h"
#include "GUI/gui_manager.h"
#include "GUI/layout_manager.h"
#include "GUI/setupgui.h"
#include "GUI/stylemanager.h"
#include "GUI/stylemanager_default.h"

// Components
#include "GUI/button.h"
#include "GUI/checkbox.h"
#include "GUI/frame.h"
#include "GUI/image.h"
#include "GUI/inputbox.h"
#include "GUI/label.h"
#include "GUI/listbox.h"
#include "GUI/listitem.h"
#include "GUI/menunode.h"
#include "GUI/popupmenu.h"
#include "GUI/progressbar.h"
#include "GUI/radiobutton.h"
#include "GUI/radiogroup.h"
#include "GUI/scrollbar.h"
#include "GUI/treeview.h"
#include "GUI/treeitem.h"
#include "GUI/treenode.h"
#include "GUI/window.h"

// Dialogs
#include "GUI/messagebox.h"
#include "GUI/filedialog.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanGUI.lib")
#else
#pragma comment(lib, "clanGUId.lib")
#endif
#endif


#endif
