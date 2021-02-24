#include "pch.h"
#include "Effect.h"


Effect::Effect(sf::Texture* ptr, float x, float y, int animSize, int animSpeed, int numFrames, int ttl)
{
	texturePtr = ptr;
	this->ttl = ttl;

	//Initialize the Animation 
	animation = new Animation(numFrames, animSize, animSpeed);

	//Prepare the sprite
	sprite.setTexture(*texturePtr);
	sprite.setTextureRect(animation->calcTextRect());
	sprite.setOrigin(animSize / 2, animSize / 2);
	sprite.setPosition(x, y);
}


Effect::~Effect()
{
	delete animation;
}


//Update the sprite
void Effect::update()
{
	//Update ttl
	if (ttl > 0)
		ttl--;

	//Update sprite
	animation->update();

	sprite.setTextureRect(animation->calcTextRect());
}


Animation * Effect::getAnimation()
{
	return animation;
}


sf::Sprite * Effect::getSprite()
{
	return &sprite;
}


float Effect::getX()
{
	return sprite.getPosition().x;
}


float Effect::getY()
{
	return sprite.getPosition().y;
}


int Effect::getTTL()
{
	return ttl;
}
