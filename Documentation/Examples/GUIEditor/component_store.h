#ifndef __COMPONENT_STORE__
#define __COMPONENT_STORE__


class ComponentStore
{
public:
	struct component_data_t
	{
		std::string name;
		CL_Component *component;
	};
	typedef std::list<component_data_t> cont;
	typedef cont::iterator iterator;

	void push_back(const component_data_t &component) { m_components.push_back(component); }
	
	iterator begin() { return m_components.begin(); }
	iterator end() { return m_components.end(); }
	
private:
	cont m_components;
};

#endif