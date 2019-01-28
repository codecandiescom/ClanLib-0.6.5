/* $Id: spritedata.h,v 1.2 2001/05/09 21:33:11 sphair Exp $ */

#ifndef header_spritedata
#define header_spritedata

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <string>
#include <vector>

class CL_ResourceManager;

class SpriteData
{ 
// Construction:
public:
	SpriteData(const char *section, CL_ResourceManager *resources);
	~SpriteData();

// Attributes:
public:
	const std::string &getName() const;
	//: Returns name of sprite

	int getTotalFrames() const;
	//: Returns total frames in sprite

	int getInitialAngle() const;
	//: Returns initial (unrotated) angle of sprite

	int getAnimationSpeed() const;
	//: Returns speed of animation

	int getWidth(int frame = 0) const;
	//: Returns width of a spriteframe

	int getHeight(int frame = 0) const;
	//: Returns height of a spriteframe
	
	bool isPingPong() const;
	//: Returns true if animation is played in ping-pong mode

	bool isLoop() const;
	//: Returns true if animation is played in loop (more than once)

// Operations:
public:
	void draw(int x, int y, int frame = 0, float angle = 0.0);

// Implementation:
private:
	std::vector<class Texture *> surfaces;

	int totalFrames;
	int initialAngle;
	int speedAnimation;
	bool playPingPong;
	bool playLoop;

	std::string name;
};

#endif