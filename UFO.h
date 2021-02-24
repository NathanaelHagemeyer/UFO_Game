#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Animation.h"
#include "GameObject.h"
#include "GameObjectEnum.h"
#include "AbductionBeam.h"
#include "Game.h"


class UFO : public GameObject
{
public:
	UFO(Game *, sf::Texture *, sf::Texture *, float, float);
	~UFO();
	void update() override;
	void collide(GameObject*) override;
	void emptyInventory();
	int* getInventory();
	bool inventoryFull();

	const static int INVENTORY_MAX = 5;

private:
	Animation * animation = nullptr;
	sf::Texture *  texturePtr = nullptr;

	const float accel = .5; //Acceleration
	//Max velocity
	const float xVelMax = 7.;
	const float yVelMax = 7.;

	//Abduction beam
	int beamPower_MAX;
	int beamPower;
	sf::Texture * beamTexturePtr = nullptr;
	GameObject* beamPtr = nullptr;

	//Inventory
	int inventory[INVENTORY_MAX];
	int currentSpace; //Where are we at in the inventory

	//Functions
	void initInventory();

	
};

