/*
	$Id: resource_tokenizer.cpp,v 1.5 2001/11/02 02:24:09 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include "resource_tokenizer.h"
#include <API/Core/IOData/inputsource.h>
#include <API/Core/System/cl_assert.h>
#include <API/Core/System/clanstring.h>

CL_ResourceTokenizer::CL_ResourceTokenizer(std::string filename, CL_InputSource *input)
{
	this->filename = filename;

	cur_pos = 0;
	cur_line = 1;
	in_remark = false;

	cl_assert(input != NULL);

	total_size = input->size();
	config_file_data = new unsigned char[total_size];
	input->read(config_file_data, total_size);
}

CL_ResourceTokenizer::~CL_ResourceTokenizer()
{
	delete[] config_file_data;
}

bool CL_ResourceTokenizer::is_operator(char c)
{
	return
		c == ',' ||
		c == '=' ||
		c == '(' ||
		c == ')' ||
		c == ';' ||
		c == '{' ||
		c == '}';
}

bool CL_ResourceTokenizer::is_space(char c)
{
	return
		c == ' ' ||
		c == '\t' ||
		c == 13 ||
		c == 10;
}

bool CL_ResourceTokenizer::is_remark(char c, int position)
{
	if (in_remark)
	{
		if (c == '\n') 
		{	
			in_remark = false;
		}
		return true;
	}
	else
	{
		if (c == '/')
		{
			if (
				position+1 < total_size &&
				config_file_data[position+1] == '/')
			{
				in_remark = true;
				return true;
			}
		}
		else if (c == '#')
		{
			in_remark = true;
			return true;
		}
	}
	return false;
}
	
std::string CL_ResourceTokenizer::get_next_token()
{
	while (
		cur_pos < total_size && 
		(
			is_remark(config_file_data[cur_pos], cur_pos) ||
			is_space(config_file_data[cur_pos])
		))
	{
		if (config_file_data[cur_pos] == '\n') cur_line++;
		cur_pos++;
	}
	if (cur_pos == total_size) return "";

	if (is_operator(config_file_data[cur_pos]))
	{
		char temp[2];
		temp[0] = config_file_data[cur_pos];
		temp[1] = 0;
		std::string ret(temp);
		cur_pos++;

		return ret;
	}
	else
	{
		int temp_pos = cur_pos;
		if (config_file_data[temp_pos] == '"')
		{
			int num_chars = 0;
			temp_pos++;
			while (temp_pos < total_size)
			{
				if (config_file_data[temp_pos] == '"')
				{
					if (temp_pos+1>=total_size || config_file_data[temp_pos+1] != '"') break;
					else
					{
						temp_pos++;
					}
				}
				temp_pos++;
				num_chars++;
			}
			if (temp_pos == total_size)
			{
				write_error("Missing '""' following '""'-begin");
				return "";
			}

			char *temp = new char[num_chars+1];
			temp[num_chars] = 0;
			int ofs = 1;
			for (int i=0;i<num_chars;i++)
			{
				temp[i] = config_file_data[cur_pos+ofs+i];
				if (config_file_data[cur_pos+ofs+i] == '"') 
				{
					ofs++;
				}
			}
			cur_pos = temp_pos+1;

			std::string ret(temp);
			delete [] temp;

			return ret;
		}
		else
		{
			while (temp_pos < total_size &&
				!is_space(config_file_data[temp_pos]) &&
				!is_operator(config_file_data[temp_pos]) &&
				!is_remark(config_file_data[temp_pos], cur_pos))
			{
				temp_pos++;
			}
			int tok_size = temp_pos-cur_pos;
			char *temp = new char[tok_size+1];
			temp[tok_size] = 0;
			memcpy(temp, &config_file_data[cur_pos], tok_size);
			cur_pos += tok_size;

			std::string ret(temp);
			delete [] temp;

			return ret;
		}
	}
}

std::string CL_ResourceTokenizer::write_error(std::string err_msg)
{
	CL_String err;
	err << "Error occured during read of config file '" <<
		filename.c_str() << "' (line " << cur_line << "): " << err_msg.c_str();

	return err.get_string();
}
