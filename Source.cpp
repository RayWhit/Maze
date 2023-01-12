#include <SFML/Graphics.hpp>
#include <iostream>
#include "Cell.h"
#include "Maze2D.h"

int main() {
    Maze maze(5);
    maze.run();
    return 0;
}