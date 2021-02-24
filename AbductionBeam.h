#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Animation.h"
//#include "SFML/Graphics.hpp"


class AbductionBeam :
	public GameObject
{
public:
	AbductionBeam(Game *, sf::Texture *, float, float);
	~AbductionBeam();
	void update() override;
	void setPosition(sf::Vector2f);
	void setPower(int);
	int getPower();
	bool isActive();
private:
	Animation * animation;
	sf::Texture *  texturePtr;
	int power;
	bool active;
};

