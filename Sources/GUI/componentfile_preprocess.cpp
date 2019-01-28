/*
	$Id: componentfile_preprocess.cpp,v 1.14 2001/12/11 20:44:22 mbn Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <algorithm>

#include "API/Core/System/error.h"
#include "API/GUI/component_manager.h"
#include "component_manager_generic.h"

#include "componentfile_preprocess.h"
#include "component_tokenizer.h"

CL_Componentfile_Preprocess::CL_Componentfile_Preprocess(
	const std::string &_data,
	const std::string &filename) : data(_data)
{
	CL_ComponentTokenizer lexer(data, filename);
	EParseState state = PARSE_GLOBAL;
	EParseState instantiation_dest_state = PARSE_GLOBAL;

	typedef std::map<std::string, TemplateDefinition *> TMAP;
	TMAP template_map;

	int offset;
	int body_scoping_count = 0;
	int cur_template_arg = 0;
//	bool copy_to_output = true;
	std::string template_name;
	std::string last_variable_name;
	int last_variable_offset = 0;
	std::string last_token;
	int last_offset = 0;
	TemplateDefinition *cur = NULL;

	TMAP::iterator cur_instantiation;
	std::vector<std::string> template_arg_vals;
	std::string cur_template_arg_val;

	std::vector<TemplateReplacement> replacements_local;
	std::vector<TemplateReplacement> replacements_global;
	std::vector<TemplateReplacement> *cur_replacement_list = NULL;

	while (true)
	{
		bool pop_token = true;
		const std::string &token = lexer.peek_next_token(&offset);
		if (token == "") break;

		if (token == "=")
		{
			last_variable_name = last_token;
			last_variable_offset = last_offset;
		}
		last_token = token;
		last_offset = offset;
		
		switch (state)
		{
		case PARSE_GLOBAL:
			if (token == "template")
			{
				state = PARSE_TEMPLATE;
				cur = new TemplateDefinition;
				cur->file_offset = offset;
			}
			else
			{
				TMAP::iterator f_it = template_map.find(token);
				if (f_it != template_map.end())
				{
					state = PARSE_TEMPLATE_INSTANTIATION;
					instantiation_dest_state = PARSE_GLOBAL;
					cur_instantiation = f_it;
					cur_replacement_list = &replacements_global;
				}
			}
			break;
		case PARSE_TEMPLATE:
			if (token == "<")
			{
				state = PARSE_TEMPLATE_ARG;
				cur_template_arg = 0;
			}
			else
			{
				state = PARSE_TEMPLATE_NAMEDEF;
				pop_token = false;
			}
			break;
		case PARSE_TEMPLATE_ARG:
			{
				const std::string &peek = lexer.peek_next_token();
				if (peek == ",")
				{
					// nop!
				}
				else if (peek == ">")
				{
					state = PARSE_TEMPLATE_NAMEDEF;
				}
				else
				{
					throw CL_Error(lexer.write_error("Invalid template syntax"));
				}
				cur->conversions[token] = std::make_pair(cur_template_arg, std::list<int>());
				cur_template_arg++;
			}
			break;
		case PARSE_TEMPLATE_NAMEDEF:
			cur->definition_copy_offset = offset;
			if (CL_ComponentManager::component_types.find(token) != CL_ComponentManager::component_types.end())
			{
				const std::string &peek = lexer.peek_next_token(&offset);
				if (peek == "{") throw CL_Error(lexer.write_error("Missing name"));
				template_name = peek;
				cur->name_offset = offset - cur->file_offset;
				state = PARSE_TEMPLATE_BODY;
				body_scoping_count = 0;
			}
			else
			{
				throw CL_Error(lexer.write_error("Missing component type"));
			}
			break;
		case PARSE_TEMPLATE_BODY:
			if (token == "{")
			{
				body_scoping_count++;
			}
			else if (token == "}")
			{
				body_scoping_count--;
				if (body_scoping_count == 0)
				{
					cur->definition = std::string(
						&data.c_str()[cur->file_offset], 
						offset+token.length() - cur->file_offset);

					TemplateReplacement s;
					s.erase_length = cur->definition.length();
					s.offset = cur->file_offset;
					replacements_global.push_back(s);

					perform_replacements(cur->definition, replacements_local, cur->file_offset);
					replacements_local.clear();

//					std::cout << "Template: '" << cur->definition.c_str() << std::endl;
					state = PARSE_GLOBAL;
					template_map[template_name] = cur;
					cur = NULL;
				}
			}
			else if (cur->conversions.find(token) != cur->conversions.end())
			{
				cur->conversions[token].second.push_back(offset - cur->file_offset);
			}
			else
			{
				TMAP::iterator f_it = template_map.find(token);
				if (f_it != template_map.end())
				{
					state = PARSE_TEMPLATE_INSTANTIATION;
					instantiation_dest_state = PARSE_TEMPLATE_BODY;
					cur_instantiation = f_it;
					cur_replacement_list = &replacements_local;
				}
			}
			break;
		case PARSE_TEMPLATE_INSTANTIATION:
			if (token != "<")
			{
				throw CL_Error(lexer.write_error("Missing '<' following use of template"));
			}
			state = PARSE_TEMPLATE_INSTANTIATION_ARG;
			cur_template_arg = 0;
			break;
		case PARSE_TEMPLATE_INSTANTIATION_ARG:
			{
				if (token != "," && token != ">")
				{
					cur_template_arg_val.append(token);
					cur_template_arg_val.append(1, ' ');
				}
				else
				{
					cur_template_arg_val.erase(cur_template_arg_val.length()-1, 1);
					template_arg_vals.push_back(cur_template_arg_val);
					cur_template_arg_val = "";
					cur_template_arg++;

					if (token == ">")
					{
						if (lexer.peek_next_token(&offset) != ";")
						{
							throw CL_Error(lexer.write_error("Missing ';' following template instantiation"));
						}
						state = instantiation_dest_state;
						int template_instantiation_length = offset - last_variable_offset + 1;
						std::string expanded_template_instantiation = (*cur_instantiation).second->definition;

						std::vector<TemplateReplacement> replacements;

						TemplateDefinition *cur_definition = (*cur_instantiation).second;
						std::string cur_definition_name = (*cur_instantiation).first;
						for (int i=0;i<(int) template_arg_vals.size();i++)
						{
							TemplateDefinition::CONV_MAP::iterator it = cur_definition->conversions.begin();
							for (;it!=cur_definition->conversions.end();it++)
							{
								if ((*it).second.first == i)
								{
									int name_length = (*it).first.length();
									std::list<int> &clist = (*it).second.second;
									std::list<int>::iterator c_it = clist.begin();
									std::list<int>::iterator c_it_end = clist.end();
									for (;c_it!=c_it_end;c_it++)
									{
										TemplateReplacement s;
										s.offset = *c_it;
										s.erase_length = name_length;
										s.new_text = template_arg_vals[i];
										replacements.push_back(s);
									}
								}
							}
						}
						perform_replacements(expanded_template_instantiation, replacements, 0);

						expanded_template_instantiation.erase(cur_definition->name_offset, cur_definition_name.length());
						expanded_template_instantiation.insert(cur_definition->name_offset, last_variable_name);
						expanded_template_instantiation.erase(0, cur_definition->definition_copy_offset - cur_definition->file_offset);

	//					std::cout << expanded_template_instantiation.c_str() << std::endl;

						TemplateReplacement s;
						s.erase_length = template_instantiation_length;
						s.new_text = expanded_template_instantiation;
						s.offset = last_variable_offset;
						cur_replacement_list->push_back(s);

						template_arg_vals.clear();
					}
				}
			}
			break;
		default:
			throw CL_Error(lexer.write_error("Internal component manager preprocessor error"));
		}

		if (pop_token) lexer.pop_to_peek();
	}

	perform_replacements(data, replacements_global, 0);
}
	
void CL_Componentfile_Preprocess::perform_replacements(
	std::string &string_data,
	std::vector<TemplateReplacement> &replacements,
	int global_offset)
{
	std::sort(replacements.begin(), replacements.end(), SortTemplateReplacementByLocationDescending());
	std::vector<TemplateReplacement>::iterator it = replacements.begin();
	std::vector<TemplateReplacement>::iterator it_end = replacements.end();
	for (;it!=it_end;it++)
	{
		TemplateReplacement &s = *it;
		string_data.erase(s.offset-global_offset, s.erase_length);
		string_data.insert(s.offset-global_offset, s.new_text);
	}
}
