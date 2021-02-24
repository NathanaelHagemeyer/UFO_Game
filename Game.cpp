#include "pch.h"
#include "Game.h"


Game::Game()
{
	//Seed the random number table
	srand(time(NULL));

	//Create window, background and ground
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH + 100, SCREEN_HEIGHT),
		"UFO Game", sf::Style::Close);
	background = new sf::RectangleShape(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));
	ground = new sf::RectangleShape(sf::Vector2f(MAP_WIDTH, 100));

	//Set parameters of window, background, and HUD view
	window->setFramerateLimit(FPS);
	background->setFillColor(sf::Color(57, 28, 89));
	ground->setFillColor(sf::Color(0, 87, 8));
	ground->setPosition(sf::Vector2f(0, MAP_HEIGHT - 100));

	//Start music
	music = new sf::Music();

	if (music->openFromFile("UFO_TEMP_TRACK.ogg"))
	{
		music->setLoop(true);
		music->play();
	}
	else
		std::cout << "Failed to open music file" << std::endl;

	//Load textures
	loadTextures();

	//Create a UFO and set the view
	ufoPtr = new UFO(this, &ufoTexture, &beamTexture, MAP_WIDTH / 2, SCREEN_HEIGHT / 2);
	addObject(ufoPtr);
	mainView.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	//Create 200 stars
	for (int i = 0; i < 200; i++)
		effects.push_back(new Effect(&starTexture, rand() % MAP_WIDTH, rand() % (MAP_HEIGHT - 100),
			16, getFPS() / 10, 7, -1));
	//Create 50 grass
	for (int i = 0; i < 50; i++)
		effects.push_back(new Effect(&grassTexture, rand() % MAP_WIDTH, rand() % 100 + (MAP_HEIGHT - 100),
			16, getFPS() / 2, 6, -1));
	//Randomize where effects start in their animation cycle
	for (int i = 0; i < effects.size(); i++)
		effects.at(i)->getAnimation()->setFrame(rand() % effects.at(i)->getAnimation()->getNumFrames());

	score = 0;

}


Game::~Game()
{
	delete window;
	delete background;
	delete ground;
	delete music;

	for (int i = objects.size() - 1; i >= 0; i--)
		delete objects.at(i);

	for (int i = effects.size() - 1; i >= 0; i--)
		delete effects.at(i);

}


//Return max FPS of game
int Game::getFPS()
{
	return FPS;
}


//This can be multiplied by 4 to get the MAP_WIDTH
int Game::getScreenWidth()
{
	return SCREEN_WIDTH;
}


//This can be multiplied by 2 to get the MAP_HEIGHT
int Game::getScreenHeight()
{
	return SCREEN_HEIGHT;
}


//Allows other GameObjects to track the player, and makes code clearer
sf::Vector2f Game::getPlayerPos()
{
	return sf::Vector2f(ufoPtr->getX(), ufoPtr->getY());
}


/*
Contains main game loop. Runs menu/game/pause screen based on gamestate.
*/
void Game::run()
{
	int temp;

	while (window->isOpen())
	{
		sf::Event event;

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		//Check for cow spawn. Spawn it out of sight.
		if (rand() % (FPS * 10) + 1 == 1)
		{
			temp = rand() % MAP_WIDTH;

			if (getPlayerPos().y > SCREEN_HEIGHT)
			{
				temp = mainView.getCenter().x;

				if (temp > MAP_WIDTH / 2)
					temp = rand() % (temp - SCREEN_WIDTH / 2);
				else
					temp = rand() % (MAP_WIDTH - (temp + SCREEN_WIDTH / 2)) + temp + SCREEN_WIDTH / 2;
			}

			objects.push_back(new Cow(this, &cowRightTexture, &cowLeftTexture, temp,
				MAP_HEIGHT - 20));
		}
			

		//Update all entities
		updateAll();

		//Check for collisions
		checkCollisions();

		//Set the view to the proper position and draw to the window
		snapView();
		mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f * SCREEN_WIDTH / (SCREEN_WIDTH + 100), 1.f));
		window->setView(mainView);
		
		window->clear();
		window->draw(*background);
		window->draw(*ground);
		drawAll();

		/*//Set view to HUD view, lock it to the main view, and draw to it
		hudView.setViewport(sf::FloatRect(0.9f, 0.f, 0.1f, 1.f));
		//hudView.setSize(100, SCREEN_HEIGHT);
		hudView.reset(sf::FloatRect(sf::Vector2f()))
	*/

		window->setView(window->getDefaultView());
		drawHUD();

		//Draw everything
		window->display();
	}

	return;
}


//Helper function that updates all GameObjects
void Game::updateAll()
{
	//Update all entities 
	//First objects
	for (int i = 0; i < objects.size(); i++)
			objects.at(i)->update();
	//Then effects, but only within 100 pixels of the viewscreen
	for (int i = 0; i < effects.size(); i++)
	{
		if ((effects.at(i)->getX() < mainView.getCenter().x + SCREEN_WIDTH / 2 + 100 ||
			effects.at(i)->getX() > mainView.getCenter().x - SCREEN_WIDTH / 2 - 100) &&
			(effects.at(i)->getY() < mainView.getCenter().y + SCREEN_HEIGHT / 2 + 100 ||
				effects.at(i)->getY() > mainView.getCenter().y - SCREEN_HEIGHT / 2 - 100))
			effects.at(i)->update();
	}
	//Then delete and remove objects/effects that need to die
	//For objects check shouldDie();
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects.at(i)->shouldDie())
		{
			delete objects.at(i);
			objects.erase(objects.begin() + i);
		}
	}
	//For effects check their time-to-live
	for (int i = 0; i < effects.size(); i++)
	{
		if (effects.at(i)->getTTL() == 0)
		{
			delete effects.at(i);
			effects.erase(effects.begin() + i);
		}
	}

	return;
}


//Helper function that draws all objects
void Game::drawAll()
{
	//Draw all entities within 100 pixels of the viewscreen
	//First effects
	for (int i = 0; i < effects.size(); i++)
	{
		if ((effects.at(i)->getX() < mainView.getCenter().x + SCREEN_WIDTH / 2 + 100 ||
			effects.at(i)->getX() > mainView.getCenter().x - SCREEN_WIDTH / 2 - 100) &&
			(effects.at(i)->getY() < mainView.getCenter().y + SCREEN_HEIGHT / 2 + 100 ||
				effects.at(i)->getY() > mainView.getCenter().y - SCREEN_HEIGHT / 2 - 100))
			window->draw(*(effects.at(i)->getSprite()));
	}
	//Then objects
	for (int i = 0; i < objects.size(); i++)
	{
		if (((objects.at(i)->getX() < mainView.getCenter().x + SCREEN_WIDTH / 2 + 100 ||
			objects.at(i)->getX() > mainView.getCenter().x - SCREEN_WIDTH / 2 - 100) &&
			(objects.at(i)->getY() < mainView.getCenter().y + SCREEN_HEIGHT / 2 + 100 ||
				objects.at(i)->getY() > mainView.getCenter().y - SCREEN_HEIGHT / 2 - 100))
			&& objects.at(i)->isVisible())
			window->draw(*(objects.at(i)->getSprite()));
	}
	

	return;
}


//Snap the view to the UFO's x and y, but only within the map
void Game::snapView()
{
	//Snap viewport to player x & y until screen edges
	if (getPlayerPos().x < MAP_WIDTH - (SCREEN_WIDTH / 2) && getPlayerPos().x > SCREEN_WIDTH / 2
		&& getPlayerPos().y < MAP_HEIGHT - (SCREEN_HEIGHT / 2) && getPlayerPos().y > SCREEN_HEIGHT / 2)
		mainView.setCenter(getPlayerPos().x, getPlayerPos().y);
	//Snap viewport to top-right
	else if (getPlayerPos().x >= MAP_WIDTH - (SCREEN_WIDTH / 2) && getPlayerPos().y <= SCREEN_HEIGHT / 2)
		mainView.setCenter(MAP_WIDTH - (SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2);
	//Snap viewport to top-left
	else if (getPlayerPos().x <= SCREEN_WIDTH / 2 && getPlayerPos().y <= SCREEN_HEIGHT / 2)
		mainView.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//Snap viewport to bottom-right
	else if (getPlayerPos().x >= MAP_WIDTH - (SCREEN_WIDTH / 2) && getPlayerPos().y >= MAP_HEIGHT -
		(SCREEN_HEIGHT / 2))
		mainView.setCenter(MAP_WIDTH - (SCREEN_WIDTH / 2), MAP_HEIGHT - (SCREEN_HEIGHT / 2));
	//Snap viewport to bottom-left
	else if (getPlayerPos().x <= SCREEN_WIDTH / 2 && getPlayerPos().y >= MAP_HEIGHT - (SCREEN_HEIGHT / 2))
		mainView.setCenter(SCREEN_WIDTH / 2, MAP_HEIGHT - (SCREEN_HEIGHT / 2));
	//Snap viewport to right edge
	else if (getPlayerPos().x >= MAP_WIDTH - (SCREEN_WIDTH / 2))
		mainView.setCenter(MAP_WIDTH - (SCREEN_WIDTH / 2), getPlayerPos().y);
	//Snap viewport to left edge
	else if (getPlayerPos().x <= SCREEN_WIDTH / 2)
		mainView.setCenter(SCREEN_WIDTH / 2, getPlayerPos().y);
	//Snap viewport to top edge
	else if (getPlayerPos().y <= SCREEN_HEIGHT / 2)
		mainView.setCenter(getPlayerPos().x, SCREEN_HEIGHT / 2);
	//Snap viewport to bottom edge
	else if (getPlayerPos().y >= MAP_HEIGHT - (SCREEN_HEIGHT / 2))
		mainView.setCenter(getPlayerPos().x, MAP_HEIGHT - (SCREEN_HEIGHT / 2));

	return;
}


//Check for objects of different teams whose radii touch, and call collide on them
void Game::checkCollisions()
{
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects.size(); j++)
		{
			if ( (objects.at(i)->getTeam() != objects.at(j)->getTeam()) && objects.at(i)->getRadius() + 
				objects.at(j)->getRadius() > sqrt(pow(objects.at(i)->getX() - objects.at(j)->getX(), 2) + 
				pow(objects.at(i)->getY() - objects.at(j)->getY(), 2)) )
			{
				objects.at(i)->collide(objects.at(j));
				objects.at(j)->collide(objects.at(i));
			}
		}
	}

	return;
}


//Increase score by amount passed
void Game::addScore(int bonus)
{
	score += bonus;

	return;
}


//Add the passed pointer to the objects array
void Game::addObject(GameObject * newObject)
{
	objects.push_back(newObject);

	return;
}


//Loads the textures to be used
void Game::loadTextures()
{
	if (!ufoTexture.loadFromFile("UFO_FLYING_SPRITESHEET.png"))
		std::cout << "Failed to load UFO texture" << std::endl;
	if (!starTexture.loadFromFile("STAR_SPRITESHEET.png"))
		std::cout << "Failed to load star texture" << std::endl;
	if (!beamTexture.loadFromFile("ABDUCTION_BEAM_SPRITESHEET.png"))
		std::cout << "Failed to load beam texture" << std::endl;
	if (!grassTexture.loadFromFile("GRASS_SPRITESHEET.png"))
		std::cout << "Failed to load grass texture" << std::endl;
	if (!cowRightTexture.loadFromFile("COW_WALK_RIGHT_SPRITESHEET.png"))
		std::cout << "Failed to load cow_right texture" << std::endl;
	if (!cowLeftTexture.loadFromFile("COW_WALK_LEFT_SPRITESHEET.png"))
		std::cout << "Failed to load cow_right texture" << std::endl;

	return;
}


//
void Game::drawHUD()
{
	int* inventoryContents = dynamic_cast<UFO *>(ufoPtr)->getInventory();

	//Background
	sf::RectangleShape background(sf::Vector2f(100, SCREEN_HEIGHT));
	background.setFillColor(sf::Color(30, 30, 30));
	background.setPosition(sf::Vector2f(SCREEN_WIDTH, 0));
	//Inventory box
	sf::RectangleShape invBox(sf::Vector2f(64, 64));
	invBox.setFillColor(sf::Color::Transparent);
	invBox.setOutlineThickness(2.f);
	invBox.setOutlineColor(sf::Color::Black);

	window->draw(background);

	//Inventory boxes
	for (int i = 0; i < 5; i++)
	{
		invBox.setPosition(SCREEN_WIDTH + 18, 250 + i * 66);
		window->draw(invBox);
	}

	//Set contents of each inventory square, then draw
	for (int i = 0; i < 5; i++)
	{
		switch (inventoryContents[i])
		{
		case nothing:
			break;
		case cow:
			inventoryDisplay[i].setPosition(SCREEN_WIDTH + 34, 258 + i * 66);
			inventoryDisplay[i].setTexture(cowLeftTexture);
			inventoryDisplay[i].setTextureRect(sf::IntRect(0, 0, 32, 32));
			window->draw(inventoryDisplay[i]);
			break;
		default:
			break;
		}
	}



	return;
}
