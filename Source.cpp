#include <SFML/Graphics.hpp>
#include <iostream>
#include "Cell.h"
#include "Maze2D.h"

int main() {
    std::string exit = "n";
    while (exit == "n") {
        int mazeSize = 0;
        while (mazeSize < 1) {
            std::cout << "Choose a maze size: ";
            std::cin >> mazeSize;
        }

        Maze maze(mazeSize);
        maze.run();

        std::cout << "Do you want to exit? (y/n)";
        std::cin >> exit;
        system("cls");
    }
    
  
    return 0;
}