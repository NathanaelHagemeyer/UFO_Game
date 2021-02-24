#include "pch.h"
#include "UFO.h"



UFO::UFO( Game *game, sf::Texture * texturePtr, sf::Texture * beamTexturePtr, 
	float x, float y) : GameObject(game, x, y)
{
	this->texturePtr = texturePtr;
	this->beamTexturePtr = beamTexturePtr;

	//Initialize the Animation
	animation = new Animation(14, 64, game->getFPS() / 5);

	//Initialize the beam
	beamPtr = new AbductionBeam(gamePtr, beamTexturePtr, this->x, this->y + 48);
	gamePtr->addObject(beamPtr);
	beamPower_MAX = game->getFPS() * 4;
	beamPower = beamPower_MAX;

	//Initialize inventory
	initInventory();
	currentSpace = 0;

	//Prepare the sprite
	sprite.setTexture(*texturePtr);
	sprite.setTextureRect(animation->calcTextRect());
	sprite.setOrigin(32, 32); //Might be wrong
	sprite.setPosition(x, y);

	team = 1;
	radius = 27;
}


UFO::~UFO()
{
	delete animation;
}


/*
Move UFO based on keyboard input & update sprite accordingly
*/
void UFO::update()
{
	bool accelUp = false;
	bool accelDown = false;
	bool accelLeft = false;
	bool accelRight = false;

	//Keyboard input to determine how to accelerate
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		accelUp = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		accelLeft = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		accelDown = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		accelRight = true;

	//If opposing keys pressed cancel acceleration
	if (accelUp && accelDown) {
		accelUp = false;
		accelDown = false;
	}
	if (accelRight && accelLeft) {
		accelRight = false;
		accelLeft = false;
	}

	//Accelerate ship based on keys pressed
	if (accelUp && accelRight) {
		yVel -= accel * std::sqrt(2) / 2;
		xVel += accel * std::sqrt(2) / 2;
	}
	else if (accelUp && accelLeft) {
		yVel -= accel * std::sqrt(2) / 2;
		xVel -= accel + std::sqrt(2) / 2;
	}
	else if (accelDown && accelRight) {
		yVel += accel * std::sqrt(2) / 2;
		xVel += accel * std::sqrt(2) / 2;
	}
	else if (accelDown && accelLeft) {
		yVel += accel * std::sqrt(2) / 2;
		xVel -= accel * std::sqrt(2) / 2;
	}
	else if (!accelLeft && !accelRight && (accelUp || accelDown)) {
		//Decrease magnitude of xVel
		if (xVel > 0) {
			xVel -= accel / 2;
			if (xVel < 0)
				xVel = 0;
		}
		if (xVel < 0) {
			xVel += accel / 2;
			if (xVel > 0)
				xVel = 0;
		}
		//Increase or decrease yVel as appropriate
		if (accelUp)
			yVel -= accel;
		if (accelDown)
			yVel += accel;
	}
	else if (!accelDown && !accelUp && (accelRight || accelLeft)) {
		//Decrease magnitude of yVel
		if (yVel > 0) {
			yVel -= accel / 2;
			if (yVel < 0)
				yVel = 0;
		}
		if (yVel < 0) {
			yVel += accel / 2;
			if (yVel > 0)
				yVel = 0;
		}
		//Increase or decrease yVel as appropriate
		if (accelRight)
			xVel += accel;
		if (accelLeft)
			xVel -= accel;
	}
	else
	{
		//Decrease magnitude of all velocity
		if (xVel > 0) {
			xVel -= accel / 2;
			if (xVel < 0)
				xVel = 0;
		}
		if (xVel < 0) {
			xVel += accel / 2;
			if (xVel > 0)
				xVel = 0;
		}
		if (yVel > 0) {
			yVel -= accel / 2;
			if (yVel < 0)
				yVel = 0;
		}
		if (yVel < 0) {
			yVel += accel / 2;
			if (yVel > 0)
				yVel = 0;
		}
	}

	//Ensure velocity does not exceed maximum
	if (xVel > xVelMax)
		xVel = xVelMax;
	if (xVel < xVelMax * -1)
		xVel = xVelMax * -1;
	if (yVel > yVelMax)
		yVel = yVelMax;
	if (yVel < yVelMax * -1)
		yVel = yVelMax * -1;

	//Update position based on velocity
	GameObject::update();

	//Ensure position is w/in bounds, resetting sprite position if necessary
	if (y > gamePtr->getScreenHeight() * 2 - 100 + radius)
		y = gamePtr->getScreenHeight() * 2 - 100 + radius;
	else if (y < 100 + radius)
		y = 100 + radius;

	//Wrap object around screen
	if (x < -(radius))
		x = gamePtr->getScreenWidth() * 4;
	else if (x > gamePtr->getScreenWidth() * 4 + radius)
		x = 0;

	//Handling beam
	//Interpret the beam as an AbductionBeam
	if (dynamic_cast<AbductionBeam*> (beamPtr) != nullptr)
	{
		//If beam is inactive, and we have at least a quarter of the charge bar, 
		//and the beam key is pressed
		if (dynamic_cast<AbductionBeam*> (beamPtr)->getPower() == 0 && beamPower > beamPower_MAX / 4
			&& sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		{
			//Set beam power to our beamPower
			dynamic_cast<AbductionBeam*> (beamPtr)->setPower(beamPower);
		}
		//If beam is active, we have beamPower, and beam key is pressed
		else if (dynamic_cast<AbductionBeam*> (beamPtr)->getPower() > 0 && beamPower > 0 &&
			sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		{
			//There is a beam, so use power to sustain it
			beamPower--;
			dynamic_cast<AbductionBeam*> (beamPtr)->setPower(beamPower);
		}
		//If the beam is active and the player releases the beam key, or the UFO runs out of
		//beam power, deactivate it
		else if (dynamic_cast<AbductionBeam*> (beamPtr)->isActive() &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::O) || beamPower == 0)
		{
			dynamic_cast<AbductionBeam*> (beamPtr)->setPower(0);
		}

		//Beam inactive, so give back some beamPower
		if (!dynamic_cast<AbductionBeam*> (beamPtr)->isActive() && beamPower < beamPower_MAX)
			beamPower++;

		//Lock beam to ufo pos
		dynamic_cast<AbductionBeam*> (beamPtr)->setPosition(sf::Vector2f(x, y + 48));
		dynamic_cast<AbductionBeam*> (beamPtr)->GameObject::update();
	}
	else
		std::cout << "beamPtr is invalid" << std::endl;

	//Update sprite
	animation->update();
	
	sprite.setTextureRect(animation->calcTextRect());
	
	return;
}


void UFO::collide(GameObject * other)
{
	if (dynamic_cast<Cow *> (other) != nullptr)
	{
		//If we have space and the cow is valid
		if (!inventoryFull() && !dynamic_cast<Cow *> (other)->shouldDie())
		{
			inventory[currentSpace] = cow;
			currentSpace++;

			other->kill();
		}
	}

	return;
}


void UFO::emptyInventory()
{
	initInventory();
	currentSpace = 0;
}


int * UFO::getInventory()
{
	return inventory;
}

bool UFO::inventoryFull()
{
	if (currentSpace == INVENTORY_MAX)
		return true;

	return false;
}


//Helper function that sets inventory values
void UFO::initInventory()
{
	for (int i = 0; i < INVENTORY_MAX; i++)
			inventory[i] = 0;

	return;
}
