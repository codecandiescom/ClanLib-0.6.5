/*
	$Id: font.cpp,v 1.9 2002/01/15 16:00:39 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Simple Font class.

*/

#include "Core/precomp.h"

#include <API/Display/Font/font.h>
#include <API/Core/Resources/resource_manager.h>
#include <API/Core/Resources/resource.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/System/error.h>

#include "font_bitmap.h"
#include "resource_font_generic.h"

CL_Font *CL_Font::create(CL_Font_Description *font_desc)
{
	return new CL_Font(font_desc);
}	

CL_Font *CL_Font::load(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	return new CL_Font(resource_id, manager);
}

CL_Font::CL_Font(CL_Font_Description *provider)
{
	impl = new CL_Font_Bitmap(provider);
	impl->add_reference();
}

CL_Font::CL_Font(
	const std::string &resource_id,
	CL_ResourceManager *manager)
{
	resource = manager->get_resource(resource_id);

	CL_ResourceData_Font *font_data =
		(CL_ResourceData_Font *) resource.get_data("font");
	cl_assert(font_data != NULL);

	resource.load();

	impl = font_data->font->impl;
	impl->add_reference();
}

CL_Font::CL_Font(class CL_Font_Generic *_impl) //: impl(_impl)
{
	impl = _impl;
	impl->add_reference();
}

CL_Font::CL_Font(const CL_Font &font)
{
	impl->add_reference();
}

CL_Font::~CL_Font()
{
	if (impl && impl->release_reference() == 0) delete impl;
	resource.unload();
}

CL_Font &CL_Font::operator =(const CL_Font &copy)
{
	resource.unload();
	if (impl && impl->release_reference() == 0) delete impl;

	resource = copy.resource;
	impl = copy.impl;

	resource.load();
	if (impl) impl->add_reference();

	return *this;
}

int CL_Font::get_height()
{
	return impl->get_height();
}

int CL_Font::get_text_width(const std::string &text)
{
	return impl->get_text_width(text);
}

int CL_Font::get_char_width(const char character)
{
	return impl->get_char_width(character);
}
	
void CL_Font::print_left(int x, int y, const std::string &text, int n_height)
{
	impl->print_left(x, y, text, n_height);
}

void CL_Font::print_left(int x, int y, float scale_x, float scale_y, const std::string &text)
{
	impl->print_left(x, y, scale_x, scale_y, text);
}
	
void CL_Font::print_center(int x, int y, const std::string &text, int n_height)
{
	impl->print_center(x, y, text, n_height);
}

void CL_Font::print_right(int x, int y, const std::string &text, int n_height)
{
	impl->print_right(x, y, text, n_height);
}
	
void CL_Font::put_target(int x, int y, const std::string &text, CL_Target *target, int alignment)
{
	impl->put_target(x, y, text, target, alignment);
}

int CL_Font::change_size(int size)
{
	return impl->change_size(size);
}

unsigned int CL_Font::change_colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return impl->change_colour(r, g, b, a);
}

