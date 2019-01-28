/*
	$Id: resource_tokenizer.h,v 1.2 2000/07/15 15:45:54 starch Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_resource_tokenizer
#define header_resource_tokenizer

#include <string>

class CL_InputSource;
class CL_ResourceTokenizer
{
public:
	CL_ResourceTokenizer(std::string filename, CL_InputSource *input);
	~CL_ResourceTokenizer();

	bool is_space(char c);
	bool is_operator(char c);
	bool is_remark(char c, int position);
	std::string get_next_token();
	std::string write_error(std::string err_msg);

private:
	int cur_pos, cur_line;
	std::string filename;
	int total_size;
	unsigned char *config_file_data;
	bool in_remark;
};

#endif
