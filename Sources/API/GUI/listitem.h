/*
	$Id: listitem.h,v 1.1 2002/03/01 19:36:54 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=listbox_item.h

#ifndef header_listbox_item
#define header_listbox_item

#include <string>

class CL_ListItem
{
public:
	CL_ListItem() : selected(false) {}

	CL_ListItem(std::string str, bool selected = false)
	{
		this->str = str;
		this->selected = selected;
	}

	bool operator< ( const CL_ListItem &item ) const
	{
		return str < item.str;
	}

	bool operator== ( const CL_ListItem &item ) const
	{
		return str == item.str;
	}

	std::string str;
	bool selected;
};

#endif
