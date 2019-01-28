/*
	$Id: componentfile_preprocess.h,v 1.5 2001/09/08 19:12:50 japj Exp $
	
	ClanGUI, copyrights by various people. Have a look in the CREDITS file.
	
	This sourcecode is distributed using the Library GNU Public Licence,
	version 2 or (at your option) any later version. Please read LICENSE
	for details.
*/

#ifndef header_componentfile_preprocess
#define header_componentfile_preprocess

#include <string>
#include <map>
#include <list>
#include <vector>

/*
	Preprocessor functionality:

	Implements template support, by preprocessing the component script file,
	expanding template instantiations with template bodies, and removing template
	definition bodies. Example;

File before:	template <X, Y, TEXT>
				label std_label
				{
					x = X;
					y = Y;
					text = TEXT;
				}

				template <X, Y, TEXT>
				frame my_frame
				{
					x = X;
					y = Y;

					label1 = std_label<X, Y, TEXT>;
				}

				my_thing = my_frame<10, 10, testing>;

<============================================================>

File after:		frame my_thing
				{
					x = 10;
					y = 10;

					label label1
					{
						x = 10;
						y = 10;
						text = testing;
					}
				}
*/

class CL_Componentfile_Preprocess
{
public:
	CL_Componentfile_Preprocess(const std::string &data, const std::string &filename);

	operator const std::string &() { return data; }
private:
	std::string data;

	enum EParseState
	{
		PARSE_GLOBAL,
		PARSE_TEMPLATE,
		PARSE_TEMPLATE_ARG,
		PARSE_TEMPLATE_NAMEDEF,
		PARSE_TEMPLATE_BODY,
		PARSE_TEMPLATE_INSTANTIATION,
		PARSE_TEMPLATE_INSTANTIATION_ARG
	};

	struct TemplateDefinition
	{
		int file_offset;
		int name_offset;
		int definition_copy_offset;
		std::string definition;
		typedef	std::map<std::string, std::pair<int, std::list<int> > > CONV_MAP;
		CONV_MAP conversions;
	};

	struct TemplateReplacement
	{
		int offset;
		std::string new_text;
		int erase_length;
	};

	class SortTemplateDefinitionByLocationDescending
	{
	public:
		bool operator()(TemplateDefinition *x, TemplateDefinition *y) const
		{
			return x->file_offset > y->file_offset;
		}
	};
	void perform_replacements(std::string &s, std::vector<TemplateReplacement> &replacements, int global_offset);

	class SortTemplateReplacementByLocationDescending
	{
	public:
		bool operator()(const TemplateReplacement &x, const TemplateReplacement &y) const
		{
			return x.offset > y.offset;
		}
	};
};

#endif

