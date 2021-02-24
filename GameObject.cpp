#include "pch.h"
#include "GameObject.h"


//Create a gameObject at a specific location
GameObject::GameObject(Game * gamePtr, float x, float y)
{
	this->x = x;
	this->y = y;
	this->gamePtr = gamePtr;

	xVel = 0;
	yVel = 0;
	radius = 32.0;
	team = 0;
	toDie = false;
	visible = true;
	beingAbducted = false;
}


//Create a gameObject with the same values as other (the object that created this)
GameObject::GameObject(GameObject *other)
{
	x = other->getX();
	y = other->getY();
	xVel = other->getXVel();
	yVel = other->getYVel();
	radius = other->getRadius();
	team = other->getTeam();
	toDie = false;
	visible = other->isVisible();
}


GameObject::~GameObject()
{
}


//Receives the object that collided with this and updates own variables accordingly
void GameObject::collide(GameObject *other)
{
	return;
}


//Called every frame (Game runs at 30 fps)
void GameObject::update()
{
	x += xVel;
	y += yVel;

	sprite.setPosition(x, y);

	return;
}


void GameObject::kill()
{
	toDie = true;
}


void GameObject::setVisible(bool newVisibility)
{
	visible = newVisibility;
}


//Returns the sprite of the GameObject
sf::Sprite * GameObject::getSprite()
{
	return &sprite;
}


//Returns object's x-pos, from center of sprite
float GameObject::getX(){ return x; }


//Returns object's y-pos, from center of sprite
float GameObject::getY(){ return y; }


//Returns object's x-velocity
float GameObject::getXVel() { return xVel; }


//Returns object's y-velocity
float GameObject::getYVel() { return yVel; }


//Returns object's radius (for collision detection)
float GameObject::getRadius() { return radius; }


//Returns object's team (for collision detection)
int GameObject::getTeam() { return team; }


bool GameObject::shouldDie()
{
	return toDie;
}

bool GameObject::isVisible()
{
	return visible;
}

bool GameObject::isBeingAbducted()
{
	return beingAbducted;
}
