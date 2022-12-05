#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Cell.h"

class Maze
{
protected:
	int mazeSize;
	int cellSize;
	int wallSize;
	int mazeDimension;

	int rightMargin;
	int bottomMargin;

	int windowWidth;
	int windowHeight;
	std::vector<int> playerPosition;
	sf::RenderWindow window;

public:
	Maze(int mazeSize);
	void run();
	/*void init();
	void drawMaze();
	void drawStartFinish();
	void drawPlayer();
	void drawStats();*/
	~Maze();
};

