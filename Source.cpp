#include <SFML/Graphics.hpp>

#include <iostream>

#include "Cell.h"
#include "Maze2D.h"


int main() {

    Maze maze(3);
    maze.run();

    //Maze2D maze2D(5);
    //maze2D.run();

  
    return 0;
}