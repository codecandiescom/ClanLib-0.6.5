/*
	$Id: component_tokenizer.cpp,v 1.13 2001/12/31 01:31:42 starch Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "component_tokenizer.h"

CL_ComponentTokenizer::CL_ComponentTokenizer(const std::string &_data, const std::string &_filename)
{
	filename = _filename;
	string_data = _data;
	data = (char *) string_data.c_str();
	total_size = string_data.size();

	peek_pos = 0;
	last_line = 0;
	in_remark = false;
	tokenize();
}

CL_ComponentTokenizer::~CL_ComponentTokenizer()
{
}

bool CL_ComponentTokenizer::is_operator(char c)
{
	const char *operators = ",=();{}<+-*/>";

	return strchr(operators, c) != NULL;
}

bool CL_ComponentTokenizer::is_space(char c)
{
	const char *spaces = " \t\r\n";

	return strchr(spaces, c) != NULL;
}

bool CL_ComponentTokenizer::is_remark(char c, int position)
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
				data[position+1] == '/')
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

void CL_ComponentTokenizer::tokenize()
{
	int cur_pos = 0;
	int cur_line = 0;
	while (cur_pos < total_size)
	{
		while (
			cur_pos < total_size && 
			(
				is_remark(data[cur_pos], cur_pos) ||
				is_space(data[cur_pos])
			))
		{
			if (data[cur_pos] == '\n') cur_line++;
			cur_pos++;
		}
		if (cur_pos == total_size) break;

		if (is_operator(data[cur_pos]))
		{
			tokens.push_back(CL_Token(std::string(&data[cur_pos], 1), cur_pos, cur_line));
			cur_pos++;
		}
		else
		{
			int temp_pos = cur_pos;
			if (data[temp_pos] == '"')
			{
				int num_chars = 0;
				temp_pos++;
				while (temp_pos < total_size)
				{
					if (data[temp_pos] == '"')
					{
						if (temp_pos+1>=total_size || data[temp_pos+1] != '"') break;
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
					break;
				}

				char *temp = new char[num_chars+1];
				temp[num_chars] = 0;
				int ofs = 1;
				for (int i=0;i<num_chars;i++)
				{
					temp[i] = data[cur_pos+ofs+i];
					if (data[cur_pos+ofs+i] == '"') 
					{
						ofs++;
					}
				}
				cur_pos = temp_pos+1;

				tokens.push_back(CL_Token(std::string(temp), cur_pos, cur_line));
				delete temp;
			}
			else
			{
				while (temp_pos < total_size &&
					!is_space(data[temp_pos]) &&
					!is_operator(data[temp_pos]) &&
					!is_remark(data[temp_pos], cur_pos))
				{
					temp_pos++;
				}
				int tok_size = temp_pos-cur_pos;
				tokens.push_back(CL_Token(std::string(&data[cur_pos], tok_size), cur_pos, cur_line));
				cur_pos += tok_size;
			}
		}
	}
}

std::string CL_ComponentTokenizer::get_next_token(int *offset)
{
	peek_pos = 0;
	last_line = tokens[0].line;
	std::string token = tokens[0].token;
	if (offset != NULL) *offset = tokens[0].offset;

	tokens.pop_front();

	return token;
}

std::string CL_ComponentTokenizer::peek_next_token(int *offset)
{
	if (peek_pos >= (int) tokens.size())
	{
		return "";
	}
	if (offset != NULL) *offset = tokens[peek_pos].offset;
	return tokens[peek_pos++].token;
}

void CL_ComponentTokenizer::pop_to_peek()
{
	while (peek_pos > 0)
	{
		peek_pos--;
		tokens.pop_front();
	}
}

std::string CL_ComponentTokenizer::write_error(std::string err_msg)
{
	CL_String err;
	err << "Error occured during read of config file '" <<
		filename.c_str() << "' (line " << last_line << "): " << err_msg.c_str();

	return err;
}
