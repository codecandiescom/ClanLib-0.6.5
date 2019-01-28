#include <ClanLib/core.h>

#include "spritedata.h"
#include "texture.h"

SpriteData::SpriteData(const char *section, CL_ResourceManager *resources)
{
	CL_String res;

	name = section;
	totalFrames = 1;
	speedAnimation = 100;
	playPingPong = false;
	playLoop = true;
	initialAngle = 0;

	try
	{
		res = section;
		res += "/animationspeed";
		speedAnimation = CL_Integer(res, resources);
	} catch (CL_Error err) {};

	try
	{
		res = section;
		res += "/initialangle";
		initialAngle = CL_Integer(res, resources);
	} catch (CL_Error err) {};

	try
	{
		res = section;
		res += "/pingpong";
		playPingPong = CL_Boolean(res, resources);
	} catch (CL_Error err) {};

	try
	{
		res = section;
		res += "/loop";
		playLoop = CL_Boolean(res, resources);
	} catch (CL_Error err) {};

	int currentSprite = 0;

	try
	{
		for(;;) {
			res = section;
			res += "/frame";
			res += currentSprite + 1;

			Texture *surface = Texture::load(res, resources);
//			int length = surface->get_num_frames();
//			if(length > 1)
				// TODO: Split surface into smaller surfaces

			surfaces.push_back(surface);
			currentSprite++;
		}
	}
	catch (CL_Error err) { ; }		// No more sprite resources found
	
	totalFrames = surfaces.size();
	if(totalFrames == 0)
	{
		std::string error = section;
		error += " sprite contains no frames";
		throw CL_Error(error);
	}
}

SpriteData::~SpriteData()
{
	for(int i=0; i<totalFrames; i++)
		delete surfaces[i];
}

int SpriteData::getTotalFrames() const
{
	return totalFrames;
}

int SpriteData::getInitialAngle() const
{
	return initialAngle;
}

int SpriteData::getAnimationSpeed() const
{
	return speedAnimation;
}

bool SpriteData::isPingPong() const
{
	return playPingPong;
}

bool SpriteData::isLoop() const
{
	return playLoop;
}

const std::string &SpriteData::getName() const
{
	return name;
}

int SpriteData::getWidth(int frame) const
{
	return surfaces[frame]->get_width();	
}

int SpriteData::getHeight(int frame) const
{
	return surfaces[frame]->get_height();	
}

void SpriteData::draw(int x, int y, int frame, float rotation)
{
	if(frame < 0 || frame > totalFrames)
	{
		std::string error = name;
		error += " spriteframe was used outside range";
		throw CL_Error(error);
	}

	surfaces[frame]->put_screen(x, y, rotation);	
}