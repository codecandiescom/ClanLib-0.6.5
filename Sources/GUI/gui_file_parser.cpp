/*
	$Id: gui_file_parser.cpp,v 1.6 2002/02/18 22:47:38 sphair Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#include "precomp.h"
#include <stack>
#include <list>
#include <map>
#include "API/Core/IOData/inputsource.h"
#include "component_tokenizer.h"
#include "componentfile_preprocess.h"
#include "API/Core/IOData/inputsource.h"
#include "API/GUI/component_options.h"
#include "API/GUI/component_type.h"
#include "API/GUI/gui_file_parser.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/component_manager.h"

class CL_Component;
class CL_ComponentType;

CL_GUIFileParser::CL_GUIFileParser(const std::string &filename, CL_InputSource *input) : filename(filename), input(input)
{
	parse();
}

/*
	Read the input file into a temporary buffer, and invoke the component file preprocessor.
	Return a string containing the preprocessed string as result
*/
std::string CL_GUIFileParser::preprocess_templates()
{
	char *temp_data = new char[input->size()+1];
	input->read(temp_data, input->size());
	temp_data[input->size()] = 0;
	std::string file_data(temp_data);
	delete[] temp_data;

	CL_Componentfile_Preprocess preprocessor(file_data, filename);
	return preprocessor;
}

/*
	Parse the input file, by tokenizing the data, and then process
	the tokens using a state machine.
*/
void CL_GUIFileParser::parse()
{
	// Tokenize the preprocessed input data
	CL_ComponentTokenizer lexer(preprocess_templates(), filename);

	typedef std::pair<std::string, ComponentInfo> named_component_t;
	std::stack<named_component_t> component_stack;
	std::map<std::string, int> component_id_enumerator;

	enum EParseState
	{
		PARSE_GLOBAL,
		PARSE_COMPONENT_TYPE,
		PARSE_COMPONENT_NAME,
		PARSE_COMPONENT,
		PARSE_VARIABLE_NAME,
		PARSE_VARIABLE_VALUE
	};
	EParseState state = PARSE_GLOBAL;
	EParseState prev_state = PARSE_GLOBAL;
	std::string component_type;
	std::string component_name;
	std::string variable_name;
	std::list<std::string> variable_value;
	std::string toplevel_style = "";

	while (true)
	{
		// determine current state by peeking the next token in the input stream
		const std::string &token = lexer.peek_next_token();
		if (token == "") break;

		CL_ComponentType *comp_type = NULL;
		switch (state)
		{
		// We're in the global scope of the .gui file. 
		// Look for 'style', and component declarations
		case PARSE_GLOBAL:
			if (token == "style")
			{
				if (lexer.peek_next_token() != "=")
				{
					throw CL_Error(lexer.write_error("Missing '=' following global style-declaration"));
				}
				prev_state = PARSE_GLOBAL;
				state = PARSE_VARIABLE_VALUE;
				variable_name = token;
			}
			else if ((comp_type = is_component_type(token)))
			{
				// an existing component type is being declared.
				state = PARSE_COMPONENT_TYPE;
				component_type = token;
				component_name = "";

				ComponentInfo info;
				info.type = token;
				info.component = NULL;
				info.component_type = comp_type;
				component_stack.push(named_component_t("", info));
			}
			else
			{
				throw CL_Error(lexer.write_error("Syntax error"));
			}
			break;
		case PARSE_COMPONENT_TYPE:
			if (token == "{")
			{
				// no-named component, create default name (button01 etc)
				int next_id = component_id_enumerator[component_type]++;
				char buf[20];
				sprintf(buf, "%d", next_id);
				if (strlen(buf) == 1) component_name = component_type+"0"+buf;
				else component_name = component_type+buf;
				component_stack.top().first = component_name;

				state = PARSE_COMPONENT;
			}
			else
			{
				// register component name
				component_name = token;
				component_stack.top().first = component_name;
				state = PARSE_COMPONENT_NAME;
			}
			break;
		case PARSE_COMPONENT_NAME:
			if (token != "{")
			{
				char buf[1000];
				sprintf(buf, "Missing '{' following declaration of component '%s'", component_name.c_str());
				throw CL_Error(lexer.write_error(buf));
			}
			state = PARSE_COMPONENT;
			break;
		case PARSE_COMPONENT:
			{
				comp_type = is_component_type(token);
				if (comp_type != NULL)
				{
					// a child component is being declared inside another component
					if (component_stack.empty())
					{
						throw CL_Error("Internal component manager parser error");
					}
					if (!component_stack.top().second.component_type->is_container()) 
					{
						throw CL_Error("Tried to add child to non-container component");
					}

					ComponentInfo info;
					info.type = token;
					info.component = NULL;
					info.component_type = comp_type;

					// push component being declared onto component stack
					component_stack.push(named_component_t("", info));
					component_type = token;
					component_name = "";
					state = PARSE_COMPONENT_TYPE;
				}
				else if (token == "}")
				{
					// declaration of current component is finished

					if (component_stack.empty())
					{
						throw CL_Error(lexer.write_error("Found '}' outside component"));
					}
					named_component_t comp = component_stack.top();
					component_stack.pop();

					// assign name to component info.
					comp.second.name = comp.first;

					// insert created component into component map
					std::pair<std::map<std::string, ComponentInfo>::iterator, bool> it = 
						m_component_map.insert(
							std::make_pair(comp.first, comp.second));
					
					if (!it.second)
					{
						throw CL_Error("All component names must be unique");
					}

					// This creation map is used to make sure that components get
					// pointers to their parent upon creation.
					// This map controls the ordering of component-creations using
					// component-nesting level as sort criteria.
					m_component_sort_map.insert(std::make_pair(
						component_stack.size(),
						&(*it.first).second));

					if (!component_stack.empty())
					{
						// the component being finalized is the child of another component
						// add finalized component to parent's child list.
						named_component_t &parent_comp = component_stack.top();
						if (!parent_comp.second.component_type->is_container())
						{
							throw CL_Error(lexer.write_error("Found component inside other component (must be container)"));
						}
						else
						{
							parent_comp.second.children.push_back(&(*it.first).second);
						}
					}
					else
					{
						// parsing of a global component is finished - return to global parse state
						state = PARSE_GLOBAL;
					}
				}
				else if (token.length() == 1 && lexer.is_operator(token[0]))
				{
					throw CL_Error(lexer.write_error("Expected variable-name or component-type"));
				}
				else
				{
					// we've entered the declaration of a component option
					prev_state = PARSE_COMPONENT;
					state = PARSE_VARIABLE_NAME;
					variable_name = token;
					variable_value.clear();
				}
			}
			break;
		case PARSE_VARIABLE_NAME:
			if (token == "=")
			{
				// a component option with a value is being declared
				state = PARSE_VARIABLE_VALUE;
			}
			else if (token == ";")
			{
				// a component option without a value is being declared
				if (!component_stack.top().second.component_type->has_option(variable_name))
				{
					char buf[1000];
					sprintf(buf, "Failed to add '%s' to component of type '%s'",
						variable_name.c_str(),
						component_stack.top().second.type.c_str());
					throw CL_Error(lexer.write_error(buf));
				}
				#ifdef BORLAND
				component_stack.top().second.options.options.insert(std::make_pair(std::string(variable_name), std::string("")));
				#else
				component_stack.top().second.options.options.insert(std::make_pair<std::string, std::string>(variable_name, ""));
				#endif
				variable_name = "";
				variable_value.clear();
				state = prev_state;
			}
			else
			{
				throw CL_Error(lexer.write_error("Expected ';' following variable declaration"));
			}
			break;
		case PARSE_VARIABLE_VALUE:
			if (token != ";")
			{
				// this is the option value
				variable_value.push_back(token);
			}
			else
			{
				if (component_stack.empty())
				{
					// the value declared is not a component option, but a global option value
					m_globals.push_back(variable_pair_t(variable_name, evaluate_expression(variable_value)));
				}
				else if (!component_stack.top().second.component_type->has_option(variable_name))
				{
					// unrecognized component option declared
					std::string variable_value_str;
					int size_list = variable_value.size();
					std::list<std::string>::iterator it;
					for (it=variable_value.begin();it!=variable_value.end();it++)
					{
						variable_value_str.append(*it);
						size_list--;
						if (size_list > 0) variable_value_str.append(1, ' ');
					}
					char buf[1000];
					sprintf(buf, "Failed to add '%s = %s' to component of type '%s'",
						variable_name.c_str(), 
						variable_value_str.c_str(),
						component_stack.top().second.type.c_str());
					throw CL_Error(lexer.write_error(buf));
				}
				else
				{
					// add option to current component's option list
					component_stack.top().second.options.options.insert(std::make_pair(variable_name, evaluate_expression(variable_value)));
				}
				variable_name = "";
				variable_value.clear();
				state = prev_state;
			}
			break;
		}
		lexer.pop_to_peek();
	}
}

std::string CL_GUIFileParser::concatenate_stringlist(std::list<std::string> &expression)
{
	std::string result;
	int size_list = expression.size();
	std::list<std::string>::iterator it;
	for (it=expression.begin();it!=expression.end();it++)
	{
		result.append(*it);
		size_list--;
		if (size_list > 0) result.append(1, ' ');
	}
	return result;
}

/*
Simple expression evaluator that allows very simple arithmetic operations to be performed.
The function recognizes '+', '-', '*', '/', '(', ')'. Paranthesises are ignored, and the
result is calculated left-to-right.
*/
std::string CL_GUIFileParser::evaluate_expression(std::list<std::string> &expression)
{
	bool is_expression = true;
	std::string result;
	std::list<std::string>::iterator it = expression.begin();
	for (;it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+" || s == "-" || s == "*" || s == "/" || s == "(" || s == ")")
		{
			continue;
		}
		const char *str = s.c_str();
		for (;*str;str++)
		{
			if ((*str < '0' || *str > '9') && *str != '.')
			{
				break;
			}
		}
		if (*str)
		{
			is_expression = false;
			break;
		}
	}

	if (!is_expression)
	{
		return concatenate_stringlist(expression);
	}

	/*
		Please note:
		This expression evaluation function is NOT FINISHED!
		This is a totally basic no-operator-precedence implementation
		that disregards paranthesis and just plain sucks, but 
		I don't have the willpower to complete it - someone else feel free :-)
	*/
	int op = 0;
	float result_val = 0.0f;
	for (it=expression.begin();it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+") op = 0;
		else if (s == "-") op = 1;
		else if (s == "*") op = 2;
		else if (s == "/") op = 3;
		else if (s == "(" || s == ")")
		{
			continue;
		}
		else
		{
			if (op == -1) throw CL_Error("Could not evaluate expression");
			float val = (float)atof(s.c_str());
			switch (op)
			{
			case 0:
				result_val += val;
				break;
			case 1:
				result_val -= val;
				break;
			case 2:
				result_val *= val;
				break;
			case 3:
				result_val /= val;
				break;
			default:
				result_val = val;
				break;
			}
			op = -1;
		}
	}

	int rval = int(result_val+0.5);
	return CL_String(rval);
}

CL_ComponentType *CL_GUIFileParser::is_component_type(const std::string &token)
{
	std::map<std::string, CL_ComponentType *>::iterator it = CL_ComponentManager::component_types.find(token);
	if (it == CL_ComponentManager::component_types.end()) 
		return NULL;

	return (*it).second;
}
