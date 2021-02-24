#pragma once
#include <SFML/Graphics.hpp>


class Animation
{
public:
	Animation(int, int, int);
	~Animation();
	void update();
	sf::IntRect calcTextRect();
	int getNumFrames();
	int getAnimSize();
	int getAnimSpeed();
	void setFrame(int);

private:
	int numFrames; //Number of different animation frames
	int animSize; //Width and height of sprite
	int animSpeed; //Game frames until animation frame change
	int frame; //Where we are in the animation cycle
	int count; //Time till frame change

};

