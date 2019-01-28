#ifndef header_options
#define header_options

#include <string>

class Options
{
// Construction:
public:
	Options();

// Attributes:
public:
	const std::string &get_player_name() const;

// Operations:
public:
	void set_player_name(const std::string &name);

// Implementation:
private:
	std::string player_name;
};

#endif
