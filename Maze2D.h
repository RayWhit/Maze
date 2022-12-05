#pragma once

#include "Maze.h"
#include "Cell.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


class Maze2D : public Maze
{
protected:
	std::vector<std::vector<Cell>> maze;
	/*int cellSize;
	int wallSize;
	int mazeSize;
	int rightMargin;
	int bottomMargin;
	std::pair<int, int> playerPosition;*/

	sf::RenderWindow window;

public:
	Maze2D(int mazeSize);
	void run();
	void init();
	void drawMaze();
	void drawStartFinish();
	void drawPlayer();
	void drawStats();
	~Maze2D();
};

