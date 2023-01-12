#include "Maze.h"
#include "Maze2D.h"
#include "Maze3D.h"
#include "Maze4D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <stack>
#include <random>
#include <math.h>


Maze::Maze(int mazeSize) {
    this->mazeSize = mazeSize;
    this->cellSize = 750 / this->mazeSize;
    this->wallSize = this->cellSize * 0.02 + 1;
    this->playerPosition;
    this->rightMargin = 200;
    this->bottomMargin = 50;
    this->mazeDimension = 2;

    this->windowWidth = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->rightMargin;
    this->windowHeight = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->bottomMargin;
}

void Maze::run() {
    this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "Maze", sf::Style::Close);
    this->window.clear(sf::Color::White);

    int tempDimension = this->mazeDimension;
    int tempMazeSize = this->mazeSize;

    // text creation
    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    if (!font.loadFromFile("ARIAL.TTF"))
    {
        std::cout << "Font loading error";
    }
    sf::Text text("", font);

    text.setPosition(0, 0);
    text.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::TextEntered:
                if ((char)e.text.unicode == 'w') {
                    tempMazeSize++;
                }
                else if ((char)e.text.unicode == 's' && tempMazeSize > 1) {
                    tempMazeSize--;
                }
                else if ((char)e.text.unicode == 'i' && tempDimension < 4) {
                    tempDimension++;
                }
                else if ((char)e.text.unicode == 'k' && tempDimension > 2) {
                    tempDimension--;
                }
                break;

            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Enter) {
                    this->window.close();
                    this->mazeSize = tempMazeSize;
                    if (tempDimension == 2) {
                        Maze2D maze2D(this->mazeSize);
                        maze2D.run();
                        this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
                    }
                    else if(tempDimension == 3) {
                        Maze3D maze3D(this->mazeSize);
                        maze3D.run();
                        this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "3D Maze", sf::Style::Close);
                    }
                    else if (tempDimension == 4) {
                        Maze4D maze4D(this->mazeSize);
                        maze4D.run();
                        this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "4D Maze", sf::Style::Close);
                    }
                }
                break;
            }
        }

        std::string menu =
            "Welcome to Ray's maze\n"
            "Use w/s to set maze size and i/k to set maze dimension\n\n"
            "Maze size: " + std::to_string(tempMazeSize) + "\n"
            "Maze dimension: " + std::to_string(tempDimension) + "\n"
            "Total number of cells: " + std::to_string((int)pow(tempMazeSize, tempDimension)) + "\n\n"
            "Enter to start\n\n"
            "Controls: \n";

        switch (tempDimension) {
            case 2:
                menu +=
                    "\tws - x axis"
                    "\n\tad - y axis"
                    "\n\t(standard maze - you know the controls)";
                break;

            case 3:
                menu +=
                    "\tws - x axis"
                    "\n\tad - y axis"
                    "\n\tik - z axis"
                    "\n\t(getting harder but still playable)";
                break;

            case 4:
                menu +=
                    "\tws - x axis"
                    "\n\tad - y axis"
                    "\n\tik - z axis"
                    "\n\tjl - w axis"
                    "\n\t(w axis?!)";
                break;
        }
        text.setString(menu);

        this->window.clear(sf::Color::White);
        this->window.draw(text);
        this->window.display();
    }
}

Maze::~Maze() {

}
