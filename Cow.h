#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Animation.h"
#include <cstdlib>

class Cow :
	public GameObject
{
public:
	Cow(Game *, sf::Texture *, sf::Texture*, float, float);
	~Cow();
	void update() override;
	void collide(GameObject *) override;

private:
	//Textures
	sf::Texture * rightTexture;
	sf::Texture * leftTexture;

	//Consts
	const float speed = 0.5f;

	Animation * animation;
	int state;
	int stateLength;
};

