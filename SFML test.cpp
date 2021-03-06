/*
Nathanael Hagemeyer
Test:
1) Compile SFML window - done
2) Upload and display sprite - done
3) Make sprite move w/ keyboard input - done
4) Update sprite based on movement - done
5) Move sprite & functionality into class UFO - done
6) Add base class GameObject and make UFO a child - done
7) Add UFO acceleration - done
8) Create wrapper class Game - done

*/

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"


int main()
{
	Game game = Game();

	game.run();

	return 0;
}


