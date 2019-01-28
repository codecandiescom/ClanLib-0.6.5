/*
	$Id: resourceoption.h,v 1.13 2002/02/25 21:23:17 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="Resources"
//! header=core.h

#ifndef header_resourceoption
#define header_resourceoption

#include <vector>
#include <string>

//: Resource Option Class
//- Holds the particular options for a resource
//- Used by the ResourceManager and DataFile compiler
class CL_ResourceOption
{
//! Construction:
public:
	//: Resource Option Constructor
  //- name - The name of the option
  //- value - Value of the option as a string
	CL_ResourceOption(const std::string &name, const std::string &value) : name(name)
	{
	  num_vals = 1;
		values.push_back(value);
	}

	//: Resource Option Constructor
	//- name - The name of the option.
	CL_ResourceOption(const std::string &name) : name(name)
	{
	  num_vals = 0;
	}

	//: Default Resource Option Constructor
	CL_ResourceOption()
	{
	  num_vals = 0;
	}

	CL_ResourceOption(const CL_ResourceOption &other)
	{
		num_vals = other.num_vals;
		name = other.name;
		if(num_vals)
		{
			std::vector<std::string>::const_iterator it;
			for(it = other.values.begin(); it != other.values.end(); ++it)
			{
				values.push_back(*it);
			}
		}
	}

//! Attributes:
public:
	//: Get Name
	//- Returns - The name of the option
	const std::string &get_name() const { return name; }

	//: Get Value
	//- i - The index of which value to return, default of 0
	//- Returns - The option for the index value of i.
	const std::string &get_value(int i=0) const { return values[i]; }

	//: Get Values
	//- Returns - An std::vector<std::string> with all the values.
	std::vector<std::string> &get_values() { return values; }

//! Operations:
public:
	void operator =( const CL_ResourceOption &other)
	{
		num_vals = other.num_vals;
		name = other.name;
		if(num_vals)
		{
			std::vector<std::string>::const_iterator it;
			for(it = other.values.begin(); it != other.values.end(); ++it)
			{
				values.push_back(*it);
			}
		}
	}

	//: Add a Value
	//- value - the string value to add
	//- Returns - the index of the added value
	int add_value(const std::string &value) { values.push_back(value); return num_vals++; }

//! Implementation:
private:
	int num_vals;
	std::string name;
	std::vector<std::string> values;
};

#endif
