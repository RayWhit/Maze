#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Cell.h"
#include "Maze.h"

class Maze3D : Maze
{
protected:
	std::vector<std::vector<std::vector<Cell>>> maze;

	sf::RenderWindow window;

public:
	Maze3D(int size);
	void run();
	void init();
	void drawMaze();
	void drawStartFinish();
	void drawPlayer();
	void drawStats();
	~Maze3D();


};

