#pragma once
#include <SFML/Graphics.hpp>

class Game;


class GameObject
{
public:
	GameObject(Game *, float, float);
	GameObject(GameObject*);
	~GameObject();
	virtual void collide(GameObject*);
	virtual void update();
	void kill();
	void setVisible(bool);
	sf::Sprite * getSprite();
	float getX();
	float getY();
	float getXVel();
	float getYVel();
	float getRadius();
	int getTeam();
	bool shouldDie();
	bool isVisible();
	bool isBeingAbducted();

protected:
	Game * gamePtr = nullptr;
	sf::Sprite sprite;
	float x;
	float y;
	float xVel;
	float yVel;
	float radius;
	int team;
	bool toDie;
	bool visible;
	bool beingAbducted;
};

