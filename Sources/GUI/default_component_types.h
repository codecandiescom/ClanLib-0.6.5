/*
	$Id: default_component_types.h,v 1.35 2002/03/01 19:43:11 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "../API/GUI/component_type.h"

#include "API/GUI/label.h"
#include "API/GUI/button.h"
#include "API/GUI/frame.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/image.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/progressbar.h"
#include "API/GUI/checkbox.h"
#include "API/GUI/listbox.h"
#include "API/GUI/window.h"
#include "API/GUI/filedialog.h"

class CL_ComponentType_Component : public CL_ComponentType
{
public:
	CL_ComponentType_Component(bool _is_container) : CL_ComponentType(_is_container)
	{
		options["x"].type = CL_ComponentType::SOptionType::NUMBER;
		options["y"].type = CL_ComponentType::SOptionType::NUMBER;
		options["width"].type = CL_ComponentType::SOptionType::NUMBER;
		options["height"].type = CL_ComponentType::SOptionType::NUMBER;
		options["visible"].type = CL_ComponentType::SOptionType::BOOL;
		options["enabled"].type = CL_ComponentType::SOptionType::BOOL;
	}
};

class CL_ComponentType_Button : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Button() : CL_ComponentType_Component(false)
	{
		options["text"].type = CL_ComponentType::SOptionType::STRING;;
		options["togglemode"].type = CL_ComponentType::SOptionType::NUMBER;
		options["surface_up"].type = CL_ComponentType::SOptionType::STRING;
		options["surface_down"].type = CL_ComponentType::SOptionType::STRING;
		options["surface_disabled"].type = CL_ComponentType::SOptionType::STRING;
		options["surface_highlighted"].type = CL_ComponentType::SOptionType::STRING;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Button(parent, style);
	}
};

class CL_ComponentType_CheckBox : public CL_ComponentType_Component
{
public:
	CL_ComponentType_CheckBox() : CL_ComponentType_Component(false)
	{
		options["text"].type = CL_ComponentType::SOptionType::STRING;
		options["togglemode"].type = CL_ComponentType::SOptionType::NUMBER;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_CheckBox(parent, style);
	}
};

class CL_ComponentType_FileDialog : public CL_ComponentType_Component
{
public:
	CL_ComponentType_FileDialog() : CL_ComponentType_Component(true)
	{
		options["file"].type = CL_ComponentType::SOptionType::STRING;
		options["filter"].type = CL_ComponentType::SOptionType::STRING;
		options["showhidden"].type = CL_ComponentType::SOptionType::BOOL;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_FileDialog(parent, style);
	}
};

class CL_ComponentType_Frame : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Frame() : CL_ComponentType_Component(true)
	{
		options["surface"].type = CL_ComponentType::SOptionType::STRING;
		options["mode"].type = CL_ComponentType::SOptionType::NUMBER;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Frame(parent, style);
	}
};

class CL_ComponentType_Image : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Image() : CL_ComponentType_Component(false)
	{
		// no specific options for image
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Image(parent, style);
	}
};

class CL_ComponentType_InputBox : public CL_ComponentType_Component
{
public:
	CL_ComponentType_InputBox() : CL_ComponentType_Component(false)
	{
		options["passwordmode"].type = CL_ComponentType::SOptionType::NUMBER;
		options["readonly"].type = CL_ComponentType::SOptionType::BOOL;
		options["maxlength"].type = CL_ComponentType::SOptionType::NUMBER;
		options["text"].type = CL_ComponentType::SOptionType::STRING;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_InputBox(parent, style);
	}
};

class CL_ComponentType_Label : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Label() : CL_ComponentType_Component(false)
	{
		options["text"].type = CL_ComponentType::SOptionType::STRING;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Label(parent, style);
	}
};

class CL_ComponentType_ListBox : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ListBox() : CL_ComponentType_Component(false)
	{
		options["item"].type = CL_ComponentType::SOptionType::NUMBER;
		options["max_visible_items"].type = CL_ComponentType::SOptionType::NUMBER;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ListBox(parent, style);
	}
};

class CL_ComponentType_ProgressBar : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ProgressBar() : CL_ComponentType_Component(false)
	{
		options["steps"].type = CL_ComponentType::SOptionType::NUMBER;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ProgressBar(parent, style);
	}
};

class CL_ComponentType_ScrollBar : public CL_ComponentType_Component
{
public:
	CL_ComponentType_ScrollBar() : CL_ComponentType_Component(false)
	{
		options["min"].type = CL_ComponentType::SOptionType::NUMBER;
		options["max"].type = CL_ComponentType::SOptionType::NUMBER;
		options["value"].type = CL_ComponentType::SOptionType::NUMBER;
		options["orientation"].type = CL_ComponentType::SOptionType::NUMBER;
		options["tracking"].type = CL_ComponentType::SOptionType::NUMBER;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_ScrollBar(parent, style);
	}
};

class CL_ComponentType_Window : public CL_ComponentType_Component
{
public:
	CL_ComponentType_Window() : CL_ComponentType_Component(true)
	{
		options["title"].type = CL_ComponentType::SOptionType::STRING;
	}

	virtual CL_Component *create_component(
		CL_Component *parent,
		CL_StyleManager *style)
	{
		return new CL_Window(parent, style);
	}
};

// TODO:
// Add missing component-types
