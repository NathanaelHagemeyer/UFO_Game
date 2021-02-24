#include "pch.h"
#include "AbductionBeam.h"


AbductionBeam::AbductionBeam(Game * game, sf::Texture * texturePtr,	float x, float y) : GameObject(game, x, y)
{
	this->texturePtr = texturePtr;

	//Initialize the Animation
	animation = new Animation(2, 64, game->getFPS() / 4);

	power = 0;

	//Prepare the sprite
	sprite.setTexture(*texturePtr);
	sprite.setTextureRect(animation->calcTextRect());
	sprite.setOrigin(32, 38); //Adjust lower
	sprite.setPosition(x, y);

	team = 1;
	radius = 26;

	visible = false;
	active = false;
}


AbductionBeam::~AbductionBeam()
{
	delete animation;
}

void AbductionBeam::setPosition(sf::Vector2f newPos)
{
	this->x = newPos.x;
	this->y = newPos.y;
}


void AbductionBeam::setPower(int powerBonus)
{
	power = powerBonus;
}


void AbductionBeam::update()
{
	//If we have power, make the beam visible and active
	if (power > 0) {
		visible = true;
		active = true;
	}
	else
	{
		visible = false;
		active = false;
	}

	GameObject::update();

	//Update sprite
	animation->update();

	sprite.setTextureRect(animation->calcTextRect());
}


int AbductionBeam::getPower()
{
	return power;
}


bool AbductionBeam::isActive()
{
	return active;
}
