#include "sprite.h"
#include "spritedata.h"

Sprite::Sprite(SpriteData *_spritedata)
:
	spritedata(_spritedata)
{
	currentFrame = 0;
	deltaFrame = 1;

	speedAnimation = spritedata->getAnimationSpeed() / 1000.0f;
	finished = false;

	updateTime = 0.0f;
}

void Sprite::setAnimationSpeed(float speed)
{
	speedAnimation = speed;
}

void Sprite::setFrame(int frame)
{
	if(frame < 0)
		currentFrame = 0;
	else if(frame >= spritedata->getTotalFrames())
		currentFrame = spritedata->getTotalFrames() - 1;
	else
		currentFrame = frame;
}

int Sprite::getFrame() const
{
	return currentFrame;
}

void Sprite::finish()
{
	finished = true;
}

void Sprite::restart()
{
	finished = false;
	currentFrame = 0;
}

bool Sprite::isFinished() const
{
	return finished;
}

void Sprite::draw(int x, int y, float angle) const
{
	if(finished == false)
	{
		angle -= spritedata->getInitialAngle();
		while (angle < 0) angle += 360;
		while (angle > 360) angle -= 360;
		spritedata->draw(x, y, currentFrame, angle);
	}
}

void Sprite::update(float timeElapsed)
{
	int totalFrames = spritedata->getTotalFrames();
	if(speedAnimation == 0 || totalFrames < 2 || finished)
		return;

	bool playPingPong = spritedata->isPingPong();
	bool playLoop = spritedata->isLoop();
			
	updateTime += timeElapsed;
	while(updateTime > speedAnimation)
	{
		updateTime -= speedAnimation;
		currentFrame += deltaFrame;

		// Beginning or end of loop ?
		if(currentFrame >= totalFrames || currentFrame < 0)
		{
			if(playLoop == false)
			{
				finish();
				return;
			}
				
			if(playPingPong == true)
			{
				deltaFrame = -deltaFrame;	// Change direction
				if(deltaFrame > 0)
					currentFrame = 0;
				else
					currentFrame = totalFrames - 1;
			}
			else
				currentFrame = 0;			// Start from start
		}
	}
}
