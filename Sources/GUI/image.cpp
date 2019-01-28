/*
	$Id: image.cpp,v 1.6 2002/03/01 22:19:09 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/GUI/image.h"
#include "API/GUI/component_options.h"
#include "API/GUI/stylemanager.h"
#include "image_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Image::CL_Image(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::CL_Image(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this);
	get_style_manager()->connect_styles("image", this);
}

CL_Image::CL_Image(
	const CL_Rect &pos,
	CL_Surface *surface,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this);
	get_style_manager()->connect_styles("image", this);
}

CL_Image::CL_Image(
	CL_Surface *surface,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), impl(NULL)
{
	impl = new CL_Image_Generic(this);
	get_style_manager()->connect_styles("image", this);
	find_preferred_size();
}

CL_Image::~CL_Image()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Surface *CL_Image::get_surface() const
{
	return impl->surface;
}

CL_Image::Mode CL_Image::get_mode() const
{
	return impl->mode;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Image::set_surface(CL_Surface *surface)
{
	impl->surface = surface;
}

void CL_Image::set_mode(Mode mode)
{
	impl->mode = mode;
}
