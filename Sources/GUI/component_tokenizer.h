/*
	$Id: component_tokenizer.h,v 1.6 2001/09/08 19:12:50 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_component_tokenizer
#define header_component_tokenizer

#include <deque>
#include <string>

class CL_InputSource;
class CL_ComponentTokenizer
{
public:
	CL_ComponentTokenizer(const std::string &data, const std::string &filename);
	~CL_ComponentTokenizer();

	bool is_space(char c);
	bool is_operator(char c);
	bool is_remark(char c, int position);
	std::string get_next_token(int *offset=NULL);
	std::string peek_next_token(int *offset=NULL);
	void pop_to_peek();
	std::string write_error(std::string err_msg);

private:
	class CL_Token
	{
	public:
		CL_Token(std::string t, int o, int l) : token(t), offset(o), line(l) { ; }
		std::string token;
		int offset;
		int line;
	};
	std::deque<CL_Token> tokens;

	int peek_pos;
	int last_line;
	bool in_remark;

	char *data;
	int total_size;
	std::string string_data;
	std::string filename;

	void tokenize();
};

#endif
