/*
	$Id: component_options.h,v 1.38 2002/02/20 07:47:11 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_styleoptions
#define header_styleoptions

#include "../Core/System/clanstring.h"
#include "../Core/System/error.h"
#include <map>
#include <string>
#include <stdio.h>

#ifdef WIN32
#define snprintf _snprintf
#endif

//: Sets the options for components
class CL_ComponentOptions
{
public:
//! Attributes:
	typedef std::multimap<std::string, std::string> options_t;
	typedef options_t::iterator iterator;
	typedef options_t::const_iterator const_iterator;

	//: Exists
	bool exists(const std::string &option_name) const
	{
		CL_String lc_name(option_name); lc_name.to_lower();
		return options.find(lc_name) != options.end();
	}

	//: Count
	int count(const std::string &option_name) const
	{
		CL_String lc_name(option_name); lc_name.to_lower();
		return (int)options.count(lc_name);
	}

	void remove_option(const std::string &option_name)
	{
		CL_String lc_name(option_name); lc_name.to_lower();
		std::multimap<std::string, std::string>::iterator it = options.find(lc_name);
		while (it != options.end())
		{
			if (it->first != option_name) break;
			std::multimap<std::string, std::string>::iterator it2 = it;
			++it;
			options.erase(it2);
		}
	}

	//: Getting the value of a unexisting option is undefined, so use the 'exists' function
	//: first to check if the option exists
	const std::string &get_value(const std::string &option_name, int offset=0) const
	{ 
		CL_String lc_name(option_name); lc_name.to_lower();
		std::multimap<std::string, std::string>::const_iterator it = options.find(lc_name);
		if (it == options.end())
		{
			char buf[100];
			snprintf(buf, 99, "Missing component option '%s'", option_name.c_str());
			buf[99] = 0;
			cl_throw_error(buf);
		}
		for (;offset>0;offset--)
		{
			it++;
			if (it == options.end())
			{
				char buf[100];
				snprintf(buf, 99, "Missing component option '%s'", option_name.c_str());
				buf[99] = 0;
				cl_throw_error(buf);
			}
		}
		return (*it).second; 
	}

	const std::string &operator[](const std::string &option_name) const
	{
		return get_value(option_name, 0);
	}

	//: Get value as int
	int get_value_as_int(const std::string &option_name, int offset=0) const
	{
		const std::string &val = get_value(option_name, offset);
		return atoi(val.c_str()); 
	}

	//: Get value as bool
	bool get_value_as_bool(const std::string &option_name, int offset=0) const
	{
		const std::string &val = get_value(option_name, offset);
		CL_String s(val);
		s.to_lower();

		if(s == "yes" ||
			s == "true" ||
			s == "on" ||
			s == "1")
			return true;
		else if(s == "no" ||
			s == "false" ||
			s == "off" ||
			s == "0")
			return false;

		char buf[200];
		snprintf(
			buf, 199,
			"Invalid bool value '%s' in component option '%s'", 
			val.c_str(),
			option_name.c_str());
		buf[199] = 0;
		cl_throw_error(buf);
		return false;
	}
	
//! Operations:
	//: Add option
	void add_option(const std::string &option_name, const std::string &option_value)
	{
		CL_String lc_name(option_name); lc_name.to_lower();
		options.insert(options_t::value_type(lc_name, option_value));
	}
	
	//: Add option
	void add_option(const std::string &option_name, int option_value)
	{
		CL_String value; value << option_value;
		add_option(option_name, value);
	}
	
	//: Clear
	void clear()
	{
		options.clear();
	}

	iterator begin() { return options.begin(); }
	iterator end() { return options.end(); }

	const_iterator begin() const { return options.begin(); }
	const_iterator end() const { return options.end(); }

	//: Options
	options_t options;
};

#endif
