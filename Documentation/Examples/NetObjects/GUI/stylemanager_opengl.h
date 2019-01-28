#ifndef header_stylemanager_opengl
#define header_stylemanager_opengl

#include <ClanLib/gui.h>

class CL_ResourceManager;
class CL_Component;

class CL_StyleManager_OpenGL : public CL_StyleManager_Default
{
public:
	CL_StyleManager_OpenGL(CL_ResourceManager *resources);
	virtual ~CL_StyleManager_OpenGL();

// Operations:
	void fill_rect(
		int x1, int y1, int x2, int y2,
		float r, float g, float b,
		float alpha0, float alpha1, float alpha2, float alpha3);

	void fill_rect(
		int x1, int y1, int x2, int y2,
		float r0, float g0, float b0, 
		float r1, float g1, float b1, 
		float r2, float g2, float b2, 
		float r3, float g3, float b3,
		float alpha);

// Overridables:
	virtual void connect_styles(
		const std::string &type,
		CL_Component *owner);
};

#endif
