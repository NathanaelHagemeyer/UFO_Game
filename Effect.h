#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"


class Effect
{
public:
	Effect(sf::Texture*, float, float, int, int, int, int);
	~Effect();
	void update();
	Animation * getAnimation();
	sf::Sprite * getSprite();
	float getX();
	float getY();
	int getTTL();
private:
	Animation * animation;
	sf::Texture* texturePtr;
	sf::Sprite sprite;
	int ttl; //Time to live (in game frames), if -1 does not count down and should live forever
};

