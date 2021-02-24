#include "pch.h"
#include "Animation.h"


Animation::Animation(int numFrames, int animSize, int animSpeed)
{
	this->numFrames = numFrames;
	this->animSize = animSize;
	this->animSpeed = animSpeed;
	frame = 0;
	count = 0;
}


Animation::~Animation()
{
}


//Update count, and switch sprite if necessary
void Animation::update()
{
	if (count < animSpeed)
		count++;
	else
	{
		count = 0;
		frame++;

		if (frame > numFrames - 1)
			frame = 0;
	}
}


//Returns an IntRect that can be used to set a sprite's textureRect
sf::IntRect Animation::calcTextRect()
{
	return sf::IntRect(frame * animSize, 0, animSize, animSize);
}


int Animation::getNumFrames()
{
	return numFrames;
}


int Animation::getAnimSize()
{
	return animSize;
}

int Animation::getAnimSpeed()
{
	return animSpeed;
}


//Set the animation to a frame. Ensures frame is valid
void Animation::setFrame(int newFrame)
{
	frame = newFrame;
	if (frame > numFrames - 1)
		frame = numFrames - 1;
	else if (frame < 0)
		frame = 0;

	return;
}
