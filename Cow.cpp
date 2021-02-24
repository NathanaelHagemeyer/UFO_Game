#include "pch.h"
#include "Cow.h"


Cow::Cow(Game *gamePtr, sf::Texture *cowRight, sf::Texture *cowLeft, float x, float y)
	: GameObject(gamePtr, x, y)
{
	//Initialize textures
	rightTexture = cowRight;
	leftTexture = cowLeft;

	//Initialize animation
	animation = new Animation(4, 32, gamePtr->getFPS() / 4);

	//Prepare the sprite
	sprite.setTexture(*cowRight);
	sprite.setTextureRect(animation->calcTextRect());
	sprite.setOrigin(16, 16); //Might be wrong
	sprite.setPosition(x, y);

	team = 2;
	radius = 13;
	state = 0; //stand
	stateLength = 0;
}

Cow::~Cow()
{
	delete animation;
}


void Cow::update()
{
	int temp;

	//If state over
	if (stateLength == 0)
	{
		//Select new state
		temp = rand() % 100 + 1;

		if (temp > 80)
		{
			state = 2; //Walk left
			stateLength = gamePtr->getFPS() * 2;
			sprite.setTexture(*leftTexture);
		}
		else if (temp > 60)
		{
			state = 1; //Walk right
			stateLength = gamePtr->getFPS() * 2;
			sprite.setTexture(*rightTexture);
		}
		else
		{
			state = 0;
			stateLength = gamePtr->getFPS();
			xVel = 0;
		}
	}

	//Update based on state (and decrement stateLength)
	switch (state)
	{
	case 0:
		//Stand still
		break;
	case 1:
		//Walk right
		animation->update();
		xVel = speed;
		break;
	case 2:
		//Walk left
		animation->update();
		xVel = -speed;
		break;
	}

	stateLength--;

	//Update sprite
	sprite.setTextureRect(animation->calcTextRect());

	//If being abducted, float upwards and cancel horizontal movement
	if (beingAbducted)
	{
		xVel = 0;
		yVel = -1;
		x = gamePtr->getPlayerPos().x;
	}
	//Otherwise, if above ground level, fall
	else if (y < gamePtr->getScreenHeight() * 2 - 20)
	{
		yVel += 1;
	}

	//Wrap object around screen
	if (x < -(radius))
		x = gamePtr->getScreenWidth() * 4;
	else if (x > gamePtr->getScreenWidth() * 4 + radius)
		x = 0;

	GameObject::update();

	//Ensure position is w/in bounds, resetting sprite position if necessary
	if (y > gamePtr->getScreenHeight() * 2 - 20)
	{
		y = gamePtr->getScreenHeight() * 2 - 20;
		yVel = 0; //Stop falling
		GameObject::update();
	}

	beingAbducted = false;

	return;
}


void Cow::collide(GameObject *other)
{
	if (dynamic_cast<AbductionBeam *> (other) != nullptr)
		if (dynamic_cast<AbductionBeam *> (other)->isActive())
			beingAbducted = true;
	if (dynamic_cast<UFO *> (other) != nullptr)
	{
		if (dynamic_cast<UFO *> (other)->inventoryFull())
		{
			//'Launch' the cow 
			//Left or right
			if (rand() % 2 == 1)
				x += rand() % 40 + 31;
			else
				x -= rand() % 40 + 31;
			//Launch upwards
			y -= rand() % 30 + 10;
		}
	}

	return;
}
