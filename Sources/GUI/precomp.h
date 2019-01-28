#ifndef header_precomp_gui
#define header_precomp_gui

#ifdef WIN32
#pragma warning (disable:4786)

// Framework
#include "API/GUI/component.h"
#include "API/GUI/component_manager.h"
#include "API/GUI/component_type.h"
#include "API/GUI/component_move_handler.h"
#include "API/GUI/component_resize_handler.h"
#include "API/GUI/component_style.h"
#include "API/GUI/component_options.h"
#include "API/GUI/gui_file_parser.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/layout_manager.h"
#include "API/GUI/setupgui.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/stylemanager_default.h"

// Components
#include "API/GUI/button.h"
#include "API/GUI/checkbox.h"
#include "API/GUI/frame.h"
#include "API/GUI/image.h"
#include "API/GUI/label.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/listbox.h"
#include "API/GUI/listitem.h"
#include "API/GUI/menunode.h"
#include "API/GUI/popupmenu.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/radiobutton.h"
#include "API/GUI/radiogroup.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/treeview.h"
#include "API/GUI/treeitem.h"
#include "API/GUI/treenode.h"
#include "API/GUI/window.h"

// Dialogs
#include "API/GUI/messagebox.h"
#include "API/GUI/filedialog.h"

#endif

#endif
