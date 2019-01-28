/* $Id: sprite.h,v 1.8 2001/05/12 21:46:31 sphair Exp $ */

#ifndef header_sprite_included
#define header_sprite_included

class SpriteData;

class Sprite
{ 
// Construction:
public:
	Sprite(SpriteData *spritedata);
	
// Attributes:
public:
	int getInitialAngle() const;
	float getAnimationSpeed() const;
	int getFrame() const;

	bool isPingPong() const;
	bool isLoop() const;
	bool isFinished() const;

// Operations:
public:
	void draw(int x, int y, float angle = 0.0f) const;
	void update(float timeElapsed);

	void setAnimationSpeed(float speed);
	void setFrame(int frame);

	void finish();
	void restart();

// Implementation:
private:
	signed int currentFrame;
	signed int deltaFrame;

	float speedAnimation;
	bool finished;

	float updateTime;

	SpriteData *spritedata;
};

#endif
