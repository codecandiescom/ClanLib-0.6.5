/*
	$Id: clanstring.h,v 1.33 2002/02/20 08:06:28 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanCore="System"
//! header=core.h

#ifndef header_clanstring
#define header_clanstring

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

#ifndef WIN32
#include <cctype>
#endif

#ifdef __BORLANDC__
#include <ctype.h>
#endif

#ifdef __MWERKS__
#include <extras.h>
#endif

#ifdef __MACOS__
#include <cctype>
#define NEED_FCVT
#endif

#ifdef NEED_FCVT
extern char *fcvt (double, int, int *, int *);
#endif

#include "../Resources/resourcetype_string.h"
#include "../Resources/resource.h"
#include "../Resources/resource_manager.h"
#include "error.h"

using namespace std;

//: Clan String Class
class CL_String
{
public:
//! Construction:
	//: String Constructor
	CL_String(const std::string &res_id, CL_ResourceManager *resources)
	{
		CL_Resource res = resources->get_resource(res_id);
		res.load();
		CL_ResourceData_String *data =
			(CL_ResourceData_String *) res.get_data("string");
		str = data->value;
		res.unload();
	}

	//: String Constructor
	CL_String(const std::string &res_id, CL_ResourceManager *resources, const std::string &def_value)
	{
		try
		{
			CL_Resource res = resources->get_resource(res_id);
			res.load();
			CL_ResourceData_String *data =
				(CL_ResourceData_String *) res.get_data("string");
			str = data->value;
			res.unload();
		}
		catch (CL_Error err)
		{
			str = def_value;
		}
	}

	//: String Constructor
	CL_String()
	{
	}
	
	//: String Constructor
	CL_String(const std::string &_str)
	{
		this->str = append_string(NULL, _str.c_str());
	}

	//: String Constructor
	CL_String(const int value)
	{
		str=int_to_string(NULL, value);
	}

	//: String Constructor
	CL_String(const float float_value)
	{
		str=float_to_string(NULL, float_value);
	}

	//: String Constructor
	CL_String(const char *text)
	{
		str=text;
	}
	
	//: String Constructor
	CL_String(const CL_String &clone)
	{
		str=clone.str;
	}

	//: String Constructor
	CL_String(const CL_String &text1, const CL_String &text2)
	{
		str=text1.str;
		str+=text2.str;
	}

	//: String Constructor
	CL_String(const char *text1, const char *text2)
	{
		str=text1;
		str+=text2;
	}

	//: String Constructor
	CL_String(const char *text, int length)
	{
		str.append(text, length);
	}
//! Operators:

	//: String Operator
	CL_String operator+ (const CL_String &other_string)
	{
		return CL_String(str, other_string.str);
	}

	//: String Operator
	CL_String operator+ (const char *other_text)
	{
		return CL_String(str, other_text);
	}

	//: String Operator
	CL_String operator+ (const int number)
	{
		return CL_String(int_to_string(str.c_str(), number));
	}

	//: String Operator
	CL_String operator+ (const float number)
	{
		return CL_String(float_to_string(str.c_str(), number));
	}

	//: String Operator
	CL_String &operator+= (const CL_String &other_string)
	{
		str += other_string.str;
		return *this;
	}

	//: String Operator
	CL_String &operator+= (const char *other_text)
	{
		str += other_text;
		return *this;
	}

	//: String Operator
	CL_String &operator+= (const int number)
	{
		str = int_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator+= (const float number)
	{
		str = float_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator+= (double float_number)
	{
		return operator+= ((float) float_number);
	}

	//: String Operator
	CL_String &operator= (const CL_String &other_string)
	{
		str=other_string.str;
		return *this;
	}

	//: String Operator
	CL_String &operator= (const char *other_text)
	{
		str = other_text;
		return *this;
	}

	//: String Operator
	CL_String &operator= (const int number)
	{
		str=int_to_string(NULL, number);
		return *this;
	}

	//: String Operator
	CL_String &operator= (const float float_number)
	{
		str=float_to_string(NULL, float_number);
		return *this;
	}

	//: String Operator
	CL_String &operator= (double float_number)
	{
		return operator= ((float) float_number);
	}

	//: String Operator
	bool operator== (const CL_String &other_string)
	{
		return str == other_string.str;
	}

	//: String Operator
	bool operator== (const char *other_string)
	{
		return str == other_string;
	}
	
	//: String Operator
	bool operator!= (const CL_String &other_string)
	{
		return !operator==(other_string);
	}

	//: String Operator
	bool operator!= (const char *other_text)
	{
		return !operator==(other_text);
	}

	//: String Operator
	operator const char* ()
	{
		return str.c_str();
	}
	
	//: String Operator
	operator std::string ()
	{
		return str;
	}

	//: String Operator
	CL_String &operator<< (const CL_String &other_string)
	{
		str += other_string.str;
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const char *other_string)
	{
		str += other_string;
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const unsigned char *other_string)
	{
		str += (const char *) other_string;
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const unsigned short number)
	{
		str=int_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const short number)
	{
		str=int_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const unsigned int number)
	{
		str=int_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const int number)
	{
		str=int_to_string(str.c_str(), number);
		return *this;
	}

	//: String Operator
	CL_String &operator<< (const float float_number)
	{
		str=float_to_string(str.c_str(), float_number);
		return *this;
	}

	//: String Operator
	CL_String &operator<< (double float_number)
	{
		return operator<< ((float) float_number);
	}

//! Attributes:
	//: Get String
	char *get_string()
	{
		return (char *) str.c_str();
	}

	//: Get Length
	int get_length()
	{
		return (int)str.length();
	}

	//: Get As int
	int get_as_int()
	{
		return atoi(str.c_str());
	}

	//: Get As Float
	float get_as_float()
	{
		return (float)atof(str.c_str());
	}

	//: Get Word
	CL_String get_word(int word_no)
	{
		int len=get_length();
		int cur_word=-1;
		int word_start=0;

		int cur_state=0;
		for (int i=0; i<len; i++)
		{
			if (cur_state == 0)
			{
				if (str[i]!=32)
				{
					if (str[i]==34)
					{
						word_start = i+1;
						cur_state = 2;
					}
					else
					{
						word_start = i;
						cur_state = 1;
					}
					cur_word++;
				}
			}
			else if (cur_state == 1)
			{
				if (str[i]==32)
				{
					if (cur_word == word_no) return mid(word_start, i-word_start);
					cur_state = 0;
				}
			}
			else if (cur_state == 2)
			{
				if (str[i]==34)
				{
					if (cur_word == word_no) return mid(word_start, i-word_start);
					cur_state = 0;
				}
			}
		}

		if (cur_word == word_no) return mid(word_start);

		return CL_String("");
	}

	//: Mid
	CL_String mid(int start, int len=-1)
	{
		if (len==-1) len=get_length()-start;
		return CL_String(std::string(str, start, len));
	}

	//: Left
	CL_String left(int len)
	{
		int strlength = get_length();
		if (strlength < len) return *this;
		return CL_String(std::string(str, 0, len));
	}

	//: Right
	CL_String right(int len)
	{
		int strlength = get_length();
		if (strlength < len) return *this;
		return CL_String(std::string(str, strlength-len, len));
	}

	//: Find
	int find(int character, int start_pos=0)
	{
		const char *ptr = str.c_str();
		char *pos = (char*)strchr(ptr+start_pos, character);
		if (pos == NULL) return -1;
		return int(pos-ptr);
	}

	//: Find Last
	int find_last(char character) const
	{
		const char *temp = str.c_str();
		int start_pos = (int)str.length();
		for(int i = start_pos; i; i--)
			if(*(temp+i) == character)
				return i;
		return -1;
	}

	//: Case Compare
	bool case_compare(const char *other)
	{
		if (other==NULL) return false;

#ifdef WIN32
#ifdef __BORLANDC__
		if (stricmp(str.c_str(), other)==0) return true;
#else
		if (_stricmp(str.c_str(), other)==0) return true;
#endif
#else
		if (strcasecmp(str.c_str(), other)==0) return true;
#endif
		return false;
	}

//! Operations:
	//: To Lower
	void to_lower()
	{
		int len = (int)str.length();
		for (int i=0; i<len; i++)
		{
			str[i] = tolower(str[i]);
		}
	}

	//: To Upper
	void to_upper()
	{
		int len = (int)str.length();
		for (int i=0; i<len; i++)
		{
			str[i] = toupper(str[i]);
		}
	}

private:
	std::string str;

	std::string int_to_string(const char *prefix, const int number)
	{
		char buf[20];
		sprintf(buf, "%d", number);

		int len=0;
		if (prefix != NULL) len = (int)strlen(prefix);

		char *text = new char[len + strlen(buf) + 1];
		if (prefix != NULL)
		{
			strcpy(text, prefix);
			strcat(text, buf);
		}
		else
		{
			strcpy(text, buf);
		}

		std::string res(text);
		delete[] text;
		return res;
	}
	
	std::string float_to_string(const char *prefix, const float _float_number)
	{
		char buf[25];
		buf[0] = 0;

		int decimal, sign;
		int precision = 4;
		char temp;

		float float_number = _float_number;

		const char *_float_buffer = fcvt(
			float_number,
			precision,
			&decimal,
			&sign);
			
		char *float_buffer = new char[strlen(_float_buffer)+1];
		strcpy(float_buffer, _float_buffer);

		if (sign) strcpy(buf, "-");
		if (decimal <= 0)
		{
			strcat(buf, "0.");
			strcat(buf, float_buffer);
		}
	    else
		{
			temp=float_buffer[decimal];
			float_buffer[decimal] = 0;
			strcat(buf, float_buffer);
			float_buffer[decimal] = temp;

			strcat(buf, ".");
			strcat(buf, &float_buffer[decimal]);
		}
    
		int len=0;
		if (prefix != NULL) len = (int)strlen(prefix);

		char *text = new char[len + strlen(buf) + 1];
		if (prefix != NULL)
		{
			strcpy(text, prefix);
			strcat(text, buf);
		}
		else strcpy(text, buf);

		std::string res(text);
		delete[] text;
		return res;
	}

	std::string append_string(const char *prefix, const char *postfix)
	{
		int len=0;
		if (prefix != NULL) len = (int)strlen(prefix);
		if (postfix != NULL) len += (int)strlen(postfix);
		char *text = new char[len + 1];
		if (prefix != NULL)
		{
			strcpy(text, prefix);
			if (postfix != NULL) strcat(text, postfix);
		}
		else if (postfix != NULL)
		{
			strcpy(text, postfix);
		}

		std::string res(text);
		delete[] text;
		return res;
	}
};

#endif
