#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "GameObjectEnum.h"
#include "Effect.h"
#include "UFO.h"
#include "AbductionBeam.h"
#include "Cow.h"
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>


class Game
{
public:
	Game();
	~Game();

	int getFPS();
	int getScreenWidth();
	int getScreenHeight();
	sf::Vector2f getPlayerPos();
	void run(); //Game loop
	void updateAll(); //Call update on every GameObject
	void drawAll(); //Call draw on every GameObject
	void snapView(); //Sets view to appropriate coordinates
	void checkCollisions(); //Check for collisions, calling collide() on appropriate objects
	void addScore(int);
	void addObject(GameObject *);
	//void addEffect(Effect *);

private:
	//Functions
	void loadTextures(); //Load necessary textures
	void drawHUD(); //Draw the player's lives, inventory, and charge bar

	//Variables
	//Game constants: Max FPS, screen size, map size (in screens)
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 600;
	const int MAP_WIDTH = SCREEN_WIDTH * 4;
	const int MAP_HEIGHT = SCREEN_HEIGHT * 2;
	const int FPS = 60;

	//Pointer to the UFO
	GameObject *ufoPtr = nullptr;

	//Vector holding all game entities.
	std::vector<GameObject *> objects;
	//Vector holding all visual effects.
	std::vector<Effect *> effects;
	//Array of sprites for displaying player inventory
	sf::Sprite inventoryDisplay[5];

	//List of textures
	sf::Texture ufoTexture;
	sf::Texture starTexture;
	sf::Texture beamTexture;
	sf::Texture grassTexture;
	sf::Texture cowRightTexture;
	sf::Texture cowLeftTexture;

	sf::Music *music = nullptr;
	sf::RenderWindow *window = nullptr;

	//Views
	sf::View mainView;
	sf::View hudView;

	//Background and ground
	sf::RectangleShape *background = nullptr;
	sf::RectangleShape *ground = nullptr;


	int score;

	
};

